%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secd.h"

#define MAX_FUN 1000
#define MAX_PARAMS 100

%}

%union {unsigned long number; char* id; void* subex;}

%start Program

%token <id> ID_TOK
%token <number> NUM_TOK
%type <subex> Applications

%token MAIN_TOK NEWLINE_TOK DEFINITION_TOK 
%token ADD_TOK SUB_TOK EQ_TOK LEQ_TOK LE_TOK GEQ_TOK GE_TOK MUL_TOK DIV_TOK AND_TOK OR_TOK PAROPEN_TOK PARCLOSE_TOK LISTOPEN_TOK LISTCLOSE_TOK
%token IF_TOK THEN_TOK ELSE_TOK

%left ADD_TOK

%{
    sexpr* mainFun;
    sexpr* currentEnd;
    sexpr* startExpr;

    sexpr* endDump = NULL;

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
    void loadFun(char* name);
    void genExpr(instruction inst);
    void genPushCon(int value);
    void pushExpr(sexpr* expr);

    void makeCodeVert();
    void makeCodeStraight();
%}

%%

Program : /*nothing*/ {cleanup();}
    | Program Def {cleanup();}
    ;

Def : MAIN_TOK DEFINITION_TOK Expr NEWLINE_TOK {genExpr(STOP); mainFun = startExpr;}
    | ID_TOK {registerFun($1,startExpr);} Pars DEFINITION_TOK Expr NEWLINE_TOK {
        genExpr(RTN);
        printSexpr(startExpr);
        printf("\n");
        numOfPars = 0;}
    ;

Pars : /*nothing*/
    | Pars ID_TOK {registerParam($2);}
    ;

Expr: Term
    | Term ADD_TOK Term {genExpr(ADD);}
    | Term SUB_TOK Term {genExpr(SUB);}
    | Term EQ_TOK Term {genExpr(EQ);}
    | Term LEQ_TOK Term {genExpr(LEQ);}
    | Term LE_TOK Term  {genExpr(LE);}
    | Term GEQ_TOK Term {genExpr(GEQ);}
    | Term GE_TOK Term {genExpr(GE);}
    | Term MUL_TOK Term {genExpr(MUL);}
    | Term DIV_TOK Term {genExpr(DIV);}
    | Term AND_TOK Term {genExpr(AND);}
    | Term OR_TOK Term {genExpr(OR);}
    | IF_TOK 
        Term THEN_TOK {genExpr(SEL);makeCodeVert();}
        Term ELSE_TOK {makeCodeStraight();makeCodeVert();}
        Term {makeCodeStraight();}
    ;

Term : NUM_TOK {genPushCon($1);}
    | Applications 
    | PAROPEN_TOK Expr PARCLOSE_TOK
    ;

Applications : ID_TOK {loadFun($1);}
    | Applications Apex {genExpr(SPECPAR);}
    ;

Apex: ID_TOK {loadFun($1);}
    | NUM_TOK {genPushCon($1);}
    | PAROPEN_TOK Expr PARCLOSE_TOK
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

    sexpr* env = createSexpr();
    env->car.list = NULL;
    env->cdr = NULL;

    sexpr* root = createSexpr();
    root->type = FUNCTION;
    root->car.list = env;
    root->cdr = fun;

    printf("written fun %s on root %d\n", name, root);
    
    definedFunctions[numOfFunctions++] = root;
}

void registerParam(char* name){
    definedParams[numOfPars++] = name;
}

void loadFun(char* name){
    for(int i = 0; i < numOfPars; i++){
        if(!strcmp(name, definedParams[i])){
            sexpr* locp = createSexpr();
            locp->car.value = (numOfPars - i);

            sexpr* locl = createSexpr();
            locl->car.value = 1;
            locl->cdr = locp;

            sexpr* container = createSexpr();
            container->car.list = locl;

            sexpr* ldInst = createSexpr();
            ldInst->car.instruction = LD;
            ldInst->cdr = container;

            currentEnd->cdr = ldInst;
            currentEnd = container;
            return;
        }
    }

    for(int i = 0; i < numOfFunctions; i++){
        if(!strcmp(name, definedFunctionNames[i])){
            sexpr* container = createSexpr();
            container->car.list = definedFunctions[i];

            sexpr* loader = createSexpr();
            loader->car.instruction = LDC;
            loader->cdr = container;

            currentEnd->cdr = loader;
            currentEnd = container;
            return;
        }
    }

    printf("Function %s is not defined\n", name);
    fflush(stdout);
    exit(EXIT_FAILURE);
}

void genPushCon(int con){
    sexpr* value = createSexpr();
    value->type = CONSTANT;
    value->car.value = con;
    value->cdr = NULL;

    sexpr* container = createSexpr();
    container->car.list = value;

    sexpr* loader = createSexpr();
    loader->car.instruction = LDC;
    loader->cdr = container;

    currentEnd->cdr = loader;
    currentEnd = container;
}

void genExpr(instruction inst){
    sexpr* inExpr = createSexpr();
    inExpr->car.instruction = inst;
    inExpr->cdr = NULL;

    currentEnd->cdr = inExpr;
    currentEnd = inExpr;
}

void pushExpr(sexpr* expr){
    sexpr* content = createSexpr();
    content->car.list = expr;

    sexpr* loader = createSexpr();
    loader->car.value = LDC;
    loader->cdr = content;

    currentEnd->cdr = loader;
    currentEnd = content;
}

void makeCodeStraight(){
    currentEnd->cdr = createSexpr();
    currentEnd->cdr->car.instruction = JOIN;

    currentEnd = endDump;
    endDump = endDump->cdr;
}

void makeCodeVert(){
    //create new end and register it
    currentEnd->cdr = createSexpr();
    currentEnd = currentEnd->cdr;

    //register end endDump
    currentEnd->cdr = endDump;
    endDump = currentEnd;

    //create the vertical list, and register it as new end
    currentEnd->car.list = createSexpr();
    currentEnd->car.list->car.instruction = NIL;
    currentEnd = currentEnd->car.list;
}