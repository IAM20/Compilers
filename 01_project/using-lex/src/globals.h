#include <stdio.h>
#define MAXRESERVED 12

typedef enum {
  /* Reserved words */
  IF = 1, ELSE, WHILE, RETURN, INT, VOID,
  
  /* Discarded */
  THEN, END, REPEAT, UNTIL, READ, WRITE,
  /*************/

  /* Multicharacter inputs */
  ID, NUM,
  
  /* Special symbols */
  ASSIGN,   // =
  EQ,       // ==
  NE,       // !=
  LT,       // <
  LE,       // <=
  GT,       // >
  GE,       // >=
  PLUS,     // +
  MINUS,    // -
  TIMES,    // *
  OVER,     // '/'
  LPAREN,   // '('
  RPAREN,   // ')'
  LSQUARE,  // '[' 
  RSQUARE,  // ']'
  LCURLY,   // '{'
  RCURLY,   // '}'
  SEMI,     // ;
  COMMA,    // ,
  LCOMMENT, // /*
  RCOMMENT, // */
  NEWLINE,  // \n
  ENDOFFILE // eof   
} TokenType;

extern FILE * source;
