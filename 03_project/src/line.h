typedef struct lnode {
  unsigned int line;
  struct lnode * next;
  struct lnode * prev;
} Lnode;

typedef struct lineList {
  unsigned int size;
  Lnode * head;
  Lnode * tail; // never deleted
} * LineList


Lnode * initLnode(unsigned int);
LineList initLineList();
int insertLine(LineList, unsigned int);
/*
|-------------|--------------|
| Circustance | return value |
|-------------|--------------|
|    Error    |      -1      |
|-------------|--------------|
|    Normal   |       0      |
|-------------|--------------|
*/

int deleteLineListTail(LineList);
void deleteLineList(LineList);
