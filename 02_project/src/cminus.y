/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */
#define YYSTYPE TreeNode*
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "cminus.tab.h"

static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char * message);
%}

%token IF ELSE WHILE RETURN INT VOID
%token THEN END REPEAT UNTIL READ WRITE
%token ID NUM 
%token ASSIGN EQ NE LT LE GT GE PLUS MINUS TIMES OVER LPAREN RPAREN LSQUARE
%token RSQUARE LCURLY RCURLY SEMI COMMA LCOMMENT RCOMMENT NEWLINE ENDOFFILE
%token ERROR

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%% /* Grammar for CMINUS */

program           : declarationlist
                    { savedTree = $1;} 
                    ;
declarationlist   : declarationlist declaration
                    { TreeNode * t = $1;
                      if (t != NULL)
                      { while (t->sibling != NULL)
                          t = t->sibling;
                        t->sibling = $2;
                        $$ = $1; }
                      else $$ = $2;
                    }
                  | declaration  { $$ = $1; }
                  ;
declaration       : vardeclaration { $$ = $1; };
                  | fundeclaration { $$ = $1; };
vardeclaration   : typespecifier id SEMI
                    { 
                      $$ = newNode(VarDeclar);
                      $$->attr.name = $2->attr.name;
                      $$->expectedType = $1->expectedType;
                      $$->isArray = 0;
                    };
                  | typespecifier id LSQUARE num RSQUARE SEMI
                    { 
                      $$ = newNode(VarDeclar);
                      $$->expectedType = $1->expectedType;
                      $$->attr.name = $2->attr.name;
                      if($1->expectedType == Integer) {
                        $$->expectedType = IntArr;
                      } else {
                        $$->expectedType = VoidArr;
                      }
                      $$->isArray = $4->attr.value;
                    };
typespecifier     : INT { 
                      $$ = newNode(TypeSpecifier);
                      $$->expectedType = Integer;
                    };
                  | VOID {
                      $$ = newNode(TypeSpecifier);
                      $$->expectedType = Void; 
                    }
                  ;
fundeclaration    : typespecifier id LPAREN params RPAREN compoundstmt
                    {
                      $$ = newNode(FunDeclar);
                      $$->child[0] = $6;
                      $$->expectedType = $1->expectedType;
                      $$->attr.name = $2->attr.name;
                    }
                  ;
params            : paramlist { $$ = $1; };
                  | VOID {
                      $$ = newNode(Param);
                      $$->attr.name = "(void)";
                      $$->expectedType = Void;
                    }
                  ;
paramlist         : paramlist COMMA param {
                      TreeNode * t = $1;
                      if(t != NULL) {
                        while(t->sibling != NULL)
                          t = t->sibling;
                        t->sibling = $3;
                        $$ = $1;
                      } else {
                        $$ = $3;
                      }
                    };
                  | param {
                      $$ = $1;
                    }
                  ;
param             : typespecifier id {
                      $$ = newNode(Param);
                      $$->attr.name = $2->attr.name;
                      $$->expectedType = $1->expectedType;
                      $$->isArray = 0;
                    };
                  | typespecifier id LSQUARE RSQUARE {
                      $$ = newNode(Param);
                      $$->attr.name = $2->attr.name;
                      $$->isArray = 1;
                      $$->expectedType = ($1->expectedType == Integer) ? IntArr : VoidArr;
                    }
                  ;
id                : ID {
                      $$ = newNode(IdExpr);
                      $$->attr.name = copyString(yytext);
                    };
num               : NUM {
                      $$ = newNode(NumExpr);
                      $$->attr.value = atoi(yytext);
                    };
compoundstmt      : LCURLY localdeclaration statementlist RCURLY {
                      $$ = newNode(CompStmt);
                      $$->child[0] = $2;
                      $$->child[1] = $3;
                    }
                  ;
localdeclaration  : localdeclaration vardeclaration {
                      TreeNode * t = $1;
                      if(t != NULL) {
                        while(t->sibling != NULL)
                          t = t->sibling;
                        t->sibling = $2;
                        $$ = $1;
                      } else {
                        $$ = $2;
                      }
                    };
                  | /* empty */ {
                      /* epsilon */
                      $$ = NULL;
                    }
                  ;
statementlist     : statementlist statement {
                      TreeNode * t = $1;
                      if(t != NULL) {
                        while(t->sibling != NULL)
                          t = t->sibling;
                        t->sibling = $2;
                        $$ = $1;
                      } else {
                        $$ = $2;
                      }
                    }; 
                  | /* empty */ { 
                      /* epsilon */ 
                      $$ = NULL;
                    }
                  ;
statement         : expressionstmt { $$ = $1; };
                  | compoundstmt { $$ = $1; };
                  | selectionstmt { $$ = $1; };
                  | iterationstmt { $$ = $1; };
                  | returnstmt { $$ = $1; }
                  ;
expressionstmt    : expression SEMI {
                      $$ = newNode(ExprStmt);
                      $$->child[0] = $1;
                    };
                  | SEMI { $$ = newNode(ExprStmt); }
                  ;

selectionstmt     : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE { 
                      $$ = newNode(IfStmt);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                    };
                  | IF LPAREN expression RPAREN statement ELSE statement { 
                      $$ = newNode(IfStmt);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                      $$->child[2] = $7;
                    }
                  ;

iterationstmt     : WHILE LPAREN expression RPAREN statement { 
                      $$ = newNode(IterStmt);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                    }
                  ;

returnstmt        : RETURN SEMI { 
                      $$ = newNode(ReturnStmt);
                    };
                  | RETURN expression SEMI { 
                      $$ = newNode(ReturnStmt);
                      $$->child[0] = $2;
                    }
                  ;

expression        : var ASSIGN expression { 
                      $$ = newNode(Expr);
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                      /* We can check types here. */
                    };
                  | simpleexpression { 
                      $$ = $1;
                    }
                  ;

var               : id { 
                      $$ = newNode(Var);
                      $$->attr.name = $1->attr.name;
                      $$->isArray = 0;
                    };
                  | id LSQUARE expression RSQUARE {
                      $$ = newNode(Var);
                      $$->attr.name = $1->attr.name;
                      $$->child[0] = $3;
                      $$->isArray = 1;
                    }
                  ;

simpleexpression  : additiveexpr relop additiveexpr { 
                      $$ = newNode(SimExpr);
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                      $$->attr.op = $2->attr.op;
                    };
                  | additiveexpr { $$ = $1; };

relop             : LE { 
                      $$ = newNode(OpExpr); 
                      $$->attr.op = LE;
                    };
                  | LT {
                      $$ = newNode(OpExpr);
                      $$->attr.op = LT;
                    };
                  | GT {
                      $$ = newNode(OpExpr);
                      $$->attr.op = GT;
                    };
                  | GE { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = GE;
                    };
                  | EQ { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = EQ;
                    };
                  | NE { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = NE;
                    };

additiveexpr      : additiveexpr addop term { 
                      $$ = newNode(AdditiveExpr);
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                      $$->attr.op = $2->attr.op;
                    };
                  | term { $$ = $1; };

addop             : PLUS { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = PLUS;
                    };
                  | MINUS { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = MINUS;
                    };

term              : term mulop factor { 
                      $$ = newNode(AdditiveExpr);
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                      $$->attr.op = $2->attr.op;
                    };
                  | factor { 
                      $$ = $1;
                    };

mulop             : TIMES { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = TIMES;
                    };
                  | OVER { 
                      $$ = newNode(OpExpr);
                      $$->attr.op = OVER;
                    };

factor            : LPAREN expression RPAREN { 
                      $$ = newNode(Expr);
                      $$->child[0] = $2;
                    };
                  | var { $$ = $1; };
                  | call { $$ = $1; };
                  | num {
                      $$ = newNode(NumExpr);
                      $$->attr.value = $1->attr.value;
                    };

call              : id LPAREN args RPAREN {
                      $$ = newNode(Func);
                      $$->child[0] = $3;
                      $$->attr.name = $1->attr.name;
                    };

args              : arglist { 
                      $$ = $1;
                    };
                  | /* empty */ { 
                      $$ = NULL;
                    };

arglist           : arglist COMMA expression { 
                      TreeNode * t = $1;
                      if(t != NULL) {
                        while(t->sibling != NULL)
                          t = t->sibling;
                        t->sibling = $3;
                        $$ = $1;
                      } else {
                        $$ = $3;
                      } 
                    };
                  | expression { $$ = $1; };

%%

static int yylex(void) {
  return getToken();
}

int yyerror(char * message)
{ fprintf(stderr, "Syntax error at line %d: %s\n", yylineno,message);
  return 0;
}

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

