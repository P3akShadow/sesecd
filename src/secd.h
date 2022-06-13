#ifndef SECD_H
#define SECD_H

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
void execute(struct sesecd *secd);
// constructs memory objects that can hold integer values or pointers to s-expr. When assiging integer value to cdr it will still be a pointer to an s-expr
// containing an integer value in its car. L=List, I= Integer. 
struct sexpr *consLL(struct sexpr *car, struct sexpr *cdr);
struct sexpr *consIL(int car, struct sexpr *cdr);
struct sexpr *consLI(struct sexpr *car, int cdr);
struct sexpr *consII(int car, int cdr);

void nilInstruction(struct sesecd *secd);
void ldcInstruction(struct sesecd *secd);
void ldInstruction(struct sesecd *secd);
void atomInstruction(struct sesecd *secd);
void carInstruction(struct sesecd *secd);
void cdrInstruction(struct sesecd *secd);
void consInstruction(struct sesecd *secd);
void addInstruction(struct sesecd *secd);
void subInstruction(struct sesecd *secd);
void eqInstruction(struct sesecd *secd);
void leqInstruction(struct sesecd *secd);
void leInstruction(struct sesecd *secd);
void geqInstruction(struct sesecd *secd);
void geInstruction(struct sesecd *secd);
void mulInstruction(struct sesecd *secd);
void divInstruction(struct sesecd *secd);
void selInstruction(struct sesecd *secd);
void joinInstruction(struct sesecd *secd);
void ldfInstruction(struct sesecd *secd);
void apInstruction(struct sesecd *secd);
void rtnInstruction(struct sesecd *secd);
void dumInstruction(struct sesecd *secd);
void rapInstruction(struct sesecd *secd);
void stopInstruction(struct sesecd *secd);


#endif
