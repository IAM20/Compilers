#include "analyze.h"
#include "line.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 4
#define SIZE 211

static int hash (char * key) {
  int temp = 0;
  int i = 0;
  while(key[i] != '\0') {
    temp = ((temp << SHIFT) + key[i++]) % SIZE;
  } return temp;
}

Scope newScope(char * scopeName) {
  Scope tmp = (Scope)malloc(sizeof(struct scope));
  if(tmp == NULL) {
    perror("Failed to memory allocation");
    exit(1);
  }

  tmp->name = scopeName;
  scope->parent = NULL;

  return tmp;
}

void popScope() {
  scopeStack[--scopeStackTop] = NULL;
}

void pushScope(Scope scope) {
  scopeStack[scopeStackTop++] = scope;
  insertScopeArr(scope);
}

void insertScopeArr(Scope scope) {
  scopeArray[scopeArraySize++] = scope;
}

void st_insert(char * scopeName, char * name, ExpectedType type, int lineno, int loc, int paramNum) {
  //If undeclared var throw exception
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
    l->next = hashTable[h];
    l->paramNum = paramNum;
    hashTable[h] = l;
  } else {
    insertLine(l->lines, lineno);
  }
}

Bucket st_lookup(char * name, char * scopeName) {
  Scope scope = NULL;
  Bucket bucket = NULL;
  int i, index;
  for(i = 0; i < scopeArraySize; i++) {
    if(strcmp(scopeArray[i]->name, scopeName) == 0) {
      scope = scopeArray[i];
      break;
    }
  }
  if(scope == NULL) return NULL;
  index = hash(name);

  while(scope != NULL) {
    bucket = scope->bucket[index];
    while ((bucket != NULL) && strcmp(name, bucket->name)) {
      bucket = bucket->next;
    }
    if(bucket != NULL) {
      return bucket;
    }
    scope = scope->parent;
  }
  return NULL;
}

Bucket st_lookup_excluding_parent(char * scope, char * name) {
  Scope scope = NULL;
  Bucket bucket = NULL;
  int i, index;
  for(i = 0; i < scopeArraySize; i++) {
    if(strcmp(scopeArray[i]->name, scopeName) == 0) {
      scope = scopeArray[i];
      break;
    }
  }
  if(scope == NULL) return NULL;
  index = hash(name);

  bucket = scope->bucket[index];
  while ((bucket != NULL) && strcmp(name, bucket->name)) {
    bucket = bucket->next;
  }
  return bucket;
}


