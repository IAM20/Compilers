#include "scan.h"
#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

int 
printToken(FILE * fp) {
  int token;
  int line = 0;
  int tmp;
  while(token = yylex()) {
    switch(token) {
    case IF:
      printf("%d: reserved word: if\n", line);
      break;
    case ELSE:
      printf("%d: reserved word: else\n", line);
      break;
    case WHILE:
      printf("%d: reserved word: while\n", line);
      break;
    case RETURN:
      printf("%d: reserved word: return\n", line);
      break;
    case INT:
      printf("%d: reserved word: int\n", line);
      break;
    case VOID:
      printf("%d: reserved word: void\n", line);
      break;
    case ID:
      printf("%d: ID, name= %s\n", line, yytext);
      break;
    case NUM:
      printf("%d: NUM, value= %d\n", line, atoi(yytext));
      break;

    /* Case : special  Symbols! */
    case ASSIGN:
      printf("%d: =\n", line);
      break;
    case EQ:
      printf("%d: ==\n", line);
      break;
    case NE:
      printf("%d: !=\n", line);
      break;
    case LT:
      printf("%d: <\n", line);
      break;
    case LE:
      printf("%d: <=\n", line);
      break;
    case GT:
      printf("%d: >\n", line);
      break;
    case GE:
      printf("%d: >=\n", line);
      break;
    case PLUS:
      printf("%d: +\n", line);
      break;
    case MINUS:
      printf("%d: -\n", line);
      break;
    case TIMES:
      printf("%d: *\n", line);
      break;
    case OVER:
      printf("%d: /\n", line);
      break;
    case LPAREN:
      printf("%d: (\n", line);
      break;
    case RPAREN:
      printf("%d: )\n", line);
      break;
    case LSQUARE:
      printf("%d: [\n", line);
      break;
    case RSQUARE:
      printf("%d: ]\n", line);
      break;
    case LCURLY:
      printf("%d: {\n", line);
      break;
    case RCURLY:
      printf("%d: }\n", line);
      break;
    case SEMI:
      printf("%d: ;\n", line);
      break;
    case COMMA:
      printf("%d: ,\n", line);
      break;
    case LCOMMENT:
      while((tmp = yylex()) != RCOMMENT) {
        if(tmp == NEWLINE) line++;
      }
      break;
    case NEWLINE:
      line++;
      break;
    default:
      printf("UNEXPECTED RETURN TOKEN NUMBER : %d\n", token);
      return 1;
    }
  }
}
