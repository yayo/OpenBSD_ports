--- src/misc1.c	Sun Aug 15 11:24:05 2010
+++ src/misc1.c	Sun Aug 15 11:24:05 2010
@@ -4116,7 +4116,7 @@
 
     if (vimdir == NULL || *vimdir == NUL)
 	return NULL;
-    p = concat_fnames(vimdir, (char_u *)VIM_VERSION_NODOT, TRUE);
+    p = concat_fnames(vimdir, "", TRUE);
     if (p != NULL && mch_isdir(p))
 	return p;
     vim_free(p);
