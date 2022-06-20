/*
*   Description of the instructions
*
*    NIL                s e (NIL.c) d           ->      (NIL.s) e c d
*    LDC                s e (LDC a.c) d         ->      (a.s) e c d
*    LD                 s e (LD (i.j).c) d      ->      (locate((i.j),e).s) e c d       locate(i,j) means to find the value in the ith list, with index j in e.
*    ATOM               (a.s) e (OP.c) d        ->      ((OP a).s) e c d              (OP a) means that the result of atom(a) will be pushed on the stack. 
*    CAR                (a.s) e (OP.c) d        ->      ((OP a).s) e c d
*    CDR                (a.s) e (OP.c) d        ->      ((OP a).s) e c d
*    CONS               (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d            (a OP b) means that the result of cons(a,b) will be pushed on the stack
*    ADD                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    SUB                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    EQ                 (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    LEQ                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    LE                 (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    GEQ                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    GE                 (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    MUL                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d
*    DIV                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d  
*    AND                (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d  
*    OR                 (a b.s) e (OP.c) d      ->      ((a OP b).s) e c d  
*    SEL                (x.s) e (SEL T F.c) d   ->      s e c? (c.d)                   SEL takes the last value on the stack, if 0 F will be execute, else T.
*    JOIN               s e (JOIN.c) (r.d)      ->      s e r d                        the car of dump should contain the control register contents after the select
*    LDF                s e (LDF f.c) d         ->      ((f.e).s) e c d                pushes a cons cell with the function and current enviroment on the stack
*    AP                 ((f.e')v.s) e (AP.c) d  ->      NIL (v.e') f (s e c.d)         applys the function from the stack. by loading the function into control, and setting the enviroment.
*    RTN                (x.z)e'(RTN.q)(s e c.d) ->      (x.s) e c d                    keeps the result of the function on the stack and returns to the main program flow
*    DUM                s e (DUM.c) d           ->      s (NIL.e) c d                  adds a dummy variable to the enviroment
*    RAP                ((f.(NIL.e)) v.s) (NIL.e) (RAP.c) d)
*                                               ->      NIL (rplaca((NIL.e),v).e) f (s e c.d)   havent fully understood the meaning yet. rplaca means replace car
*    STOP               s e (STOP.c) e          ->      s e (STOP.c) d                  stop the machine
*
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "secd.h"

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
void andInstruction(struct sesecd *secd);
void orInstruction(struct sesecd *secd);
void selInstruction(struct sesecd *secd);
void joinInstruction(struct sesecd *secd);
void ldfInstruction(struct sesecd *secd);
void apInstruction(struct sesecd *secd);
void rtnInstruction(struct sesecd *secd);
void dumInstruction(struct sesecd *secd);
void rapInstruction(struct sesecd *secd);
void stopInstruction(struct sesecd *secd);

void execute(struct sesecd *secd){

if(secd->c->car.instruction == 0) {
        printf("Error in S-EXPR structure, no instruction in control register");  
        exit(1);
}

switch(secd->c->car.instruction) {
    
    case NIL:
        nilInstruction(secd);
        break;
    case LDC:
        ldcInstruction(secd);
        break;
    case LD:
        ldInstruction(secd);
        break;
    case ATOM:
        atomInstruction(secd);
        break;
    case CAR:
        carInstruction(secd);
        break;
    case CDR:
        cdrInstruction(secd);
        break;
    case CONS:
        consInstruction(secd);
        break;
    case ADD:
        addInstruction(secd);
        break;
    case SUB:
        subInstruction(secd);
        break;
    case EQ:
        eqInstruction(secd);
        break;
    case LEQ:
        leqInstruction(secd);
        break;
    case LE:
        leInstruction(secd);
        break;
    case GEQ:
        geqInstruction(secd);
        break;
    case GE:
        geInstruction(secd);
        break;
    case MUL:
        mulInstruction(secd);
        break;
    case DIV:
        divInstruction(secd);
        break;
    case AND:
        andInstruction(secd);
        break;
    case OR:
        orInstruction(secd);
        break;
    case SEL:
        selInstruction(secd);
        break;
    case JOIN:
        joinInstruction(secd);
        break;
    case LDF:
        ldfInstruction(secd);
        break;
    case AP:
        apInstruction(secd);
        break;
    case RTN:
        rtnInstruction(secd);
        break;
    case DUM:
        dumInstruction(secd);
        break;
    case RAP:
        rapInstruction(secd);
        break;
    case STOP:
        stopInstruction(secd);
        break;

    }
}

struct sexpr *consLL(struct sexpr *car, struct sexpr *cdr){
    if (car == NULL) {
        printf("error during cons. car == null\n");
        exit(1);
    }
    if (cdr== NULL) {
        printf("error during cons. cdr == null\n");
        exit(1);
    }
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.list = car;
    cons->cdr = cdr;
    cons->type = LIST;
    return cons;
}

struct sexpr *consIL(int car, struct sexpr *cdr){
    if (cdr== NULL) {
        printf("error during cons. cdr == null\n");
        exit(1);
    }
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.value = car;
    cons->type = VALUE;
    cons->cdr = cdr;
    return cons;
}
struct sexpr *consLI(struct sexpr *car, int cdr){
    if (car == NULL) {
        printf("error during consLI. car == null, cdr = %d\n", cdr);
        exit(1);
    }
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.list = car;
    cons->type = LIST;
    consCDR->car.value = cdr;
    cons->cdr = consCDR;
    return cons;
}

struct sexpr *consII(int car, int cdr){
    struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    cons->car.value = car;
    cons->type = VALUE;
    consCDR->car.value = cdr;
    consCDR->type = VALUE;
    cons->cdr = consCDR;
    return cons;
}


void nilInstruction(struct sesecd *secd){
    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->type = NILVALUE;
    nil->cdr = secd->s->cdr;
    secd->s = nil;
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
    int element = secd->c->cdr->car.list->cdr->car.value;
    struct sexpr *environment = secd->e;
    for(int i = 1; i < subList; i++) {
        environment = environment->cdr;
    }
    environment = environment->car.list;
        for(int i = 1; i < element; i++) {
        environment = environment->cdr;
    }
    if(environment->type == VALUE) {
    printf("loaded value from env: %d\n", environment->car.value);
    secd->s = consIL(environment->car.value, secd->s);
    }
    else if(environment->type == LIST || environment->type == INSTRUCTION) {
    printf("loaded function from env\n");
    secd->s = consLL(environment->car.list, secd->s);
    }
    else{
        secd->s = consLL(createSexpr(), secd->s);
    }
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

    secd->s = consIL(secd->s->car.list->car.value, secd->s->cdr);
    secd->c = secd->c->cdr;
}

void cdrInstruction(struct sesecd *secd){

    secd->s = consLL(secd->s->car.list->cdr, secd->s->cdr);
    secd->c = secd->c->cdr;
}

void consInstruction(struct sesecd *secd){

    struct sexpr *newList;
    if(secd->s->type != LIST && secd->s->cdr->type != LIST) {
        newList = consII(secd->s->car.value, secd->s->cdr->car.value);
    } else if (secd->s->type != LIST) {
        newList = consIL(secd->s->car.value, secd->s->cdr->car.list);
    } else if (secd->s->cdr->type != LIST) {
        newList = consLI(secd->s->car.list, secd->s->cdr->car.value);
    } else {
        newList = consLL(secd->s->car.list, secd->s->cdr->car.list);
    }
    if (secd->s->cdr->cdr != NULL) {
    secd->s = consLL(newList, secd->s->cdr->cdr);
    } else {
        struct sexpr *nil = createSexpr();
        secd->s = consLL(newList, nil);
    }
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

void andInstruction(struct sesecd *secd){
    
    int result = 0;
    if (secd->s->car.value != 0 && secd->s->cdr->car.value != 0) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void orInstruction(struct sesecd *secd){

    int result = 0;
    if (secd->s->car.value != 0 || secd->s->cdr->car.value != 0) result = 1;
    secd->s = consIL(result, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;
}

void selInstruction(struct sesecd *secd){

    secd->d = consLL(secd->c->cdr->cdr->cdr, secd->d);
    if(secd->s->car.value != 0){
        secd->c = secd->c->cdr->car.list;
    } else secd->c = secd->c->cdr->cdr->car.list;

    secd->s = secd->s->cdr;
}

void joinInstruction(struct sesecd *secd){

    secd->c = secd->d->car.list;
    secd->d = secd->d->cdr;
}

void ldfInstruction(struct sesecd *secd){
    sexpr* funToLoad = secd->c->cdr->car.list;
    sexpr* restOfControl = secd->c->cdr->cdr;

    sexpr* pushedFun = consLL(funToLoad, secd->e);

    //printSexpr(pushedFun);
    printf("\n");

    secd->s = consLL(pushedFun, secd->s);
    secd->c = restOfControl;
    /*
    struct sexpr *functionEnviroment;
    functionEnviroment = consLL(secd->c->cdr->car.list, secd->e);
    secd->s = consLL(functionEnviroment, secd->s);
    secd->c = secd->c->cdr->cdr;
    */
}

void apInstruction(struct sesecd *secd){
    /*
    sexpr* funToApply = secd->s->car->car;
    sexpr* loadedEnv = secd->s->car->cdr;
    sexpr* environmentOnStack = secd->s->cdr->car.list;
    sexpr* stackContinuation = secd->s->cdr->cdr;
    sexpr* continuation = secd->c->cdr;

    secd->d = consLL(stackContinuation, consLL(secd->e, consLL(continuation, secd->d)));
    secd->s = NULL;
    secd->e = consLL(environmentOnStack, loadedEnv);
    secd->c = funToApply;
   */ 
    struct sexpr *controlDump;
    struct sexpr *envControlDump;
    controlDump = consLL(secd->c->cdr, secd->d);
    envControlDump = consLL(secd->e, controlDump);
    if(secd->s->cdr->cdr != NULL) {
    secd->d = consLL(secd->s->cdr->cdr, envControlDump);
    } else {
        struct sexpr *nil = createSexpr();
        secd->d = consLL(nil, envControlDump);
    }
    /*
    if(secd->s->cdr->type != LIST && secd->s->car.list->cdr->type != LIST) {
        secd->e = consII(secd->s->cdr->car.value, secd->s->car.list->cdr->car.value);
    } else if (secd->s->car.list->cdr->type!= LIST) {
        secd->e = consIL(secd->s->cdr->car.value, secd->s->car.list->cdr);
    } else if (secd->s->type != LIST) {
        secd->e = consLI(secd->s->cdr->car.list, secd->s->car.list->cdr->car.value);
    } else {
        secd->e = consLL(secd->s->cdr->car.list, secd->s->car.list->cdr);
    }
    */

    secd->e = consLL(secd->s->cdr->car.list, secd->s->car.list->cdr);
    
    secd->c = secd->s->car.list->car.list;

    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->type = NILVALUE;
    secd->s = nil;
}

void rtnInstruction(struct sesecd *secd){

    if(secd->s->type != LIST) {
            secd->s = consIL(secd->s->car.value, secd->d->car.list);
    } else  secd->s = consLL(secd->s->car.list, secd->d->car.list);

    secd->e = secd->d->cdr->car.list;
    secd->c = secd->d->cdr->cdr->car.list;
    secd->d = secd->d->cdr->cdr->cdr;
}

void dumInstruction(struct sesecd *secd){

    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->type= NILVALUE;
    secd->e = consLL(nil, secd->e);
    secd->c = secd->c->cdr;
}

void rapInstruction(struct sesecd *secd){
    
    struct sexpr *controlDump;
    struct sexpr *envControlDump;
    controlDump = consLL(secd->c->cdr, secd->d);
    envControlDump = consLL(secd->e, controlDump);
    if(secd->s->cdr->cdr != NULL) {
    secd->d = consLL(secd->s->cdr->cdr, envControlDump);
    } else {
        struct sexpr *nil = createSexpr();
        secd->d = consLL(nil, envControlDump);
    }

    /*
    if(secd->s->cdr->type != LIST && secd->s->car.list->cdr->cdr->type != LIST) {
        secd->e = consII(secd->s->cdr->car.value, secd->s->car.list->cdr->cdr->car.value);
    } else if (secd->s->car.list->cdr->cdr->type != LIST) {
        secd->e = consIL(secd->s->cdr->car.value, secd->s->car.list->cdr->cdr);
    } else if (secd->s->type != LIST) {
        secd->e = consLI(secd->s->cdr->car.list, secd->s->car.list->cdr->cdr->car.value);
    } else {
        secd->e = consLL(secd->s->cdr->car.list, secd->s->car.list->cdr->cdr);
    }
    */
    secd->e = secd->s->car.list->cdr;
    // rplaca. replace head of e 
    secd->e = consLL(secd->s->cdr->car.list, secd->e->cdr);
    secd->c = secd->s->car.list->car.list;
    
    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->type = NILVALUE;
    secd->s = nil;
}

void stopInstruction(struct sesecd *secd) {
    printf("Result: %d\n", secd->s->car.value);
    exit(0);
}



struct sexpr *addCDRList(struct sexpr *car, struct sexpr *cadr){
    if (cadr == NULL) {
        printf("cadr == NULL in addCDRList\n");
        exit(1);
    }
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->type = LIST;
    cdr->car.list = cadr;
    return cdr;
}

struct sexpr *addInstructions(struct sexpr *car, enum instruction instruction){
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->type = INSTRUCTION;
    cdr->car.instruction = instruction;
    return cdr;
}

struct sexpr *addValue(struct sexpr *car, int value){
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->type = VALUE;
    cdr->car.value = value;
    return cdr;
}

struct sexpr *createSexpr(){
    struct sexpr *car = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->type = NILVALUE;
    car->car.instruction = NIL;
    return car;
}


void printSexpr(sexpr* car){
    if(car == NULL) {
        printf("NULL");
        return;
    }
    printf("{car:%d, cdr:", car->car);
    printSexpr(car->cdr);
    printf("}");
}