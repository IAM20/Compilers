#include "globals.h"
#include "symtab.h"
#include "line.h"
#include <stdlib.h>

static void traverse(TreeNode * t, 
    void(* preProc)(TreeNode *), 
    void(* postProc)(TreeNode *)) {
  if(t != NULL) {
    preProc(t);
    for(int i = 0; i < MAXCHILDREN; i++) {
      traverse(t->child[i], preProc, postProc);
    }
    postProc(t);
    traverse(t->sibling, preProc, postProc);
  }
}

static void popAfterInsertProc(TreeNode * t) {
  if(t == NULL) return;
  if(t->nodeKind == CompStmt) {
    popScope();
  }
}

static int isFirstCompoundStmt = 0;
static int locationCounter = 0;
static void insertNode(TreeNode * t) {
  int argNums;
  int numParams;
  switch (t->nodeKind) {
    //case IfStmt:
    case CompStmt:
      if(!isFirstCompoundStmt) {
        Scope scope = newScope(scopeStack[scopeStackTop - 1]->name);
        scope->parent = scopeStack[scopeStackTop - 1]->name;
        pushScope(scope);
      }
      isFirstCompoundK = false;
      break;
    //case IterStmt:
    //case ReturnStmt:
    case Expr:
      if(t->child[0]->expectedType != t->child[1]->expectedType) {
        printf("Type assign error");
        exit(1);
      }
    //case SimExpr:
    //case AdditiveExpr:
    //case NumExpr:
    //case IdExpr:
    //case VarDeclar:
      if(st_lookup_excluding_parent(scopeStack[scopeStackTop - 1]->name, t->attr.name) != NULL) {
        printf("Redefinition of variable!\n");
        exit(1);
      }

      if(t->expectedType == Void || t->expectedType == VoidArr) {
        printf("Variable should not be void type.\n");
        exit(1);
      }
      st_insert(scopeStack[scopeStackTop - 1]->name, t->attr.name, 
            t->expectedType, t->lineno, locationCouner++, -1);
      break;
    case FunDeclar:
      locationCounter = 1;
      if(st_lookup(scopeStack[scopeStackTop - 1]->name, t->attr.name) != NULL) {
        printf("Redefinition of function\n");
        exit(1);
      }
      numParams = 0;
      if(t->child[0]->expectedType != Void || t->child[0]->expectedType != VoidArr)
      else {
        TreeNode * temp = temp->child[0];
        do {
          numParams++;
          temp = temp->sibling;
        } while(temp != NULL)

      if(strcmp(scopeStack[scopeStackTop - 1]->name, "global") == 0) {
        st_insert("global", t->attr.name, t->expectedType, lineno, locationCounter++, numParams);
      }
      Scope scope = newScope(t->attr.name);
      scope->parent = scopeStack[scopeStackTop - 1];
      pushScope(scope);
      isFirstCompoundK = true;
      break;
    case Param:
      if(t->expectedType != Void) {
        if(st_lookup_excluding_parent(scopeStack[scopeStackTop - 1]->name, t->attr.name) != NULL) {
          printf("Redefinition of parameter\n");
          exit(1);
        }
      }

      st_insert(scopeStack[scopeStackTop - 1]->name, t->attr.name, t->expectedType, t->lineno, 0, -1);
      break;
    case Var:
      //If undeclared var throw exception
      if(st_lookup(scopeStack[scopeStackTop - 1]->name, t->attr.name) == NULL) {
        printf("Undeclared variable\n");
        exit(1);
      }
      break;
    case Func:
      argNums = 0;
      Bucket tempBucket;
      if((tmpBucket = st_lookup(scopeStack[scopeStackTop - 1]->name, t->attr.name)) == NULL) {
        printf("Function \"%s\" does not exist.\n", t->attr.name);
        exit(1);
      } else {
        TreeNode * temp = temp->child[0];
        do {
          argNums++;
          temp = temp->sibling;
        } while(temp != NULL)
      }

      if(argNums != tmpBucket->numParams) {
        printf("Wrong parameter to function \"%s\"", t->attr.name);
        exit(1);
      }

      //function call check argument number
      break;
    //case ExprStmt:
    default:
  }
}


void buildSymtab(TreeNode * syntaxTree) {
  Scope globalScope = newScope("global");
  pushScope(globalScope);
  st_insert("global", "input", Integer, 0, 1, 0);
  st_insert("global", "output", Void, 0, 0, 1);

  traverse(syntaxTree, insertNode, popAfterInsertProc);
  popScope();
  //printSymTab();
}
