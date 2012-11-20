--- src/main.c	Sun Aug  8 13:09:55 2010
+++ src/main.c	Sun Aug  8 13:09:55 2010
@@ -558,6 +558,16 @@
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
     /* Execute --cmd arguments. */
     exe_pre_commands(&params);
 
