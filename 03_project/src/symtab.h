#include "line.h"
#include "globals.h"

#define VAR -1

typedef struct bucket {
  char * name;
  ExpectedType expectedType;
  LineList lines;
  int memloc;
  int paramNum;
  struct bucket * next;
} * Bucket;

typedef struct scope {
  char * name;
  Bucket bucket[221];
  struct scope * parent;
} * Scope;

static Scope scopeStack[211];
static Scope scopeArray[211];
static int scopeStackTop;
static int scopeArraySize;

Scope currScope();
Scope newScope(char *);
void popScope();
void pushScope(Scope scope);
void insertScopeArr(Scope scope);
void stInsert(char *, char *, ExpectedType, int, int, int);
void printSymTab();
Bucket stLookup(char *, char *);
Bucket stLookupExcludingParent(char *, char *);
