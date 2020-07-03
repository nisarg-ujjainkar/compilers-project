#ifndef AST_H
#define AST_H

#include<stddef.h>
#include"table.h"

typedef struct AST
{
    enum kind
    {
        A_var,
        A_num,
        A_Assn,
        A_Exp,
        A_WhileStm,
        A_IfStm,
        A_IfElse,
        A_line,
        A_cond,
        A_condJoin
    }Kind;
    union
    {
        double val;
        symrec *variable;
        struct
        {
            struct AST *left, *right;
            char *Operator;
        }Expression;
        struct
        {
            symrec *variable;
            struct AST *rhs;
        }Assignment;
        struct
        {
            struct AST *cond;
            struct AST *code;
        }While;
        struct
        {
            struct AST *cond;
            struct AST *code;
        }If;
        struct
        {
            struct AST *cond;
            struct AST *codeif;
            struct AST *codeelse;
        }IfElse;
        struct
        {
            struct AST *left;
            struct AST *right;
            char *op;
        }condJoin;
        struct
        {
            struct AST *left;
            struct AST *right;
            char *op;
        }cond;
        struct
        {
            int count;
            struct AST **next;
        }next;
    }node;
};

typedef struct AST AST;

AST* genVariable(symrec *var);
AST* genNumber(double val);
AST* genExpression(AST *left, AST *right, char op);
AST* genAssignmennt(symrec *var, AST *rhs);
AST* genWhile(AST *cond, AST *code);
AST* genIf(AST *cond, AST *code);
AST* genIfElse(AST *cond, AST *ifCode, AST *elseCode);
AST* genCond(AST *left, AST *right, char *op);
AST* genCondJoin(AST *left, AST *right, char *op);
AST* genLine(AST *head, AST *next);

#endif