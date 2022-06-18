%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secd.h"

#define MAX_FUN 1000
%}

%union {unsigned long number; char* id;}

%start Program

%token <id> ID_TOK
%token <number> NUM_TOK

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

    void cleanup();
    void genExpr(instruction inst);
    void registerFun(char* name, sexpr* fun);
    sexpr* findFun(char* name);
    void genExpr(instruction inst);
    void genPushCon(int value);
    void functionCall(sexpr* calledFunction);
%}

%%

Program : /*nothing*/ {cleanup();}
    | Program Def {cleanup();}
    ;

Def : MAIN_TOK DEFINITION_TOK Expr {genExpr(STOP); mainFun = startExpr;}
    | ID_TOK DEFINITION_TOK Expr {genExpr(RTN);registerFun($1, startExpr);printSexpr(startExpr);}
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
    | ID_TOK {functionCall(findFun($1));}
    | PAROPEN_TOK Expr PARCLOSE_TOK
    ;

%%;

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

    dummy.s = NULL;
    dummy.e = NULL;
    dummy.c = mainFun;
    dummy.d = NULL;

    while(dummy.c != NULL){
        execute(&dummy);
    }

    return 0;
}

void cleanup(){
    currentEnd = createSexpr();
    currentEnd->car.instruction = NIL;
    currentEnd-> cdr = NULL;
    startExpr = currentEnd;
}

void registerFun(char* name, sexpr* fun){
    definedFunctionNames[numOfFunctions] = name;
    definedFunctions[numOfFunctions++] = fun;
}

sexpr* findFun(char* name){
    for(int i = 0; i < numOfFunctions; i++){
        fflush(stdout);
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

void functionCall(sexpr* calledFunction){
    sexpr* loadExpr = createSexpr();
    loadExpr->car.instruction = LDF;

    sexpr* loadFun = createSexpr();
    loadFun->car.list = calledFunction;
    
    sexpr* applyFun = createSexpr();
    applyFun->car.instruction = AP;
    applyFun->cdr = NULL;

    loadExpr->cdr = loadFun;
    loadFun->cdr = applyFun;

    currentEnd->cdr = loadExpr;
    currentEnd = applyFun;
}