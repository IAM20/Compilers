#include "symtab.h"
#include <string.h>
#include <stdlib.h>

#define TRUE      1
#define FALSE     0

typedef int bool;

static int isFirstCompoundStmt = 0;
static int locationCounter = 0;
static int globalCounter = 0;

static void 
traverse (TreeNode * t, 
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

static void 
popAfterInsertProc (TreeNode * t) {
  if(t == NULL) return;
  if(t->nodeKind == CompStmt) {
    popScope();
  }
}

ExpectedType 
getTypeOfID (char * scopeName, char * name) {
  Bucket bucket;
  bucket = stLookup(scopeName, name);
  if(bucket == NULL) {
    printf("ID : %s is not declared\n", name);
    exit(1);
  }
  return bucket->expectedType;
}

// TODO
bool 
checkTypeErrorAtReturnStmt(TreeNode * t) {
  char * currScopeName;
  Bucket tempBucket;
  
  currScopeName = currScope()->name;
  tempBucket = stLookup("global", currScopeName);

  if(tempBucket == NULL || tempBucket->paramNum < 0) {
    /* Something is wrong! This may not be happen */
    printf("Unexpected return statement in line %d\n", t->lineno);
    return TRUE;
  } else if(tempBucket->expectedType == Void && t->child[0] != NULL) {
    printf("Return error in line %d: function %s must not have return value\n",
            t->lineno,
            tempBucket->name);
    return TRUE;
  } else if(tempBucket->expectedType != Void && t->child[0] == NULL) {
    printf("Return error in line %d: function %s must have return value.\n",
            t->lineno,
            tempBucket->name);
    return TRUE;
  } else if(tempBucket->expectedType != t->expectedType) {
    if(t->expectedType == VoidArr) {
      if(tempBucket->expectedType != getTypeOfID(currScope()->name, t->child[0]->attr.name)) {
        printf("Return type error in line %d\n", t->lineno);
        return TRUE;
      }
    } else {
      printf("Return type error in line %d\n", t->lineno);
      return TRUE;
    }
  }


  return FALSE;
}

// TODO
bool 
checkTypeErrorAtExpr(TreeNode * t) {
  Bucket tempBucket;
  ExpectedType typeLeft = t->child[0]->expectedType;
  ExpectedType typeRight = t->child[1]->expectedType; 
  
  if(t->child[0] != NULL && typeLeft == VoidArr) {
    typeLeft = getTypeOfID(currScope()->name, t->child[0]->attr.name);
  }
  
  if(t->child[1] != NULL && typeRight== VoidArr) {
    typeRight = getTypeOfID(currScope()->name, t->child[1]->attr.name);
  }

  if(typeLeft != typeRight) {
    printf("Type assign error in line %d\n", t->lineno);
    return TRUE;
  }

  return FALSE;
}

static void 
insertNode(TreeNode * t) {
  int argNums;
  int numParams;
  char * currScopeName;
  Bucket tempBucket;
  
  switch (t->nodeKind) {
    case CompStmt:
      if(!isFirstCompoundStmt) {
        Scope scope = newScope(currScope()->name);
        scope->parent = currScope();
        pushScope(scope);
      }
      isFirstCompoundStmt = 0;
      break;
    case ReturnStmt:
      if(checkTypeErrorAtReturnStmt(t)) {
        /* There's an type error! */
        exit(1);
      }
      break;
    case Expr:
      if(checkTypeErrorAtExpr(t)) {
        /* There's an type error! */
        exit(1);
      }
      break;
    case VarDeclar:
      if(stLookupExcludingParent(currScope()->name, t->attr.name) != NULL) {
        printf("Redefinition of variable!\n");
        exit(1);
      }

      if(t->expectedType == Void || t->expectedType == VoidArr) {
        printf("Variable should not be void type.\n");
        exit(1);
      }

      if(strcmp(currScope()->name, "global")) {
        stInsert( currScope()->name, 
                  t->attr.name, 
                  t->expectedType, 
                  t->lineno, 
                  locationCounter++, 
                  VAR);
      } else {
        stInsert( currScope()->name, 
                  t->attr.name,
                  t->expectedType,
                  t->lineno,
                  globalCounter++,
                  VAR);
      }
      
      break;

    case FunDeclar:
      locationCounter = 0;
      numParams = 0;
      
      if(stLookup(currScope()->name, t->attr.name) != NULL) {
        printf("Redefinition of function\n");
        exit(1);
      }
      
      if((t->child[0]->expectedType != Void) && (t->child[0]->expectedType != VoidArr)) {
        TreeNode * temp = t->child[0];
        do {
          numParams++;
          temp = temp->sibling;
        } while(temp != NULL);
      }

      if(strcmp(currScope()->name, "global") == 0) {
        stInsert( "global", 
                  t->attr.name, 
                  t->expectedType, 
                  t->lineno, 
                  globalCounter++, 
                  numParams);
      }
      
      Scope scope = newScope(t->attr.name);
      scope->parent = currScope();
      pushScope(scope);
      isFirstCompoundStmt = 1;
      
      break;
    case Param:
      
      if(t->expectedType != Void) {
        if(stLookupExcludingParent(currScope()->name, t->attr.name) != NULL) {
          printf("Redefinition of parameter\n");
          exit(1);
        }

      stInsert( currScope()->name, 
                t->attr.name, 
                t->expectedType, 
                t->lineno, 
                locationCounter++, 
                VAR);
      }
      break;
    case Var:
      //If undeclared var throw exception
      if(stLookup(currScope()->name, t->attr.name) == NULL) {
        printf("current scope : %s\n", currScope()->name);
        printf("Undeclared variable %s at line %d\n", t->attr.name, t->lineno);
        printSymTab();
        exit(1);
      }
      
      break;
    
    case Func:
      /* Function call semantic analysis, just check number of parameter.  */
      argNums = 0;
      tempBucket = NULL;

      if((tempBucket = stLookup(currScope()->name, t->attr.name)) == NULL) {
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

 /* 
  * case IterStmt:
  * case IfStmt:
  *   This two are checked by parser.
  *
  * case ExprStmt:
  * case SimExpr:
  * case AdditiveExpr:
  * case NumExpr:
  * case IdExpr:
  *   Useless cases
  */
    default:
      ;
  }
}


void 
semanticAnalysis (TreeNode * syntaxTree) {
  if(syntaxTree == NULL) {
    printf("Invalid AST\n");
    return;
  }
  scopeStackTop = scopeArraySize = 0;
  Scope globalScope = newScope("global");
  pushScope(globalScope);
  stInsert("global", "input", Integer, 0, globalCounter++, 0);
  stInsert("global", "output", Void, 0, globalCounter++, 1);
  

  traverse(syntaxTree, insertNode, popAfterInsertProc);
  popScope();
}
