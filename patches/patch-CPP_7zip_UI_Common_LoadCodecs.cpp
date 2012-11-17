--- CPP/7zip/UI/Common/LoadCodecs.cpp	Sat Jan 22 19:46:28 2011
+++ CPP/7zip/UI/Common/LoadCodecs.cpp	Sat Jan 22 19:46:28 2011
@@ -25,6 +25,16 @@
 #include "Windows/Registry.h"
 #else
 #include "Common/StringConvert.h"
+#include <sys/param.h>
+#include <sys/stat.h>
+#include <sys/vnode.h>
+#include <sys/sysctl.h>
+#define _KERNEL /* for DTYPE_* */
+#include <sys/file.h>
+#undef _KERNEL
+#include <kvm.h>
+#include <libgen.h>
+#include <assert.h>
 #endif
 
 using namespace NWindows;
@@ -34,6 +44,24 @@
 extern HINSTANCE g_hInstance;
 #endif
 
+bool find_exe(struct kinfo_file2 *k,char *f)
+{if(!(DTYPE_VNODE==k->f_type && KERN_FILE_TEXT==k->fd_fd && (~FWRITE&FREAD)==((FWRITE|FREAD)&k->f_flag) && VREG == k->v_type && VTEXT == k->v_flag && 0==k->f_offset && 0==k->f_rbytes && 0==k->f_wbytes && 0==k->f_rxfer && 0==k->f_rwfer))
+  return(false);
+else
+ {struct stat s;
+  if(0!=stat(f,&s))
+   return(false);
+  else
+   {if(s.st_rdev!=k->va_rdev||s.st_dev!=k->va_fsid||s.st_ino!=k->va_fileid)
+     return(false);
+    else
+     {assert(s.st_size==k->va_size&&s.st_mode==k->va_mode&&0==memcmp(realpath(f,NULL),k->f_mntonname,strnlen(k->f_mntonname,MAXPATHLEN)));
+      return(true);
+     }
+   }
+ }
+}
+
 static CSysString GetLibraryFolderPrefix()
 {
   #ifdef _WIN32
@@ -45,6 +73,45 @@
   #else
   const char *p7zip_home_dir = getenv("P7ZIP_HOME_DIR");
   if (p7zip_home_dir == 0) p7zip_home_dir="./";
+
+{const pid_t pid=getpid();
+ size_t len=strnlen(p7zip_home_dir,MAXPATHLEN);
+ char *buf=(char*)alloca(_POSIX2_LINE_MAX>len?_POSIX2_LINE_MAX:len);
+ kvm_t *kd=kvm_openfiles(NULL,NULL,NULL,KVM_NO_FILES,buf);
+ assert(NULL!=kd);
+ int cnt;
+ struct kinfo_file2 *k=kvm_getfile2(kd,KERN_FILE_BYPID,pid,sizeof(*k),&cnt);
+ assert(NULL!=k);
+ assert(0<cnt && pid==k->p_pid /* && 0==strncmp(basename(argv[0]),k->p_comm,MAXPATHLEN) */ );
+ strlcpy(buf,p7zip_home_dir,len);
+ len=1+strnlen(k->p_comm,MAXPATHLEN);
+ assert(1<len);   
+ char *p_comm=(char*)alloca(len);
+ *p_comm='/';
+ strlcpy(p_comm+1,k->p_comm,len);
+ if(!find_exe(k,strncat(buf,p_comm,len)))
+  {char *paths_env=getenv("PATH");
+   size_t paths_env_len=strnlen(paths_env,32767);
+   char *path,*paths=(char*)alloca(paths_env_len);
+   strlcpy(paths,paths_env,paths_env_len);
+   for(;NULL!=(path=strsep(&paths,":"));)
+    {size_t path_len=strnlen(path,MAXPATHLEN);
+     char *f=(char*)alloca(path_len+len);
+     strlcpy(f,path,path_len+1);
+     strlcpy(f+path_len,p_comm,1+len);
+     if(find_exe(k,f))
+      {f[path_len+1]=0;
+       setenv("P7ZIP_HOME_DIR",f,1);
+       p7zip_home_dir=getenv("P7ZIP_HOME_DIR");
+       goto FOUND_EXE;
+      }
+    }
+   printf("Can't find_exe '%s' in '%s:%s'\n",p_comm+1,realpath(p7zip_home_dir,NULL),paths_env);
+   FOUND_EXE:;
+  }
+ cnt=kvm_close(kd); assert(0==cnt);
+}
+
 #ifdef _UNICODE
   return MultiByteToUnicodeString(p7zip_home_dir);
 #else
