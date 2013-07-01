--- src/main.c      Thu Mar  7 23:32:54 2013
+++ src/main.c      Mon Jul  1 18:02:14 2013
@@ -554,6 +554,16 @@
     debug_break_level = params.use_debug_break_level;
 #endif

+    do_cmdline_cmd("so /usr/share/vim/syntax/syntax.vim");
+    do_cmdline_cmd("set nocompatible");
+    do_cmdline_cmd("noremap <F5> :update<CR>");
+    do_cmdline_cmd("inoremap <F5> <C-O>:update<CR>");
+    do_cmdline_cmd("set backspace=indent,eol,start");
+    do_cmdline_cmd("set hlsearch");
+    do_cmdline_cmd("set ww=<>[]");
+    do_cmdline_cmd("set et");
+    do_cmdline_cmd("set ts=1");
+
 #ifdef FEAT_MZSCHEME
     /*
      * Newer version of MzScheme (Racket) require earlier (trampolined)
