#include"ast.h"
#include"table.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>


AST::AST* genVariable(symrec *var)
{
    printf("generating Variablr\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_var;
    node->node.variable=var;
    return node;
}

AST::AST* genNumber(double val)
{
    printf("generating Number\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_num;
    node->node.val=val;
    return node;
}

AST::AST* genExpression(AST::AST *left, AST::AST *right, char op)
{
    printf("generating Expression\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_Exp;
    node->node.Expression.left=left;
    node->node.Expression.right=right;
    node->node.Expression.Operator=op;
    return node;
}

AST::AST* genAssignment(symrec *var, AST::AST *rhs)
{
    printf("generating Assignmentn");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_Assn;
    node->node.Assignment.rhs=rhs;
    node->node.Assignment.variable=var;
    return node;
}

AST::AST* genWhile(AST::AST *cond, AST::AST *code)
{
    printf("generating While\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_WhileStm;
    node->node.While.code=code;
    node->node.While.cond=cond;
    return node;
}

AST::AST* genIf(AST::AST *cond, AST::AST *code)
{
    printf("generating If\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_IfStm;
    node->node.If.code=code;
    node->node.If.cond=cond;
    return node;
}

AST::AST* genIfElse(AST::AST *cond, AST::AST *ifCode, AST::AST *elseCode)
{
    printf("generating If Else\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_IfElse;
    node->node.IfElse.codeelse=elseCode;
    node->node.IfElse.codeif=ifCode;
    node->node.IfElse.cond=cond;
    return node;
}

AST::AST* genCondJoin(AST::AST *left, AST::AST *right, char *op)
{
    printf("generating Cond Join\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_condJoin;
    node->node.condJoin.left=left;
    node->node.condJoin.right=right;
    node->node.condJoin.op=op;
    return node;
}

AST::AST* genCond(AST::AST *left, AST::AST *right, char *op)
{
    printf("generating Cond\n");
    // AST::AST *node=(AST::AST*)malloc(sizeof(AST::AST));
    AST::AST *node = new AST::AST;
    node->Kind=AST::AST::A_cond;
    node->node.cond.left=left;
    node->node.cond.right=right;
    node->node.cond.op=op;
    return node;
}

