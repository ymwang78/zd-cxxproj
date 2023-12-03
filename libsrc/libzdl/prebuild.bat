del zdl_grammar.tab.*
del zdl_lex.cpp
set PATH=D:\WinApp\DevTools\win_flex_bison-2.5.25\;%PATH%;
win_flex zdl_lex.l
win_bison -d -o zdl_grammar.tab.cpp zdl_grammar.y