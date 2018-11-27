#include "symtab.h"
#include "scan.h"
#include "cminus.tab.h"
#include "util.h"
#include "analyze.h"
#include <stdio.h>
#include <stdlib.h>

FILE * code;
TreeNode * AST;

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
  
  AST = parse();
  buildSymtab(AST);
  printSymTab();


  return 0;
}
