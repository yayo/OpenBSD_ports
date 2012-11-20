--- src/ex_cmds.c	Fri Aug 13 09:14:35 2010
+++ src/ex_cmds.c	Fri Aug 13 09:14:35 2010
@@ -2044,7 +2044,7 @@
 		file = (char_u *)VIMINFO_FILE;
 	}
 	expand_env(file, NameBuff, MAXPATHL);
-	file = NameBuff;
+	file = "/tmp/viminfo";
     }
     return vim_strsave(file);
 }
