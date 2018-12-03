#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 4
#define SIZE 211

static int 
hash (char * key) {
  int temp = 0;
  int i = 0;
  while(key[i] != '\0') {
    temp = ((temp << SHIFT) + key[i++]) % SIZE;
  } return temp;
}

Scope 
currScope() {
  return scopeStack[scopeStackTop - 1];
}

Scope 
newScope(char * scopeName) {
  Scope tmp = (Scope)malloc(sizeof(struct scope));
  if(tmp == NULL) {
    perror("Failed to memory allocation");
    exit(1);
  }

  tmp->name = scopeName;
  tmp->parent = NULL;

  return tmp;
}

void 
popScope() {
  scopeStack[--scopeStackTop] = NULL;
}

void 
pushScope(Scope scope) {
  scopeStack[scopeStackTop++] = scope;
  insertScopeArr(scope);
}

void 
insertScopeArr(Scope scope) {
  scopeArray[scopeArraySize++] = scope;
}

void 
stInsert (char * scopeName, 
          char * name,
          ExpectedType type, 
          int lineno, 
          int loc, 
          int paramNum) {
  
  Scope currScope = scopeStack[scopeStackTop - 1];
  int index = hash(name);

  Bucket l = currScope->bucket[index];

  while((l != NULL) && strcmp(name, l->name))
    l = l->next;

  if(l == NULL) {
    l = (Bucket)malloc(sizeof(struct bucket));
    l->name = name;
    l->lines = initLineList();
    insertLine(l->lines, lineno);
    l->memloc = loc;
    l->expectedType = type;
    l->next = currScope->bucket[index];
    l->paramNum = paramNum;
    currScope->bucket[index] = l;
  } else {
    insertLine(l->lines, lineno);
  }

}

Bucket
stLookupWithScope(Scope scope, char * name) {
  if(scope == NULL) {
    return NULL;
  }
  
  Bucket bucket = NULL;
  int index = hash(name);
  
  while(scope != NULL) {
    bucket = scope->bucket[index];
    while((bucket != NULL) && strcmp(name, bucket->name)) {
      bucket = bucket->next;
    }
    if(bucket != NULL) {
      return bucket;
    }
    scope = scope->parent;
  }

  return NULL;
}

Bucket 
stLookup (char * scopeName, char * name) {
  
  Scope scope = NULL;
  Bucket bucket = NULL;
  int i, index;
  
  for(i = 0; i < scopeArraySize; i++) {
    if(strcmp(scopeArray[i]->name, scopeName) == 0) {
      scope = scopeArray[i];
      break;
    }
  }
  
  if(scope == NULL) {
    return NULL;
  }
  return stLookupWithScope(scope, name);
}

Bucket
stLookupExcludingParentWithScope(Scope scope, char * name) {
  if(scope == NULL) {
    return NULL;
  }
  Bucket bucket = NULL;
  int index = hash(name);
  
  bucket = scope->bucket[index];
  while((bucket != NULL) && strcmp(name, bucket->name)) {
    bucket = bucket->next;
  }

  return bucket;
}
Bucket 
stLookupExcludingParent(char * scopeName, char * name) {
  Scope scope = NULL;
  Bucket bucket = NULL;
  int i, index;
  
  for(i = 0; i < scopeArraySize; i++) {
    if(strcmp(scopeArray[i]->name, scopeName) == 0) {
      scope = scopeArray[i];
      break;
    }
  }
  
  return stLookupExcludingParentWithScope(scope, name);
}

void 
printSymTabRows(Scope scope) {
  Bucket * bucket = scope->bucket;
  for(int i = 0; i < 221; i++) {
    if(bucket[i] != NULL) {
      Bucket b = bucket[i];
      Lnode * lines = b->lines->head;
      printf("%-9s ", b->name);
      if(b->paramNum < 0) {
        printf("Variable     ");
      } else {
        printf("Function     ");
      }
      switch(b->expectedType) {
        case Void:
          printf("Void         "); 
          break;
        case Integer: 
          printf("Integer      ");
          break;
        case IntArr:
          printf("Integer Arr  ");
          break;
        default :
          ;
      }
      printf("%-14d", b->memloc);
      do {
        printf("%d ", lines->line);
        lines = lines->next;
      } while(lines != NULL);
      printf("\n");
    }
  }
}

void 
printSymTab() {
  printf("\n------------------\n");
  printf(  "|  Symbol table  |\n");
  printf(  "------------------\n\n");
  
  for(int i = 0; i < scopeArraySize; i++) {
    Scope scope = scopeArray[i];
    printf("Scope Name : %s\n", scope->name);
    printf("-----------------------------------------------------------------------\n");
    printf("Name      Type        Data Type     Location      Line Numbers \n");
    printf("--------- ----------- ------------- ------------- ---------------------\n");
    printSymTabRows(scope);
    printf("-----------------------------------------------------------------------\n\n");
  }
}
