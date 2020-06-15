#ifndef AST_H
#define AST_H

#include<stddef.h>
#include"table.h"

typedef struct AST
{
    enum kind
    {
        Assn,
        Exp,
        WhileStm,
        IfStm
    }Kind;
    double val;
    symrec *variable;
    typedef struct Expression
    {
        struct AST *left, *right;
        char *Operator;
    }Expression;
    typedef struct Assignment
    {
        symrec *variable;
        struct AST *rhs;
    }Assignment;
    typedef struct While
    {
        struct AST *cond;
        struct AST *code;
    }While;
    typedef struct If
    {
        struct AST *cond;
        struct AST *code;
    }If;
    typedef struct IfElse
    {
        struct AST *cond;
        struct AST *codeif;
        struct AST *codeelse;
    }IfElse;
}AST;

AST* genVariable(symrec *var);
AST* genNumber(double val);
AST* genExpression(AST *left, AST *right, char op);
AST* genAssignmennt(symrec *var, AST *val);
AST* genWhile(AST *cond, AST *code);
AST* genIf(AST *cond, AST *ifCode, AST *elseCode);

#endif