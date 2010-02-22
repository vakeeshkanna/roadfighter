%{
#include <stdio.h>
#include "y.tab.h"
extern int  yylval;
%}
%%
[0-9]+                  yylval=strdup(yytext); return NUMBER;
heat|Heat|HEAT          return TOKHEAT;
on|off                  yylval=!strcmp(yytext,"on"); return STATE;
target                  return TOKTARGET;
temperature             return TOKTEMPERATURE;
\n                      /* ignore end of line */;
[ \t]+                  /* ignore whitespace */;
%%
