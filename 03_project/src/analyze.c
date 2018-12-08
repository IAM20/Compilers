#include "symtab.h"
#include <string.h>
#include <stdlib.h>

#define TRUE      1
#define FALSE     0

typedef int bool;

static int isFirstCompoundStmt = 0;
static int locationCounter = 0;
static int globalCounter = 0;

static int
traverse (TreeNode * t, 
         int(* preProc)(TreeNode *), 
         void(* postProc)(TreeNode *)) {
  int retval = FALSE;
  if(t != NULL) {
    retval = (preProc(t)) ? TRUE : retval;
    for(int i = 0; i < MAXCHILDREN; i++) {
      retval = (traverse(t->child[i], preProc, postProc)) ? TRUE : retval; 
    }
    postProc(t);
    retval = (traverse(t->sibling, preProc, postProc)) ? TRUE : retval;
  }
  return retval;
}

static void 
popAfterInsertProc (TreeNode * t) {
  if(t == NULL) return;
  if(t->nodeKind == CompStmt) {
    popScope();
  }
}

ExpectedType
getTypeOfID (TreeNode * t) {
  Bucket bucket;
  bucket = stLookupWithScope(currScope(), t->attr.name);
  if(bucket == NULL) {
    printf("Line %d : identifier %s is not declared\n", t->lineno, t->attr.name);
    printf("Terminate the compilation, please check line number %d first\n", t->lineno);
    exit(1);
  }
  return bucket->expectedType;
}

bool 
checkTypeErrorAtReturnStmt(TreeNode * t) {
  char * currScopeName;
  Bucket tempBucket;
  
  currScopeName = currScope()->name;
  tempBucket = stLookup("global", currScopeName);

  if(tempBucket == NULL || tempBucket->paramNum < 0) {
    /* Something is wrong! This may not be happen */
    printf("Line %d : Unexpected return statement\n", t->lineno);
    return TRUE;
  } else if(tempBucket->expectedType == Void && t->child[0] != NULL) {
    printf("Line %d : function %s must not have return value\n",
            t->lineno,
            tempBucket->name);
    return TRUE;
  } else if(tempBucket->expectedType != Void && t->child[0] == NULL) {
    printf("Line %d: function %s must have return value.\n",
            t->lineno,
            tempBucket->name);
    return TRUE;
  } else if(tempBucket->expectedType != t->expectedType) {
    if(t->expectedType == VoidArr) {
      if(tempBucket->expectedType != getTypeOfID(t->child[0])) {
        printf("Line %d : Return type error.", t->lineno);
        return TRUE;
      }
    } else {
      printf("Line %d : Return type error", t->lineno);
      return TRUE;
    }
  }

  return FALSE;
}

bool 
checkTypeErrorAtExpr(TreeNode * t) {
  if(t->child[0] == NULL || t->child[1] == NULL) {
    return FALSE;
  }
  Bucket tempBucket;
  ExpectedType typeLeft = t->child[0]->expectedType;
  ExpectedType typeRight = t->child[1]->expectedType; 
  
  if(t->child[0] != NULL && typeLeft == VoidArr) {
    typeLeft = getTypeOfID(t->child[0]);
  }
  
  if(t->child[1] != NULL && typeRight== VoidArr) {
    typeRight = getTypeOfID(t->child[1]);
  }

  if(typeLeft != typeRight) {
    printf("Line %d : Type assign error in line", t->lineno);
    return TRUE;
  }

  return FALSE;
}

static int 
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
        return TRUE;
      }
      break;
    case Expr:
      if(checkTypeErrorAtExpr(t)) {
        /* There's an type error! */
        return TRUE;
      }
      break;
    case VarDeclar:
      if(stLookupExcludingParentWithScope(currScope(), t->attr.name) != NULL) {
        printf("Line %d : Redefinition of variable %s\n", t->lineno, t->attr.name);
        return TRUE;
      }

      if(t->expectedType == Void || t->expectedType == VoidArr) {
        printf("Line %d : Variable %s should not be void type.\n", t->lineno, t->attr.name);
        return TRUE;
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
      
      if(stLookupWithScope(currScope(), t->attr.name) != NULL) {
        printf("Line %d : Redefinition of function %s\n", t->lineno, t->attr.name);
        return TRUE;
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
      
      if(t->expectedType != Void || t->expectedType != VoidArr) {
        if(stLookupExcludingParentWithScope(currScope(), t->attr.name) != NULL) {
          printf("Line %d : Redefinition of parameter %s\n", t->lineno, t->attr.name);
          return TRUE;
        }

      stInsert( currScope()->name, 
                t->attr.name, 
                t->expectedType, 
                t->lineno, 
                locationCounter++, 
                VAR);
      } else {
        if(strcmp(t->attr.name, "(void)")) {
          printf("Line %d : wrong type of argument %s\n", t->lineno, t->attr.name);
          return TRUE;
        }
      }
      
      break;
    case Var:
      //If undeclared var throw exception
      if(stLookupWithScope(currScope(), t->attr.name) == NULL) {
        printf("Line %d : undeclared variable %s", t->lineno, t->attr.name);
        return TRUE;
      }
      if(t->child[0] != NULL) {
        if(t->child[0]->expectedType != Integer) {
          printf("Line %d : unexpected type in array\n", t->lineno);
          return TRUE;
        }
      }
      break;
    
    case Func:
      /* Function call semantic analysis, just check number of parameter.  */
      argNums = 0;
      tempBucket = NULL;

      if((tempBucket = stLookupWithScope(currScope(), t->attr.name)) == NULL) {
        printf("Line %d : Function \"%s\" does not exist.\n", t->lineno, t->attr.name);
        return TRUE;
      } else {
        TreeNode * temp = t->child[0];
        while(temp != NULL) {
          argNums++;
          temp = temp->sibling;
        }
      }

      if(argNums != tempBucket->paramNum) {
        printf("Line %d : Wrong parameter to function \"%s\"", t->lineno, t->attr.name);
        return TRUE;
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
  return FALSE;
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
  

  if(traverse(syntaxTree, insertNode, popAfterInsertProc)) {
    printf("There is semantic error!\n");
  }
  popScope();
}
