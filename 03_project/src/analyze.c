#include "symtab.h"
#include <string.h>
#include <stdlib.h>

static int isFirstCompoundStmt = 0;
static int locationCounter = 0;
static int globalCounter = 0;

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

static void insertNode(TreeNode * t) {
  int argNums;
  int numParams;
  char * currScopeName;
  Bucket tempBucket;
  Bucket subBucket;
  switch (t->nodeKind) {
    //case IfStmt:
    case CompStmt:
      if(!isFirstCompoundStmt) {
        Scope scope = newScope(currScope()->name);
        scope->parent = currScope();
        pushScope(scope);
      }
      isFirstCompoundStmt = 0;
      break;
    //case IterStmt:
    case ReturnStmt:
      // Return type check
      currScopeName = currScope()->name;
      tempBucket = st_lookup("global", currScopeName);
      if(tempBucket == NULL) {
        printf("Unexpected error in line %d\n", t->lineno);
        exit(1);
      } else if(tempBucket->paramNum < 0) {
        printf("Unexpected return statement in line %d\n", t->lineno);
        exit(1);
      } else if(tempBucket->expectedType == Void && t->child[0] != NULL) {
        printf("Return error in line %d: function %s has void return type\n", t->lineno, tempBucket->name);
        exit(1);
      } else if(tempBucket->expectedType != Void && t->child[0] == NULL) {
        printf("Return error in line %d: function %s must have return value\n", t->lineno, tempBucket->name);
        exit(1);
      }  else if(tempBucket->expectedType != t->expectedType) {
        if(t->expectedType == VoidArr) {
          subBucket = st_lookup(currScope()->name, t->child[0]->attr.name);
          if(subBucket == NULL) {
            printf("Function call exception\n");
            exit(1);
          }
          if(subBucket->expectedType != tempBucket->expectedType) {
            printf("Return type error in line %d\n", t->lineno);
            printf("Expected type : %d\n", tempBucket->expectedType);
            printf("Actual type : %d\n", subBucket->expectedType);
          }
        } else {
          printf("Return type error in line %d\n", t->lineno);
          printf("Expected type : %d\n", tempBucket->expectedType);
          printf("Actual type : %d\n", subBucket->expectedType);
          exit(1);
        }
      }
      break;
    case Expr:
      if(t->child[1] != NULL && t->child[1]->expectedType == VoidArr) {
        tempBucket = st_lookup(currScope()->name, t->child[1]->attr.name);
        if(tempBucket == NULL) {
          printf("Undefined variable %s in line %d\n", t->child[1]->attr.name, t->lineno);
          exit(1);
        }
        t->child[1]->expectedType = tempBucket->expectedType;
      }
      if(t->child[0] != NULL && t->child[0]->expectedType == VoidArr) {
        tempBucket = st_lookup(currScope()->name, t->child[0]->attr.name);
        if(tempBucket == NULL) {
          printf("Undefined variable %s in line %d\n", t->child[0]->attr.name, t->lineno);
          exit(1);
        }
        t->child[0]->expectedType = tempBucket->expectedType;
      }

      if(t->child[0]->expectedType != t->child[1]->expectedType) {
        printf("Type assign error in line %d\n", t->lineno);
        printf("Expected type : %d\n", t->child[0]->expectedType);
        printf("Actual type : %d\n", t->child[1]->expectedType);
        printSymTab();
        exit(1);
      }
      break;
    //case SimExpr:
    //case AdditiveExpr:
    //case NumExpr:
    //case IdExpr:
    case VarDeclar:
      if(st_lookup_excluding_parent(currScope()->name, t->attr.name) != NULL) {
        printf("Redefinition of variable!\n");
        exit(1);
      }

      if(t->expectedType == Void || t->expectedType == VoidArr) {
        printf("Variable should not be void type.\n");
        exit(1);
      }

      if(strcmp(currScope()->name, "global")) {
        st_insert(currScope()->name, t->attr.name, 
            t->expectedType, t->lineno, locationCounter++, -1);
      } else {
        st_insert(currScope()->name, 
                  t->attr.name,
                  t->expectedType,
                  t->lineno,
                  globalCounter++,
                  -1);
      }
      break;
    case FunDeclar:
      locationCounter = 0;
      if(st_lookup(currScope()->name, t->attr.name) != NULL) {
        printf("Redefinition of function\n");
        exit(1);
      }
      numParams = 0;
      if((t->child[0]->expectedType != Void) && (t->child[0]->expectedType != VoidArr)) {
        TreeNode * temp = t->child[0];
        do {
          numParams++;
          temp = temp->sibling;
        } while(temp != NULL);
      }
      if(strcmp(currScope()->name, "global") == 0) {
        st_insert("global", t->attr.name, t->expectedType, t->lineno, globalCounter++, numParams);
      }
      Scope scope = newScope(t->attr.name);
      scope->parent = currScope();
      pushScope(scope);
      isFirstCompoundStmt = 1;
      break;
    case Param:
      if(t->expectedType != Void) {
        if(st_lookup_excluding_parent(currScope()->name, t->attr.name) != NULL) {
          printf("Redefinition of parameter\n");
          exit(1);
        }

      st_insert(currScope()->name, t->attr.name, t->expectedType, t->lineno, locationCounter++, -1);
      }
      break;
    case Var:
      //If undeclared var throw exception
      if(st_lookup(currScope()->name, t->attr.name) == NULL) {
        printf("current scope : %s\n", currScope()->name);
        printf("Undeclared variable %s at line %d\n", t->attr.name, t->lineno);
        printSymTab();
        exit(1);
      }
      break;
    case Func:
      argNums = 0;
      tempBucket = NULL;
      if((tempBucket = st_lookup(currScope()->name, t->attr.name)) == NULL) {
        printf("Function \"%s\" does not exist.\n", t->attr.name);
        exit(1);
      } else {
        TreeNode * temp = t->child[0];
        while(temp != NULL) {
          argNums++;
          temp = temp->sibling;
        }
      }

      if(argNums != tempBucket->paramNum) {
        printf("Wrong parameter to function \"%s\"", t->attr.name);
        exit(1);
      }

      //function call check argument number
      break;
    //case ExprStmt:
    default:
      ;
  }
}


void buildSymtab(TreeNode * syntaxTree) {
  if(syntaxTree == NULL) {
    printf("Invalid AST\n");
    return;
  }
  scopeStackTop = scopeArraySize = 0;
  Scope globalScope = newScope("global");
  pushScope(globalScope);
  st_insert("global", "input", Integer, 0, globalCounter++, 0);
  st_insert("global", "output", Void, 0, globalCounter++, 1);
  

  traverse(syntaxTree, insertNode, popAfterInsertProc);
  popScope();
  //printSymTab();
}
