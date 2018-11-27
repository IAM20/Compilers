#include "globals.h"
#include "line.h"

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
  Bucket bucket[SIZE];
  struct scope * parent;
} * Scope;

static Scope scopeStack[211];
static Scope scopeArray[211];
static int scopeStackTop = 0;
static int scopeArraySize = 0;

Scope newScope(char *);
void popScope();
void pushScope(Scope scope);
void insertScopeArr(Scope scope);
void st_insert(char *, char *, ExpectedType, int, int);
Bucket st_lookup(char *, char *);
Bucket st_lookup_excluding_parent(char *, char *);
