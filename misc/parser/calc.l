%{
#include <stdio.h>
#include "y.tab.h"
extern int  yylval;
%}
%%
[0-9]+          yylval=atoi(yytext); return DIGIT;
[a-zA-Z]		    return LETTER; 
[ \n]+          /* eat whitespace */;
.               return (int) yytext[0];
%%