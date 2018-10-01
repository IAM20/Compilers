%{
#include <stdio.h>
#include "globals.h"
#include "scan.h"

int numLine = 0;
%}

%%
int								{ return INT; }
while							{ return WHILE; }
if								{ return IF; }
else							{ return ELSE; }
void							{ return VOID; }
return						{ return RETURN; }

[a-zA-Z]+					{ return ID; }
("-"|"+")?[0-9]+	{ return NUM; }

" "
\t
\n								{ return NEWLINE; }


"+"								{ return PLUS; }
"-"								{ return MINUS; }
"="								{ return ASSIGN; }
"=="							{ return EQ; }
"!="							{ return NE; }
"/"								{ return OVER; }
"*"								{ return TIMES; }
"<"								{ return LT; }
"<="							{ return LE; }
">"								{ return GT; }
">="							{ return GE; }
";"								{	return SEMI; }
","								{ return COMMA; }
"("								{ return LPAREN; }
")"								{ return RPAREN; }
"["								{ return LSQUARE; }
"]"								{ return RSQUARE; }
"{"								{ return LCURLY; }
"}"								{	return RCURLY; }
"/*"							{ return LCOMMENT; }
"*/"							{ return RCOMMENT; }
.									{ /* do nothing */ }
%%
