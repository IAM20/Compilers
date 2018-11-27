#include <stdio.h>
#include <stdlib.h>
#include "line.h"

Lnode * 
initLnode(unsigned int line) {
  
  Lnode * tmp = (Lnode *)malloc(sizeof(struct lnode));
  tmp->line = line;
  tmp->next = NULL;

  return tmp;
}

LineList 
initLineList() {
  
  LineList tmp = (LineList)malloc(sizeof(struct lineList));
  if(tmp == NULL) {
    exit(1);
  }
  tmp->size = 0;
  tmp->head = tmp->tail = NULL;

  return tmp;
}

int 
insertLine(LineList lines, unsigned int line) {
  
  Lnode * tmp = initLnode(line);
  switch(lines->size) {
    case 0:
      lines->head = lines->tail = tmp;
      break;
    
    default:
      lines->tail->next = tmp;
      tmp->prev = lines->tail;
      lines->tail = tmp;
      break;
  }
  
  lines->size++;
  return 0;
}

int 
deleteLineListTail(LineList lines) {
  
  if(lines->size == 0) {
    return 0;
  }

  Lnode * tmp = lines->tail;
  lines->tail = lines->tail->prev;
  
  free(tmp);
  tmp = NULL;

  return --lines->size;
}

void 
deleteLineList(LineList lines) {
  
  while(deleteLineListTail(lines));
  free(lines);
  lines = NULL;

}

