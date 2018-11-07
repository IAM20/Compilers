#include "scan.h"
#include "cminus.tab.h"
#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Refactoring for input params : integer. */

static int indentno = 0;
#define INDENT    indentno+=2
#define UNINDENT  indentno-=2

int 
printToken(int token) {
  int line = yylineno;
  int tmp;
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
    while((tmp = getToken()) != RCOMMENT) {
      if(tmp == NEWLINE) line++;
      if(tmp == ENDOFFILE) {
        fprintf(stderr, "UNEXPECTED RETURN TOKEN NUMBER : %d\n", token);
        return 1;
      }
    }
    break;
  case NEWLINE:
    line++;
    break;
  case ENDOFFILE:
    printf("\tEOF\n");
    return 0;
    break;
  default:
    printf("UNEXPECTED RETURN TOKEN NUMBER : %d\n", token);
    return 1;
  }
  return 0;
}

TreeNode * newNode(NodeKind nodeKind) {
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
  int i;
  if(t == NULL) {
    fprintf(stderr, "Failed to memory allocation\n");
  } else {
    for(i = 0; i < MAXCHILDREN; i++) {
      t->child[i] = NULL;
    }
    
    t->sibling = NULL;
    t->nodeKind = nodeKind;
    t->lineno = yylineno;
    t->expectedType = Void;
  }

  return t;
}

char * copyString(char * s) {
  int n;
  char * retstr;
  if(s == NULL) {
    return NULL;
  }
  
  n = strlen(s) + 1;
  retstr = (char *)malloc(sizeof(char) * n);

  if(retstr == NULL) {
    fprintf(stderr, "Failed to memory allocation\n");
  } else strcpy(retstr, s);

  return retstr;
}

/* DFS  
void printTree(TreeNode * treeNode) {
  if (treeNode == NULL) {
    return;
  }
  
  switch(treeNode->nodeKind) {
    case IfStmt:
    case CompStmt:
    case ExprStmt:
    case IterStmt:
    case ReturnStmt:
    case Expr:
    case SimExpr:
    case AdditiveExpr:
    case OpExpr:
    case NumExpr:
    case IdExpr:
    case VarDeclar:
    case FunDeclar:
    case TypeSpecifier:
    case Param:
    case Var:
    case Func:
    case Args:
    default:
      return;
  }

  return;
}
*/

static void printSpaces(void) {
  int i;
  for(i = 0; i <indentno; i++) {
    printf(" ");
  }
}

void printType(ExpectedType type) {
  switch(type) {
    case Void:
      printf("type : void");
      break;
    case Integer:
      printf("type : int");
      break;
    case VoidArr:
      printf("type : void array");
      break;
    case IntArr:
      printf("type : int array");
      break;
    default:
      printf("type : unknown type");
      break;
  }
}

void printOperation(TokenType op) {
  switch(op) {
    case ASSIGN:
      printf("=");
      break;
    case EQ:
      printf("==");
      break;
    case NE:
      printf("!=");
      break;
    case LT:
      printf("<");
      break;
    case LE:
      printf("<=");
      break;
    case GT:
      printf(">");
      break;
    case GE:
      printf(">=");
      break;
    case PLUS:
      printf("+");
      break;
    case MINUS:
      printf("-");
      break;
    case TIMES:
      printf("*");
      break;
    case OVER:
      printf("/");
      break;
  }
}

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    switch(tree->nodeKind) {
      case IfStmt:
        printSpaces();
        if(tree->child[2] == NULL) {
          printf("If (condition) (body)\n");
        } else {
          printf("If (condition) (body) (else)\n");
        }
        break;
      case CompStmt:
        printSpaces();
        printf("Compound statement : \n");
        break;
      case ExprStmt:
        //printSpaces();
        //printf("Expr statement\n");
        break;
      case IterStmt:
        //printSpaces();
        //printf("Iter statement\n");
        break;
      case ReturnStmt:
        printSpaces();
        if(tree->child[0] != NULL) {
          printf("Return :\n");
        } else {
          printf("Return\n");
        }
        break;
      case Expr:
        printSpaces();
        printf("Assign : (destintion) (source)\n");
        break;
      case SimExpr:
        printSpaces();
        printf("Operation : ");
        printOperation(tree->attr.op);
        printf("\n");
        break;
      case AdditiveExpr:
        printSpaces();
        printf("Operation : ");
        printOperation(tree->attr.op);
        printf("\n");
        break;
      case OpExpr:
        //printSpaces();
        //printf("Operation\n");
        break;
      case NumExpr:
        printSpaces();
        printf("Const : %d\n", tree->attr.value);
        break;
      case IdExpr:
        printSpaces();
        printf("ID : %s\n", tree->attr.name);
        break;
      case VarDeclar:
        printSpaces();
        printf("Var declaration, name : %s, ", tree->attr.name);
        printType(tree->expectedType);
        
        if(tree->isArray) {
          printf(", array size : %d", tree->isArray);
        }
        printf("\n");
        break;
      case FunDeclar:
        printSpaces();
        printf("Function delcaration, name : %s, return ",
            tree->attr.name);
        printType(tree->expectedType);
        printf("\n");
        break;
      case TypeSpecifier:
        //printSpaces();
        //printf("Type specifier\n");
        break;
      case Param:
        printSpaces();
        printf("Single Parameter, ");
        printf("name : %s, ", tree->attr.name);
        printType(tree->expectedType);
        printf("\n");
        break;
      case Var:
        printSpaces();
        printf("Id : %s", tree->attr.name);
        if(tree->isArray) {
          printf(", at value below");
        } printf("\n");
        break;
      case Func:
        printSpaces();
        printf("Call, name : %s, with arguments below\n", tree->attr.name);
        break;
      case Args:
        //printSpaces();
        //printf("Argument\n");
        break;
      default:
        printSpaces();
        printf("Unknown node kind\n");
        break;
    }  
    for (i=0;i<MAXCHILDREN;i++) {
      if(tree->child[i] != NULL) {   
        printTree(tree->child[i]);
      }
    }
    tree = tree->sibling;
  }
  UNINDENT;
}
