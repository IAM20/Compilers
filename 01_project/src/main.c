#include "scan.h"
#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE        1
#define FALSE       0

#define NO_PARSE    TRUE
#define NO_ANALYZE  TRUE

/* allocate and set tracing flag. */
const int EchoSource    = TRUE;
const int TraceScan     = TRUE;

const int TraceParse    = FALSE;
const int TraceAnalyze  = FALSE;

const int TraceCode     = FALSE;

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

  if(printToken(yyin)) {
    fprintf(stderr, "Failed to scan code.\n");
    return 1;
  }

  return 0;
}
