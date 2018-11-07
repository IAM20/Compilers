#include <stdio.h>

char tokenString[1024];
int getToken(void);
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
//extern int yylex(void);

