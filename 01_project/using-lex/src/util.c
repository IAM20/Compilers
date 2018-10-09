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
  while((token = yylex()) != 0) {
    switch(token) {
    case IF:
      printf("\t%d reserved word: if\n", line);
      break;
    case ELSE:
      printf("\t%d reserved word: else\n", line);
      break;
    case WHILE:
      printf("\t%d reserved word: while\n", line);
      break;
    case RETURN:
      printf("\t%d reserved word: return\n", line);
      break;
    case INT:
      printf("\t%d reserved word: int\n", line);
      break;
    case VOID:
      printf("\t%d reserved word: void\n", line);
      break;
    case ID:
      printf("\t%d ID, name= %s\n", line, yytext);
      break;
    case NUM:
      printf("\t%d NUM, value= %d\n", line, atoi(yytext));
      break;

    /* Case : special  Symbols! */
    case ASSIGN:
      printf("\t%d =\n", line);
      break;
    case EQ:
      printf("\t%d ==\n", line);
      break;
    case NE:
      printf("\t%d !=\n", line);
      break;
    case LT:
      printf("\t%d <\n", line);
      break;
    case LE:
      printf("\t%d <=\n", line);
      break;
    case GT:
      printf("\t%d >\n", line);
      break;
    case GE:
      printf("\t%d >=\n", line);
      break;
    case PLUS:
      printf("\t%d +\n", line);
      break;
    case MINUS:
      printf("\t%d -\n", line);
      break;
    case TIMES:
      printf("\t%d *\n", line);
      break;
    case OVER:
      printf("\t%d /\n", line);
      break;
    case LPAREN:
      printf("\t%d (\n", line);
      break;
    case RPAREN:
      printf("\t%d )\n", line);
      break;
    case LSQUARE:
      printf("\t%d [\n", line);
      break;
    case RSQUARE:
      printf("\t%d ]\n", line);
      break;
    case LCURLY:
      printf("\t%d {\n", line);
      break;
    case RCURLY:
      printf("\t%d }\n", line);
      break;
    case SEMI:
      printf("\t%d ;\n", line);
      break;
    case COMMA:
      printf("\t%d ,\n", line);
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
  return 0;
}
