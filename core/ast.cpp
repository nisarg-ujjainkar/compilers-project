#include"ast.hpp"
#include"table.hpp"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>

using namespace std;

AST* genVariable(symrec *var)
{
    printf("generating Variablr\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_var;
    node->node.variable=var;
    return node;
}

AST* genNumber(double val)
{
    printf("generating Number\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_num;
    node->node.val=val;
    return node;
}

AST* genExpression(AST *left, AST *right, char op)
{
    printf("generating Expression\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_Exp;
    node->node.Expression.left=left;
    node->node.Expression.right=right;
    node->node.Expression.Operator=op;
    return node;
}

AST* genAssignment(symrec *var, AST *rhs)
{
    printf("generating Assignment\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_Assn;
    node->node.Assignment.rhs=rhs;
    node->node.Assignment.variable=var;
    return node;
}

AST* genWhile(AST *cond, AST *code)
{
    printf("generating While\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    cout<<"\tcond has type "<<cond->Kind<<endl;
    cout<<"\tcode has type "<<code->Kind<<endl;
    AST *node = new AST;
    node->Kind=AST::A_WhileStm;
    node->node.While.code=code;
    node->node.While.cond=cond;
    return node;
}

AST* genIf(AST *cond, AST *code)
{
    printf("generating If\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_IfStm;
    node->node.If.code=code;
    node->node.If.cond=cond;
    return node;
}

AST* genIfElse(AST *cond, AST *ifCode, AST *elseCode)
{
    printf("generating If Else\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_IfElse;
    node->node.IfElse.codeelse=elseCode;
    node->node.IfElse.codeif=ifCode;
    node->node.IfElse.cond=cond;
    return node;
}

AST* genCondJoin(AST *left, AST *right, char *op)
{
    printf("generating Cond Join\n");
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_condJoin;
    node->node.condJoin.left=left;
    node->node.condJoin.right=right;
    node->node.condJoin.op=op;
    return node;
}

AST* genCond(AST *left, AST *right, char *op)
{
    printf("generating Cond with operator ");
    cout<<op<<endl;
    cout<<"\tleft has type "<<left->Kind<<endl;
    cout<<"\tright has type "<<right->Kind<<endl;
    // AST *node=(AST*)malloc(sizeof(AST));
    AST *node = new AST;
    node->Kind=AST::A_cond;
    node->node.cond.left=left;
    node->node.cond.right=right;
    node->node.cond.op=op;
    return node;
}

