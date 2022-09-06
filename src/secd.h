#ifndef SECD_H
#define SECD_H
#include <time.h>


// instruction set of the sesecd
typedef enum instruction{
    NIL = 1,
    LDC,
    LD,
    ATOM,
    CAR,
    CDR,
    CONS,
    ADD,
    SUB,
    EQ,
    LEQ,
    LE,
    GEQ,
    GE,
    MUL,
    DIV,
    AND,
    OR,
    SEL,
    JOIN,
    LDF,
    AP,
    RTN,
    DUM,
    RAP,
    STOP,

    SPECPAR,
    MAP,
    /*FUNCTION,
    CONSTANT*/
} instruction;

typedef enum typeInfo{
  FUNCTION,
  CONSTANT,
  LIST
} typeInfo;


// Nested list that can contain tree structured data. car is the head and cdr the tail. 
// (contents of the address part of the register, contents of the decrement part of the register)
// value wil normally be interpreted as a number, but after instructions that produce a true or false value, 0 will be false and ever other integer true.
typedef struct sexpr {

  typeInfo type;
  
  struct sexpr *cdr;
  union car {
    struct sexpr *list;
    int value;
    enum instruction instruction;
  } car;
} sexpr;

// The 4 registers can take arbitrary s-expressions. 
typedef struct sesecd {
  
  struct sexpr *s;
  struct sexpr *e;
  struct sexpr *c;
  struct sexpr *d;
} sesecd;

// execute the programm. should read from the c s-expression and manipulate the secd state accordingly. 
void execute();
// constructs memory objects that can hold integer values or pointers to s-expr. When assiging integer value to cdr it will still be a pointer to an s-expr
// containing an integer value in its car. L=List, I= Integer. 
struct sexpr *consLL(struct sexpr *car, struct sexpr *cdr);
struct sexpr *consIL(int car, struct sexpr *cdr);
struct sexpr *consLI(struct sexpr *car, int cdr);
struct sexpr *consII(int car, int cdr);


//this sets the maximum space that exists for a page in the garbage collector
#define MAX_PAGE_SIZE 1000000

//the number of times the garbage collector ran
int gcConducted;

//the time when the vm started running
clock_t start_time;

//this is global in order to make garbage collection easier
sesecd *secd;

/*
*   The following methods are here to make the creation of s-expressions in secd syntax easier
*   Generally it will return a pointer to the current cdr, to make appending the list easier. The idea is to build an s-expression from the AST-Tree
*/

// creates a new node in CDR and puts the cadr s-expression into the car of cdr. might be useful for loading functions, returns pointer to new CDR
struct sexpr *addCDRList(struct sexpr *car, struct sexpr *cadr);
// creates new node in CDR with given instruction in its CAR, returns pointer to new CDR
struct sexpr *addInstructions(struct sexpr *car, enum instruction);
// creates new node in CDR with the int value in its CAR, returns pointer to new CDR
struct sexpr *addValue(struct sexpr *car, int value);
// returns new S expr with NIL instruction
struct sexpr *createSexpr();

//prints --a-- representation of the sexpr
void printSexpr(sexpr* car);
void printSexprIndent(sexpr* sexprToPrint, int indent);

#endif
