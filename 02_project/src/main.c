#include "scan.h"
#include "cminus.tab.h"
#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

FILE * code;

int
main(int argc, char * argv[]) {
  if(argc != 2) {
    fprintf(stderr, "USAGE : %s FILE_NAME\n", argv[0]);
    return 0;
  }

  if((code = fopen(argv[1], "r")) == NULL) {
    perror("Failed to open file");
    return 1;
  }
  
  yyin = code;
  printf("C-MINUS COMPILATION: %s\n", argv[1]);
  
  int token;
  printTree(parse());

  return 0;
}
