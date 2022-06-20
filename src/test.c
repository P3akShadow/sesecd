#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "secd.h"

int main() {
/*
    struct sexpr *control, *stack, *env, *dump, *ldc, *stop, *constant, *ldc2, *constant2, *add;
    control = (struct sexpr*) malloc(sizeof(struct sexpr));
    ldc=(struct sexpr*) malloc(sizeof(struct sexpr));
    stop = (struct sexpr*) malloc(sizeof(struct sexpr));
    constant = (struct sexpr*) malloc(sizeof(struct sexpr));
    constant2 = (struct sexpr*) malloc(sizeof(struct sexpr));
    ldc2 = (struct sexpr*) malloc(sizeof(struct sexpr));
    add = (struct sexpr*) malloc(sizeof(struct sexpr));
    control->car.instruction = NIL;
    control->cdr= ldc;
    ldc->car.instruction = LDC;
    ldc->cdr = constant;
    constant->car.value= 10;
    constant->cdr = ldc2;
    ldc2->car.instruction = LDC;
    ldc2->cdr = constant2;
    constant2->car.value = 20;
    constant2->cdr = add;
    add->car.instruction = ADD;
    add->cdr = stop;
    stop->car.instruction = STOP;
*/
    struct sexpr *control, *workPointer, *stack, *env, *dump, *function, *function2, *function3, *ld, *ld2, *ld3, *ld4, *ld5, *sel1, *sel2;
    /*
    control = createSexpr();
    workPointer = control;
    workPointer = addInstructions(workPointer, LDC);
    workPointer = addValue(workPointer, 20);
    workPointer = addInstructions(workPointer, LDC);
    workPointer = addValue(workPointer, 50);
    workPointer = addInstructions(workPointer, ADD);
    workPointer = addInstructions(workPointer, STOP);
*/
    //stack = createSexpr();
    //env= createSexpr();
    //dump = createSexpr();

// try code from script
    // 1.1
    ld = createSexpr();
    ld->car.instruction = 0;
    ld->car.value = 1;
    workPointer = ld;
    workPointer = addValue(workPointer, 1);

    // 1.2
    ld2 = createSexpr();
    ld2->car.instruction = 0;
    ld2->car.value = 1;
    workPointer = ld2;
    workPointer = addValue(workPointer, 2);

    // 3.2
    ld3 = createSexpr();
    ld3->car.instruction = 0;
    ld3->car.value = 3;
    workPointer = ld3;
    workPointer = addValue(workPointer, 2);

    // 2.1
    ld4 = createSexpr();
    ld4->car.instruction = 0;
    ld4->car.value = 2;
    workPointer = ld4;
    workPointer = addValue(workPointer, 1);

    // 2.2
    ld5 = createSexpr();
    ld5->car.instruction = 0;
    ld5->car.value = 2;
    workPointer = ld5;
    workPointer = addValue(workPointer, 2);

    sel1 = createSexpr();
    sel1->car.instruction = LDC;
    workPointer = sel1;
    workPointer = addValue(workPointer, 1);
    workPointer = addInstructions(workPointer, JOIN);

    sel2 = createSexpr();
    workPointer = sel2;
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld);
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld2);
    workPointer = addInstructions(workPointer, MUL);
    workPointer = addInstructions(workPointer, CONS);
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld3);
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld);
    workPointer = addInstructions(workPointer, SUB);
    workPointer = addInstructions(workPointer, CONS);
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld4);
    workPointer = addInstructions(workPointer, AP);
    workPointer = addInstructions(workPointer, JOIN);

    function2 = createSexpr();
    function2->car.instruction = LDC;
    workPointer = function2;
    workPointer =  addValue(workPointer, 0);
    workPointer = addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld);
    workPointer = addInstructions(workPointer, EQ);
    workPointer = addInstructions(workPointer, SEL);
    workPointer = addCDRList(workPointer, sel1);
    workPointer = addCDRList(workPointer, sel2);
     workPointer = addInstructions(workPointer, RTN);
    
    function3 = createSexpr();
    workPointer = function3;
    workPointer =  addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld5);
    workPointer =  addInstructions(workPointer, CONS);
    workPointer =  addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld4);
    workPointer =  addInstructions(workPointer, CONS);
    workPointer =  addInstructions(workPointer, LD);
    workPointer = addCDRList(workPointer, ld);
    workPointer =  addInstructions(workPointer, AP);
    workPointer =  addInstructions(workPointer, RTN);

    function = createSexpr();
    function->car.instruction = DUM;
    workPointer = function;
    workPointer =  addInstructions(workPointer, NIL);
    workPointer =  addInstructions(workPointer, LDF);
    workPointer = addCDRList(workPointer, function2);
    workPointer =  addInstructions(workPointer, CONS);
    workPointer =  addInstructions(workPointer, LDF);
    workPointer = addCDRList(workPointer, function3);
    workPointer =  addInstructions(workPointer, RAP);
    workPointer =  addInstructions(workPointer, RTN);

  

    control = createSexpr();
    workPointer = control;
    workPointer = addInstructions(workPointer, LDC);
    workPointer = addValue(workPointer, 1);
    workPointer = addInstructions(workPointer, CONS);
    workPointer = addInstructions(workPointer, LDC);
    workPointer = addValue(workPointer, 3);
    workPointer = addInstructions(workPointer, CONS);
    workPointer = addInstructions(workPointer, LDF);
    workPointer = addCDRList(workPointer, function);
    workPointer = addInstructions(workPointer, AP);
    workPointer = addInstructions(workPointer, STOP);

    struct sesecd *secd = (struct sesecd*) malloc(sizeof(struct sesecd));
    secd->s = stack;
    secd->e = env;
    secd->c = control;
    secd->d = dump;

    while(secd->c != NULL) {
        //printSexpr(secd->c);
        execute(secd);
        fflush(stdout);
        
        
    }
   return 0;
}