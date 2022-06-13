#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "secd.h"


void run(struct sesecd *secd){

switch(secd->c->car.instruction) {
    
    case NULL:
        printf("Error in S-EXPR structure, no instruction in control register");  
        exit(1);
        break;
    case NIL:
        stopInstruction(*secd);
        break;
    case LDC:
    case LD:
    case ATOM:
    case CAR:
    case CDR:
    case CONS:
    case ADD:
    case SUB:
    case EQ:
    case LEQ:
    case LE:
    case GEQ:
    case GE:
    case MUL:
    case DIV:
    case AND:
    case OR:
    case SEL:
    case JOIN:
    case LDF:
    case AP:
    case RTN:
    case DUM:
    case RAP:
    case STOP:
        stopInstruction(*secd);
        break;

}


struct sexpr consLL(struct sexpr *car, struct sexpr *cdr){
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.list = *car;
    cons->cdr = *cdr;
    return cons;
}

struct sexpr consIL(int car, struct sexpr *cdr){
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.value = car;
    cons->cdr = *cdr;
    return cons;
}
struct sexpr consLI(struct sexpr *car, int cdr){
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.list = *car;
    consCDR->car.value = cdr;
    cons->cdr = *consCDR;
    return cons;
}
struct sexpr consII(int *car, int *cdr){
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.value = car;
    consCDR->car.value = cdr;
    cons->cdr = *consCDR;
    return cons;
}


void stopInstruction(struct sesecd *secd) {
    printf("Result: %d", secd->s->car.value);
    exit(0);
}

}