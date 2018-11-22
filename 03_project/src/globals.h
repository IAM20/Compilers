#include <stdio.h>
#define MAXRESERVED 12
#define MAXCHILDREN 5

typedef int TokenType;

typedef enum { 
  IfStmt, CompStmt, ExprStmt, IterStmt, ReturnStmt,
  Expr, SimExpr, AdditiveExpr, OpExpr, NumExpr, IdExpr,
  VarDeclar, FunDeclar,
  TypeSpecifier,
  Param,
  Var,
  Func,
  Args,
}NodeKind;

typedef enum { Void, Integer, VoidArr, IntArr } ExpectedType;

typedef struct treeNode {
  struct treeNode * child[MAXCHILDREN];
  struct treeNode * sibling;
  int lineno;
  NodeKind nodeKind;
  
  union {
    TokenType op;
    int value;
    char * name;
  } attr;
  
  int isArray;
  ExpectedType expectedType;
} TreeNode;

TreeNode * parse(void);
int yyparse(void);
extern FILE * source;
