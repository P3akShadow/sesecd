#ifndef SECD_H
#define SECD_H

// instruction set of the sesecd
typedef enum {
    NIL,
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
    SEL,
    JOIN,
    LDF,
    AP,
    RTN,
    DUM,
    RAP,
    STOP
} instruction;


// Nested list that can contain tree structured data. car is the head and cdr the tail. 
// (contents of the address part of the register, contents of the decrement part of the register)
// value wil normally be interpreted as a number, but after instructions that produce a true or false value, 0 will be false and ever other integer true.
typedef struct sexpr {
  
  struct sexpr *cdr;
  union car {
    struct sexpr *list;
    int value;
    enum instruction instruction;
  }
} sexpr;

// The 4 registers can take arbitrary s-expressions. 
typedef struct sesecd {
  
  struct sexpr *s
  struct sexpr *e
  struct sexpr *c
  struct sexpr *d
} sesecd;

// execute the programm. should read from the c s-expression and manipulate the secd state accordingly. 
void run(struct sesecd *secd);
// constructs memory objects that can hold integer values or pointers to s-expr. When assiging integer value to cdr it will still be a pointer to an s-expr
// containing an integer value in its car. L=List, I= Integer. 
struct sexpr consLL(struct sexpr *car, struct sexpr *cdr);
struct sexpr consIL(int car, struct sexpr *cdr);
struct sexpr consLI(struct sexpr *car, int cdr);
struct sexpr consII(int *car, int *cdr);


#endif
