#ifndef SECD_H
#define SECD_H


typedef enum {
    NIL,
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
    AND,
    OR,
    SEL,
    JOIN,
    LDF,
    AP,
    RTN,
    DUM,
    RAP,
    STOP
} instruction;

#endif
