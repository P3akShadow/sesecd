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
        nilInstruction(*secd);
        break;
    case LDC:
        ldcInstruction(*secd);
        break;
    case LD:
        ldInstruction(*secd);
        break;
    case ATOM:
        atomInstruction(*secd);
        break;
    case CAR:
        carInstruction(*secd);
        break;
    case CDR:
        cdrInstruction(*secd);
        break;
    case CONS:
        consInstruction(*secd);
        break;
    case ADD:
        addInstruction(*secd);
        break;
    case SUB:
        subInstruction(*secd);
        break;
    case EQ:
        eqInstruction(*secd);
        break;
    case LEQ:
        leqInstruction(*secd);
        break;
    case LE:
        leInstruction(*secd);
        break;
    case GEQ:
        geqInstruction(*secd);
        break;
    case GE:
        geInstruction(*secd);
        break;
    case MUL:
        mulInstruction(*secd);
        break;
    case DIV:
        divInstruction(*secd);
        break;
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

void nilInstruction(struct sesecd *secd){
    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->car.instruction = NIL;
    secd->s = consLL(nil, NULL);
    secd->c = secd->c->cdr;
}

void ldcInstruction(struct sesecd *secd){
    secd->s = consIL(secd->c->cdr->car.value, secd->s);
    if(secd->c->cdr->cdr == NULL) {
        printf("Error in S-Expression, no Instructions after LDC.");
        exit(1);
    }
    secd->c = secd->c->cdr->cdr;
}

// ld(5.3) means that the third element of the fifth list in E is being. The list is in the cadr of c. (cdr->car.)
void ldInstruction(struct sesecd *secd){

    int subList = secd->c->cdr->car.list->car.value;
    int element = secd->c->cdr->car.list->car.list->car.value;
    struct sexpr *environment = secd->e;
    for(int i = 1; i < subList; i++) {
        enviroment = enviroment->cdr
    }
    enviroment = enviroment->car.list;
        for(int i = 1; i < element; i++) {
        enviroment = enviroment->cdr
    }
    secd->s = consIL(enviroment->car.value, secd->s);
    secd->c = secd->c->cdr->cdr;
}

void atomInstruction(struct sesecd *secd){

    if(secd->s->car.list == NULL){
        secd->s = consIL(1, secd->s->cdr);
    }
    else secd->s = consIL(0, secd->s->cdr);

    secd->c = secd->c->cdr;
}

void carInstruction(struct sesecd *secd){

    secd->s = consLL(secd->s->car.list->car.value, secd->s->cdr)
    secd->c = secd->c->cdr;
}

void cdrInstruction(struct sesecd *secd){

    secd->s = consLL(secd->s->car.list->cdr, secd->s->cdr)
    secd->c = secd->c->cdr;
}

void consInstruction(struct sesecd *secd){

    struct sexpr newList;
    if(secd->s->car.list = NULL && secd->s->cdr->car.list = NULL) {
        newList = consII(secd->s->car.value, secd->s->cdr->car.value);
    } else if (secd->s->car.list = NULL) {
        newList = consIL(secd->s->car.value, secd->s->cdr->car.list);
    } else if (secd->s->cdr->car.list = NULL) {
        newList = consIL(secd->s->car.list, secd->s->cdr->car.value);
    } else {
        newList = consLL(secd->s->car.list, secd->s->cdr->car.list);
    }
    secd->c = consLL(newList, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void addInstruction(struct sesecd *secd){

    int result = secd->s->car.value + secd->s->cdr->car.value;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void subInstruction(struct sesecd *secd){

    int result = secd->s->car.value - secd->s->cdr->car.value;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void eqInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value == secd->s->cdr->car.value) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}


void leqInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value <= secd->s->cdr->car.value) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void leInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value < secd->s->cdr->car.value) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void geqInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value >= secd->s->cdr->car.value) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void geInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value > secd->s->cdr->car.value) result = 1;    
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void mulInstruction(struct sesecd *secd){

    int result = secd->s->car.value * secd->s->cdr->car.value;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void divInstruction(struct sesecd *secd){

    int result = secd->s->car.value / secd->s->cdr->car.value;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}



void stopInstruction(struct sesecd *secd) {
    printf("Result: %d", secd->s->car.value);
    exit(0);
}

}