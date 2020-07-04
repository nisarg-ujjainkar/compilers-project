#ifndef AST_H
#define AST_H

#include<stddef.h>
#include"table.h"

struct Expression
{
    struct AST *left, *right;
    char Operator;
};
struct Assignment
{
    symrec *variable;
    struct AST *rhs;
};
struct While
{
    struct AST *cond;
    struct AST *code;
};
struct If
{
    struct AST *cond;
    struct AST *code;
};
struct IfElse
{
    struct AST *cond;
    struct AST *codeif;
    struct AST *codeelse;
};
struct condJoin
{
    struct AST *left;
    struct AST *right;
    char *op;
};
struct cond
{
    struct AST *left;
    struct AST *right;
    char *op;
};
struct AST
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
        struct Expression Expression;
        struct Assignment Assignment;
        struct While While;
        struct If If;
        struct IfElse IfElse;
        struct condJoin condJoin;
        struct cond cond;
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
AST* genAssignment(symrec *var, AST *rhs);
AST* genWhile(AST *cond, AST *code);
AST* genIf(AST *cond, AST *code);
AST* genIfElse(AST *cond, AST *ifCode, AST *elseCode);
AST* genCond(AST *left, AST *right, char *op);
AST* genCondJoin(AST *left, AST *right, char *op);
AST* genLine(AST *head, AST *next);

#endif