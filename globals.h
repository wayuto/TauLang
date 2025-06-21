#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#define int long long

extern int debug;
extern int assembly;
extern int index_of_bp;
extern int basetype;
extern int expr_type;
extern int *idmain;
extern int token_val;
extern int *current_id, *symbols;
extern int token;
extern int pool_size;
extern int line;
extern int *text, *old_text, *stack;
extern char *data;
extern char *src, *old_src;
extern int *pc, *bp, *sp, ax, cycle;

enum {
    LEA,
    IMM,
    JMP,
    CALL,
    JZ,
    JNZ,
    ENT,
    ADJ,
    LEV,
    LI,
    LC,
    SI,
    SC,
    PUSH,
    OR,
    XOR,
    AND,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    SHL,
    SHR,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    OPEN,
    READ,
    CLOS,
    PRTF,
    MALC,
    MSET,
    MCMP,
    EXIT
};

enum {
    Num = 128,
    Fun,
    Sys,
    Glo,
    Loc,
    Id,
    Char,
    Else,
    Enum,
    If,
    Int,
    Return,
    Sizeof,
    While,
    Assign,
    Cond,
    Lor,
    Lan,
    Or,
    Xor,
    And,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Shl,
    Shr,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Inc,
    Dec,
    Brak
};

enum {
    Token,
    Hash,
    Name,
    Type,
    Class,
    Value,
    BType,
    BClass,
    BValue,
    IdSize
};

enum {
    CHAR,
    INT,
    PTR
};

struct identifier {
    int token;
    int hash;
    char *name;
    int class;
    int type;
    int value;
    int Bclass;
    int Btype;
    int Bvalue;
};

extern int eval();

extern void next();

extern void expression();

extern void match(int tk);

extern void next();

extern void statement();

extern void global_declaration();

extern void enum_declaration();

extern void function_declaration();

extern void function_body();

extern void function_parameter();

void error(const char *fmt, ...);

#endif // GLOBALS_H
