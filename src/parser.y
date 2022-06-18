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

%token DEFINITION_TOK ADD_TOK SUB_TOK EQ_TOK LEQ_TOK LE_TOK GEQ_TOK GE_TOK MUL_TOK DIV_TOK AND_TOK OR_TOK PAROPEN_TOK PARCLOSE_TOK LISTOPEN_TOK LISTCLOSE_TOK

%left ADD_TOK

%{
    typedef struct exprStore{
        sexpr* start;
        sexpr* end;
    } exprStore;

    sexpr* currentEnd;
    sexpr* startExpr;

    void yyerror();
    int yylex(void);
    sexpr definedFunctions[MAX_FUN];
    char *definedFunctionNames[MAX_FUN];

    void genAdd();
    void genStop();
    void genPushCon(int value);
%}

%%

Program : /*nothing*/
    | Program Def
    ;

Def : ID_TOK DEFINITION_TOK Expr {genStop();}
    ;

Expr: Term
    | Term ADD_TOK Term {genAdd();}
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

    printSexpr(startExpr);
    printf("\n");
    
    sesecd dummy;

    dummy.s = NULL;
    dummy.e = NULL;
    dummy.c = startExpr;
    dummy.d = NULL;

    while(dummy.c != NULL){
        execute(&dummy);
    }

    return 0;
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

void genAdd(){
    sexpr* add = createSexpr();
    add->car.instruction = ADD;
    add->cdr = NULL;

    currentEnd->cdr = add;
    currentEnd = add;
}

void genRet(){
    sexpr* ret = createSexpr();
    ret->car.instruction = RTN;
    ret->cdr = NULL;

    currentEnd->cdr = ret;
    currentEnd = ret;
}

void genStop(){

    sexpr* stop = createSexpr();
    stop->car.instruction = STOP;
    stop->cdr = NULL;

    currentEnd->cdr = stop;
}