%{
#include <stdio.h>
#include "globals.h"
#include "scan.h"
#include "cminus.tab.h"

char tokenString[1024];
int numLine = 0;
%}

%%
int               { return INT; }
while             { return WHILE; }
if                { return IF; }
else              { return ELSE; }
void              { return VOID; }
return            { return RETURN; }

[a-zA-Z]+         { return ID; }
[0-9]+            { return NUM; }

" "
\t
\n                { yylineno++; }


"+"               { return PLUS; }
"-"               { return MINUS; }
"="               { return ASSIGN; }
"=="              { return EQ; }
"!="              { return NE; }
"/"               { return OVER; }
"*"               { return TIMES; }
"<"               { return LT; }
"<="              { return LE; }
">"               { return GT; }
">="              { return GE; }
";"               { return SEMI; }
","               { return COMMA; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"["               { return LSQUARE; }
"]"               { return RSQUARE; }
"{"               { return LCURLY; }
"}"               { return RCURLY; }
"/*"              {
                    char before; char now;
                    before = input();
                    do {
                      now = input();
                      if(now == EOF) break;
                      if(now == '\n') yylineno++;
                      if(before == '*' && now == '/') {
                        break;
                      } else {
                        before = now;
                      }
                    } while(1);
                  }
.                 { /* do nothing */ }
%%

int getToken(void)
{
  static int first = 1;
  if(!first) {
    strncpy(tokenString, yytext, 1024);
  }
  first = 0;
  int currentToken = yylex();
  return currentToken;
}


