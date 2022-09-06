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

void nilInstruction();
void ldcInstruction();
void ldInstruction();
void atomInstruction();
void carInstruction();
void cdrInstruction();
void consInstruction();
void addInstruction();
void subInstruction();
void eqInstruction();
void leqInstruction();
void leInstruction();
void geqInstruction();
void geInstruction();
void mulInstruction();
void divInstruction();
void andInstruction();
void orInstruction();
void selInstruction();
void joinInstruction();
void ldfInstruction();
void apInstruction();
void rtnInstruction();
void dumInstruction();
void rapInstruction();
void stopInstruction();

void specparInstruction();
void mapInstruction();

sesecd *secd;
//these are the pages where the elements live
sexpr pages[2][MAX_PAGE_SIZE];
//current page
int pageIndex = 0;
int indexOnPage = 0;
//writes the new location fo each sexpr that has been copied
sexpr* savedSexprs[MAX_PAGE_SIZE];
sesecd* secdToSave = NULL;

//this is a an array of pointer pointers, since the destination of the pointer will be specified
sexpr **pointersToOverwrite[MAX_PAGE_SIZE];
int gcPointerToOverwriteIndex = 0;

int gcConducted = 0;

void execute(){

if(secd->c->car.instruction == 0) {
        printf("Error in S-EXPR structure, no instruction in control register");  
        exit(1);
}

switch(secd->c->car.instruction) {
    
    case NIL:
        nilInstruction();
        break;
    case LDC:
        ldcInstruction();
        break;
    case LD:
        ldInstruction();
        break;
    case ATOM:
        atomInstruction();
        break;
    case CAR:
        carInstruction();
        break;
    case CDR:
        cdrInstruction();
        break;
    case CONS:
        consInstruction();
        break;
    case ADD:
        addInstruction();
        break;
    case SUB:
        subInstruction();
        break;
    case EQ:
        eqInstruction();
        break;
    case LEQ:
        leqInstruction();
        break;
    case LE:
        leInstruction();
        break;
    case GEQ:
        geqInstruction();
        break;
    case GE:
        geInstruction();
        break;
    case MUL:
        mulInstruction();
        break;
    case DIV:
        divInstruction();
        break;
    case AND:
        andInstruction();
        break;
    case OR:
        orInstruction();
        break;
    case SEL:
        selInstruction();
        break;
    case JOIN:
        joinInstruction();
        break;
    case LDF:
        ldfInstruction();
        break;
    case AP:
        apInstruction();
        break;
    case RTN:
        rtnInstruction();
        break;
    case DUM:
        dumInstruction();
        break;
    case RAP:
        rapInstruction();
        break;
    case STOP:
        stopInstruction();
        break;

    case SPECPAR:
        specparInstruction();
        break;
    case MAP:
        mapInstruction();
        break;
    }
}

sexpr *saveSexpr(sexpr* sexprToSave, sexpr* oldPage, sexpr* newPage);

sexpr *mallocSexpr(){
    sexpr *page = pages[pageIndex];
    if(indexOnPage < MAX_PAGE_SIZE){
        return &page[indexOnPage++];
    }

    //printf("initializing garbage collection\n");
    gcConducted++;

    pageIndex = (pageIndex + 1) % 2;
    indexOnPage = 0;
    
    sexpr *newPage = pages[pageIndex];

    //in the beginning, no element is saved
    memset(savedSexprs, 0, sizeof(sexpr*) * MAX_PAGE_SIZE);

    //printf("saving s\n");
    secd->s = saveSexpr(secd->s, page, newPage);
    //printf("saving e\n");
    secd->e = saveSexpr(secd->e, page, newPage);
    //printf("saving c\n");
    secd->c = saveSexpr(secd->c, page, newPage);
    //printf("saving d(%d)\n", secd->d);
    secd->d = saveSexpr(secd->d, page, newPage);


    for (int i = 0; i < gcPointerToOverwriteIndex; i++){
        fflush(stdout);

        if(pointersToOverwrite[i] == NULL){
            continue;
        }
        
        sexpr **locationToOverwrite = pointersToOverwrite[i];
        *locationToOverwrite = saveSexpr(*locationToOverwrite, page, newPage);
    }

    return mallocSexpr();
}

sexpr *saveSexpr(sexpr* sexprToSave, sexpr* oldPage, sexpr* newPage){
    //to decide whether the car/cdr must be saved, one simply looks up if it points into the old page
    //there is an error potential, if the value of the expr is inside of the page or the car/cdr are not used but not set to null

    int oldIndex = (sexprToSave - oldPage);

    if(oldIndex < 0 || oldIndex >= MAX_PAGE_SIZE){
        return sexprToSave;
    }

    //this means, the expr has already been saved
    if(savedSexprs[oldIndex] != NULL){
        //if already exists, just returns the newLocation, saved at savedSexprs[oldIndex]
        return savedSexprs[oldIndex];
    }

    sexpr* newLoc = mallocSexpr();

    memcpy(newLoc, sexprToSave, sizeof(sexpr));
    savedSexprs[oldIndex] = newLoc;


    int carDiff = (sexprToSave->car.list - oldPage);
    if(carDiff >= 0 && carDiff < MAX_PAGE_SIZE){
        newLoc->car.list = saveSexpr(sexprToSave->car.list, oldPage, newPage);
    } else {
        newLoc->car = sexprToSave->car;
    }

    int cdrDiff = (sexprToSave->cdr - oldPage);
    if(cdrDiff >= 0 && cdrDiff < MAX_PAGE_SIZE){
        newLoc->cdr = saveSexpr(sexprToSave->cdr, oldPage, newPage);
    } else {
        newLoc->cdr = sexprToSave->cdr;
    }
    return newLoc;
}

struct sexpr *consLL(struct sexpr *car, struct sexpr *cdr){
    int startIndex = gcPointerToOverwriteIndex;
    if(car != NULL){
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &car;
    }
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &cdr;
    //struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    sexpr *cons = mallocSexpr();

    cons->car.list = car;
    cons->cdr = cdr;
    gcPointerToOverwriteIndex = startIndex;

    return cons;
}

struct sexpr *consIL(int car, struct sexpr *cdr){
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &cdr;
    //struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    sexpr *cons = mallocSexpr();
    
    cons->car.value = car;
    cons->cdr = cdr;
    gcPointerToOverwriteIndex--;

    return cons;
}
struct sexpr *consLI(struct sexpr *car, int cdr){
    
    int startIndex = gcPointerToOverwriteIndex;
    if(car != NULL){
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &car;
    }
    //struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    //struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    sexpr *cons = mallocSexpr();
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &cons;
    sexpr *consCDR = mallocSexpr();
    cons->car.list = car;
    consCDR->car.value = cdr;
    cons->cdr = consCDR;
    gcPointerToOverwriteIndex = startIndex;

    return cons;
}

struct sexpr *consII(int car, int cdr){
    //struct sexpr *cons = (struct sexpr*) malloc(sizeof(struct sexpr));
    //struct sexpr *consCDR = (struct sexpr*) malloc(sizeof(struct sexpr));
    sexpr *cons = mallocSexpr();
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &cons;
    sexpr *consCDR = mallocSexpr();
    cons->car.value = car;
    consCDR->car.value = cdr;
    cons->cdr = consCDR;
    gcPointerToOverwriteIndex--;

    return cons;
}


void nilInstruction(){
    //struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    sexpr *nil = mallocSexpr();
    nil->car.instruction = NIL;
    secd->s = consLL(nil, NULL);
    secd->c = secd->c->cdr;
}

void ldcInstruction(){
    secd->s = consLL(secd->c->cdr->car.list, secd->s);
    if(secd->c->cdr->cdr == NULL) {
        printf("Error in S-Expression, no Instructions after LDC.");
        exit(1);
    }
    secd->c = secd->c->cdr->cdr;
}

// ld(5.3) means that the third element of the fifth list in E is being. The list is in the cadr of c. (cdr->car.)
void ldInstruction(){
    fflush(stdout);

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
    secd->s = consLL(environment->car.list, secd->s);
    secd->c = secd->c->cdr->cdr;
}

void atomInstruction(){

    if(secd->s->car.list == NULL){
        secd->s = consIL(1, secd->s->cdr);
    }
    else secd->s = consIL(0, secd->s->cdr);

    secd->c = secd->c->cdr;
}

void carInstruction(){

    secd->s = consIL(secd->s->car.list->car.value, secd->s->cdr);
    secd->c = secd->c->cdr;
}

void cdrInstruction(){

    secd->s = consLL(secd->s->car.list->cdr, secd->s->cdr);
    secd->c = secd->c->cdr;
}

void consInstruction(){
    int pointerAtTheStart = gcPointerToOverwriteIndex;

    struct sexpr *newList;
    if(secd->s->car.list == NULL && secd->s->cdr->car.list == NULL) {
        newList = consII(secd->s->car.value, secd->s->cdr->car.value);
    } else if (secd->s->car.list == NULL) {
        newList = consIL(secd->s->car.value, secd->s->cdr->car.list);
    } else if (secd->s->cdr->car.list == NULL) {
        newList = consLI(secd->s->car.list, secd->s->cdr->car.value);
    } else {
        newList = consLL(secd->s->car.list, secd->s->cdr->car.list);
    }
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &newList;
    secd->c = consLL(newList, secd->s->cdr->cdr);
    gcPointerToOverwriteIndex--;
    secd->c = secd->c->cdr;

    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }
}

void expandRecordOnStack(){
    int pointerAtTheStart = gcPointerToOverwriteIndex;

    sexpr* ap = consIL(AP, secd->c);
    secd->c = ap;

    sexpr* env = consLL(secd->s->car.list->car.list, secd->s->cdr);
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &env;
    sexpr* pushedFun = consLL(secd->s->car.list->cdr, secd->e);
    secd->s = consLL(pushedFun, env);
    gcPointerToOverwriteIndex--;

    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }
}

void calcTosCdr(){
    int pointerAtTheStart = gcPointerToOverwriteIndex;

    if(secd->s->cdr->car.list->type == FUNCTION){
        sexpr* oldStack = secd->s;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &oldStack;
    
        secd->s = secd->s->cdr;

        expandRecordOnStack(secd);
        
        for(int i = 0; secd->c->car.instruction != RTN; i++){
            execute();
            //printf("after %d steps of final evaluation:\ns:\n", ++i);
    
            //printSexpr(secd->s);
            //printf("\ne:\n");
            //printSexpr(secd->e);
            //printf("\nc:\n");
            //printSexpr(secd->c);
            //printf("\nd:\n");
            //printSexpr(secd->d);
            //printf("\n");
        }
        execute();

        secd->s = consLL(oldStack->car.list, secd->s);
        gcPointerToOverwriteIndex--;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }
}

void calcTos(){
    if(secd->s->car.list->type == FUNCTION){
        expandRecordOnStack();
        for(int i = 0; secd->c->car.instruction != RTN && i < 100; i++){
            execute();
            //printf("after %d steps of final evaluation:\ns:\n", ++i);
    
            //printSexpr(secd->s);
            //printf("\ne:\n");
            //printSexpr(secd->e);
            //printf("\nc:\n");
            //printSexpr(secd->c);
            //printf("\nd:\n");
            //printSexpr(secd->d);
            //printf("\n");
        }
        execute();
        return;
    }
}

void addInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        int result = secd->s->car.list->car.value + secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void subInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;

        int result = secd->s->cdr->car.list->car.value - secd->s->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void eqInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;

        int result = secd->s->car.list->car.value == secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}


void leqInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        //other way round, since the second element of the stack is the defining one
        int result = secd->s->car.list->car.value >= secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void leInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        //other way round, since the second element of the stack is the defining one
        int result = secd->s->car.list->car.value > secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}
   
void geqInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        //other way round, since the second element of the stack is the defining one
        int result = secd->s->car.list->car.value <= secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void geInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        //other way round, since the second element of the stack is the defining one
        int result = secd->s->car.list->car.value < secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void mulInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        int result = secd->s->car.list->car.value * secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void divInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        int result = secd->s->cdr->car.list->car.value / secd->s->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void andInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        int result = secd->s->car.list->car.value && secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void orInstruction(){
    //these two functions make sure that there are just values on the top of the stack and the next part of the stack
    calcTosCdr(secd);
    calcTos(secd);

    if(secd->s->cdr->car.list->type == CONSTANT && secd->s->car.list->type == CONSTANT){
        int pointerAtTheStart = gcPointerToOverwriteIndex;
        
        int result = secd->s->car.list->car.value || secd->s->cdr->car.list->car.value;
        sexpr* valCont = consIL(result, NULL);
        valCont->type = CONSTANT;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &valCont;
        secd->s = consLL(valCont, secd->s->cdr->cdr);
        gcPointerToOverwriteIndex--;
        secd->c = secd->c->cdr;

        if(pointerAtTheStart != gcPointerToOverwriteIndex){
            printf("pointer neq to the start\n");
            exit(1);
        }
        return;
    }

    printf("type on stack unknown (%d, %d), abort\n", secd->s->car.list->type, secd->s->cdr->car.list->type);
    exit(1);
}

void selInstruction(){

    secd->d = consLL(secd->c->cdr->cdr->cdr, secd->d);
    if(secd->s->car.list->car.value != 0){
        secd->c = secd->c->cdr->car.list;
    } else secd->c = secd->c->cdr->cdr->car.list;

    secd->s = secd->s->cdr;
}

void joinInstruction(){

    secd->c = secd->d->car.list;
    secd->d = secd->d->cdr;
}

void ldfInstruction(){
    int pointerAtTheStart = gcPointerToOverwriteIndex;
        
    sexpr* funToLoad = secd->c->cdr->car.list;
    sexpr* restOfControl = secd->c->cdr->cdr;

    sexpr* pushedFun = consLL(funToLoad, secd->e);

    //printSexpr(pushedFun);
    //printf("\n");

    pointersToOverwrite[gcPointerToOverwriteIndex++] = &pushedFun;
    secd->s = consLL(pushedFun, secd->s);
    gcPointerToOverwriteIndex--;

    secd->c = restOfControl;

    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }
    /*
    struct sexpr *functionEnviroment;
    functionEnviroment = consLL(secd->c->cdr->car.list, secd->e);
    secd->s = consLL(functionEnviroment, secd->s);
    secd->c = secd->c->cdr->cdr;
    */
}

void apInstruction(){
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
    int pointerAtTheStart = gcPointerToOverwriteIndex;
    
    struct sexpr *controlDump;
    struct sexpr *envControlDump;

    controlDump = consLL(secd->c->cdr, secd->d);
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &controlDump;
    envControlDump = consLL(secd->e, controlDump);
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &envControlDump;
    secd->d = consLL(secd->s->cdr->cdr, envControlDump);
    secd->e = consLL(secd->s->cdr->car.list, secd->s->car.list->cdr);
    gcPointerToOverwriteIndex -= 2;

    secd->c = secd->s->car.list->car.list;

    secd->s = consLL(NULL, NULL);

    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }
}

void rtnInstruction(){
    secd->s = consLL(secd->s->car.list, secd->d->car.list);


    secd->e = secd->d->cdr->car.list;
    secd->c = secd->d->cdr->cdr->car.list;
    secd->d = secd->d->cdr->cdr->cdr;
}

void dumInstruction(){

    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->car.instruction = NIL;
    secd->e = consLL(nil, secd->e);
    secd->c = secd->c->cdr;
}

void rapInstruction(){
    
    struct sexpr *controlDump;
    struct sexpr *envControlDump;
    controlDump = consLL(secd->c->cdr, secd->d);
    envControlDump = consLL(secd->e, controlDump);
    secd->d = consLL(secd->s->cdr->cdr, envControlDump);
    secd->e = consLL(secd->s->car.list->cdr, secd->s->cdr->car.list);
    secd->c = secd->s->car.list->car.list;
    
    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->car.instruction = NIL;
    secd->s = consLL(nil, NULL);
}

void stopInstruction() {
    printf("stop found\n");

    if(secd->s->car.list->type == CONSTANT){
        printf("Result: %d\n", secd->s->car.list->car.value);
        printf("total times garbage has been collected %d\n", gcConducted);
        double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
        printf("Evaluation took %f seconds\n", elapsed_time);
        exit(0);
    }

    if(secd->s->car.list->type == LIST){
        printf("Result: List\n");

        sexpr* element = secd->s->car.list->cdr;
        pointersToOverwrite[gcPointerToOverwriteIndex++] = &element;

        int results[1000];
        int numOfElems = 0;

        while(element != NULL && numOfElems < 1000){
            printf("calculating result of index %d\n", numOfElems);

            if(element->type == CONSTANT){
                results[numOfElems] = element->car.list->car.value;
            }

            else if (element->type == FUNCTION){
                //setup secd to evaluate the function
                secd->s = consLL(element->car.list, secd->s);
                secd->c = consIL(AP, secd->c);
                secd->c = consIL(LDF, secd->c);

                while(secd->s->car.list->type == FUNCTION){
                    calcTos(secd);
                }

                results[numOfElems] = secd->s->car.list->car.value;
                secd->s = secd->s->cdr;
            }
            numOfElems++;
            element = element->cdr;
        }

        for(int i = 0; i < numOfElems; i ++){
            printf("%d, ", results[i]);
        }

        printf("\n");
        gcPointerToOverwriteIndex--;

        printf("total times garbage has been collected %d\n", gcConducted);
        double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
        printf("Evaluation took %f seconds\n", elapsed_time);
        exit(0);
    }

    printf("Still a function on stack!\nExpanding to show a concrete value\n");
    expandRecordOnStack(secd);

    for(int i = 0; ; i++){
        execute();
        //printf("after %d steps of final evaluation:\ns:\n", ++i);

        //printSexpr(secd->s);
        //printf("\ne:\n");
        //printSexpr(secd->e);
        //printf("\nc:\n");
        //printSexpr(secd->c);
        //printf("\nd:\n");
        //printSexpr(secd->d);
        //printf("\n");
    }

    printf("total times garbage has been collected %d\n", gcConducted);
    double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
    printf("Evaluation took %f seconds\n", elapsed_time);
    exit(0);
}

void specparInstruction() {
    int pointerAtTheStart = gcPointerToOverwriteIndex;
    //printf("specifiying a param\n");

    sexpr* oldEnv = secd->s->cdr->car.list->car.list;
    //loads from stack in order to be able to digest values that have already been passed as params
    sexpr* newEnv = consLL(secd->s->car.list, oldEnv);
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &newEnv;
    sexpr* newRoot = consLL(newEnv, secd->s->cdr->car.list->cdr);
    newRoot->type = FUNCTION;
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &newRoot;
    secd->s = consLL(newRoot, secd->s->cdr->cdr);
    gcPointerToOverwriteIndex -= 2;
    secd->c = secd->c->cdr;

    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }
}

void mapInstruction(){
    int pointerAtTheStart = gcPointerToOverwriteIndex;
    
    //the list in the cdr can not be a function
    calcTosCdr(secd);

    //this is the function that needs to be mapped (here, the function is in the first place)
    printf("map instruction detected\n");
    sexpr* functionArg = secd->s->car.list;
    sexpr* list = secd->s->cdr->car.list;

    pointersToOverwrite[gcPointerToOverwriteIndex++] = &functionArg;
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &list;

    sexpr* newList = consLL(NULL,NULL);
    newList->type = LIST;
    sexpr* newListElem = newList;
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &newList;
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &newListElem;

    sexpr* listElem = list->cdr;

    sexpr* oldEnv = functionArg->car.list;
    pointersToOverwrite[gcPointerToOverwriteIndex++] = &oldEnv;

    gcPointerToOverwriteIndex += 3;

    while(listElem != NULL){
        pointersToOverwrite[gcPointerToOverwriteIndex - 3] = &listElem;

        sexpr* newEnv = consLL(listElem->car.list, oldEnv);
        pointersToOverwrite[gcPointerToOverwriteIndex - 2] = &newEnv;
        sexpr* newRoot = consLL(newEnv, functionArg->cdr);

        pointersToOverwrite[gcPointerToOverwriteIndex - 1] = &newRoot;

        newRoot->type = FUNCTION;

        newListElem->cdr = consLL(NULL, NULL);
        newListElem = newListElem->cdr;
        newListElem->car.list = newRoot;

        listElem = listElem->cdr;
    }

    secd->s = consLL(newList, secd->s->cdr->cdr);
    secd->c = secd->c->cdr;

    gcPointerToOverwriteIndex -= 8;


    if(pointerAtTheStart != gcPointerToOverwriteIndex){
        printf("pointer neq to the start\n");
        exit(1);
    }

}

/*
struct sexpr *addCDRList(struct sexpr *car, struct sexpr *cadr){
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->car.list = cadr;
    return cdr;
}

struct sexpr *addInstructions(struct sexpr *car, enum instruction instruction){
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->car.instruction = instruction;
    return cdr;
}

struct sexpr *addValue(struct sexpr *car, int value){
    struct sexpr *cdr = (struct sexpr*) malloc(sizeof(struct sexpr));
    car->cdr = cdr;
    cdr->car.value = value;
    return cdr;
}
*/

struct sexpr *createSexpr(){
    struct sexpr *car = mallocSexpr();
    car->car.instruction = NIL;
    return car;
}

/*
void printSexpr(sexpr* car){
    if(car == NULL) {
        printf("NULL");
        return;
    }
    printf("{car:%d (own index would be %d)", car->car.value, car - pages[pageIndex]);
    if(car->car.value > 1000 || car->car.value < -1000){
        if(car->car.list->type == CONSTANT){printf("(containing con %d)", car->car.list->car.value);}
    }
    printf(", cdr:");
    printSexpr(car->cdr);
    printf("}");
}

void printSexprIndent(sexpr* sexprToPrint, int indent){
    if(sexprToPrint == NULL){
        printf("%*sNULL", indent, "");
    }

    printf("%*s{ own index: %d car-raw-value: %d\n", indent, "", sexprToPrint - pages[pageIndex], sexprToPrint->car.value);
    if(sexprToPrint->car.list - pages[pageIndex] >= 0 && sexprToPrint->car.list - pages[pageIndex] < MAX_PAGE_SIZE){
        if(indent > 20){
            printf("%*sshowing car only until indent 20\n", indent, "");
            return;
        }
        printf("%*scar:\n", indent, "");
        printSexprIndent(sexprToPrint->car.list, indent + 1);
    }


    if(sexprToPrint->cdr - pages[pageIndex] >= 0 && sexprToPrint->cdr - pages[pageIndex] < MAX_PAGE_SIZE){
        printf("%*scdr:\n", indent, "");
        printSexprIndent(sexprToPrint->cdr, indent + 1);
    }
    printf("%*s}\n", indent, "");
}
*/