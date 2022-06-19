%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secd.h"

#define MAX_FUN 1000
#define MAX_PARAMS 100

#define FUNCITON 0
#define CONSTANT 1
%}

%union {unsigned long number; char* id; void* subex;}

%start Program

%token <id> ID_TOK
%token <number> NUM_TOK
%type <subex> Applications

%token MAIN_TOK DEFINITION_TOK ADD_TOK SUB_TOK EQ_TOK LEQ_TOK LE_TOK GEQ_TOK GE_TOK MUL_TOK DIV_TOK AND_TOK OR_TOK PAROPEN_TOK PARCLOSE_TOK LISTOPEN_TOK LISTCLOSE_TOK

%left ADD_TOK

%{
    sexpr* mainFun;
    sexpr* currentEnd;
    sexpr* startExpr;

    void yyerror();
    int yylex(void);

    int numOfFunctions = 0;
    sexpr* definedFunctions[MAX_FUN];
    char *definedFunctionNames[MAX_FUN];

    int numOfPars = 0;
    char* definedParams[MAX_PARAMS];

    void cleanup();
    void genExpr(instruction inst);
    void registerFun(char* name, sexpr* fun);
    void registerParam(char* name);
    sexpr* findFun(char* name);
    sexpr* argumentAdd(sexpr* rawFun, sexpr* argument);
    void genExpr(instruction inst);
    void genPushCon(int value);
    void functionCall(sexpr* calledFunction);
%}

%%

Program : /*nothing*/ {cleanup();}
    | Program Def {cleanup();}
    ;

Def : MAIN_TOK DEFINITION_TOK Expr {genExpr(STOP); mainFun = startExpr;}
    | ID_TOK Pars DEFINITION_TOK Expr {
        genExpr(RTN);
        registerFun($1, startExpr);
        printSexpr(startExpr);
        numOfPars = 0;}
    ;

Pars : /*nothing*/
    | Pars ID_TOK {registerParam($2);}
    ;

Expr: Term
    | Term ADD_TOK Term {genExpr(ADD);}
    | Term SUB_TOK Term
    | Term EQ_TOK Term
    | Term LEQ_TOK Term
    | Term LE_TOK Term
    | Term GEQ_TOK Term
    | Term GE_TOK Term
    | Term MUL_TOK Term
    | Term DIV_TOK Term
    | Term AND_TOK Term
    | Term OR_TOK Term
    ;

Term : NUM_TOK {genPushCon($1);}
    | Applications {functionCall($1);}
    | PAROPEN_TOK Expr PARCLOSE_TOK
    ;

Applications : ID_TOK {$$ = findFun($1);}
    | Applications ID_TOK {$$ = argumentAdd($1, findFun($2));}
    ;

%%

void yyerror(void){
    printf("An error occured\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
}

int main(void){
    memset(definedFunctionNames, 0, sizeof(char*) * MAX_FUN);

    currentEnd = createSexpr();
    currentEnd->car.instruction = NIL;
    currentEnd-> cdr = NULL;

    startExpr = currentEnd;

    yyparse();

    printf("finished parsing\n");

    printSexpr(mainFun);
    printf("\n");
    
    sesecd dummy;

    struct sexpr *nil = (struct sexpr*) malloc(sizeof(struct sexpr));
    nil->car.instruction = NIL;
    sexpr* nilExpr = consLL(nil,NULL);

    dummy.s = nilExpr;
    dummy.e = nilExpr;
    dummy.c = mainFun;
    dummy.d = nilExpr;

    int i = 0;
    while(dummy.c != NULL){
        execute(&dummy);
        printf("after %d steps:\ns:\n", ++i);

        printSexpr(dummy.s);
        printf("\ne:\n");
        printSexpr(dummy.e);
        printf("\nc:\n");
        printSexpr(dummy.c);
        printf("\nd:\n");
        printSexpr(dummy.d);
        printf("\n");
    }

    return 0;
}

void cleanup(){
    currentEnd = createSexpr();
    currentEnd->car.instruction = NIL;
    currentEnd-> cdr = NULL;
    startExpr = currentEnd;
    numOfPars = 0;
}

void registerFun(char* name, sexpr* fun){
    definedFunctionNames[numOfFunctions] = name;

    sexpr* type = createSexpr();
    type->car.value = FUNCITON;

    sexpr* base = createSexpr();
    base->car.list = createSexpr();
    base->car.list->car.list = NULL;
    base->car.list->cdr = NULL;
    base->cdr = fun;

    type->cdr = base;
    definedFunctions[numOfFunctions++] = type;
}

void registerParam(char* name){
    definedParams[numOfPars++] = name;
}

sexpr* findFun(char* name){
    for(int i = 0; i < numOfPars; i++){
        if(!strcmp(name, definedParams[i])){
            sexpr* locp = createSexpr();
            locp->car.value = (numOfPars -i) + 1;

            sexpr* locl = createSexpr();
            locl->car.value = 1;
            locl->cdr = locp;

            sexpr* ldInst = createSexpr();
            ldInst->car.list = locl;

            sexpr* type = createSexpr();//da müsst man halt function oder con rein tun können
            type->car.value = CONSTANT;
            type->cdr=ldInst;

            return type;
        }
    }

    for(int i = 0; i < numOfFunctions; i++){
        if(!strcmp(name, definedFunctionNames[i])){
            return definedFunctions[i];
        }
    }

    printf("Function %s is not defined\n", name);
    fflush(stdout);
    exit(EXIT_FAILURE);
}

void genPushCon(int con){
    sexpr* loader = createSexpr();
    loader->car.instruction = LDC;

    sexpr* value = createSexpr();
    value->car.value = con;
    value->cdr = NULL;

    loader->cdr = value;

    currentEnd->cdr = loader;
    currentEnd = value;
}

void genExpr(instruction inst){
    sexpr* inExpr = createSexpr();
    inExpr->car.instruction = inst;
    inExpr->cdr = NULL;

    currentEnd->cdr = inExpr;
    currentEnd = inExpr;
}

sexpr* argumentAdd(sexpr* rawFun, sexpr* argument){
    sexpr* env = rawFun->cdr->car.list;
    sexpr* fun = rawFun->cdr->cdr;

    sexpr* newElem = createSexpr();
    newElem->car = argument->car;
    newElem->cdr = env;

    sexpr* newRoot = createSexpr();
    newRoot->car.list = newElem;
    newRoot->cdr = fun;

    sexpr* newType = createSexpr();
    newType->car.value = FUNCITON;
    newType->cdr = newRoot;

    return newType;
}

void functionCall(sexpr* callRecord){
    if(callRecord->car.value == CONSTANT){
        genPushCon(callRecord->cdr->car.value);
        return;
    }

    sexpr* env = callRecord->cdr->car.list;
    sexpr* calledFunction = callRecord->cdr->cdr;

    sexpr* loadEnv = createSexpr();
    loadEnv->car.instruction = LDC;

    sexpr* constructedEnv = createSexpr();
    constructedEnv->car.list = env;

    sexpr* loadExpr = createSexpr();
    loadExpr->car.instruction = LDF;

    sexpr* loadFun = createSexpr();
    loadFun->car.list = calledFunction;
    
    sexpr* apFun = createSexpr();
    apFun->car.instruction = AP;
    apFun->cdr = NULL;

    loadEnv->cdr = constructedEnv;
    constructedEnv->cdr = loadExpr;
    loadExpr->cdr = loadFun;
    loadFun->cdr = apFun;

    currentEnd->cdr = loadEnv;
    currentEnd = apFun;
}