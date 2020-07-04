#include"ast.h"
#include"table.h"
#include<stdio.h>
#include<stdlib.h>

AST* genVariable(symrec *var)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_var;
    node->node.variable=var;
    return node;
}

AST* genNumber(double val)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_num;
    node->node.val=val;
    return node;
}

AST* genExpression(AST *left, AST *right, char op)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_Exp;
    node->node.Expression.left=left;
    node->node.Expression.right=right;
    node->node.Expression.Operator=op;
    return node;
}

AST* genAssignment(symrec *var, AST *rhs)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_Assn;
    node->node.Assignment.rhs=rhs;
    node->node.Assignment.variable=var;
    return node;
}

AST* genWhile(AST *cond, AST *code)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_WhileStm;
    node->node.While.code=code;
    node->node.While.cond=cond;
    return node;
}

AST* genIf(AST *cond, AST *code)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_IfStm;
    node->node.If.code=code;
    node->node.If.cond=cond;
    return node;
}

AST* genIfElse(AST *cond, AST *ifCode, AST *elseCode)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_IfElse;
    node->node.IfElse.codeelse=elseCode;
    node->node.IfElse.codeif=ifCode;
    node->node.IfElse.cond=cond;
    return node;
}

AST* genCondJoin(AST *left, AST *right, char *op)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_condJoin;
    node->node.condJoin.left=left;
    node->node.condJoin.right=right;
    node->node.condJoin.op=op;
    return node;
}

AST* genCond(AST *left, AST *right, char *op)
{
    AST *node=malloc(sizeof(AST));
    node->Kind=A_cond;
    node->node.cond.left=left;
    node->node.cond.right=right;
    node->node.cond.op=op;
    return node;
}

AST* genLine(AST *head, AST *next)
{
    if(!head)
    {
        head=malloc(sizeof(AST));
        head->Kind=A_line;
        head->node.next.count=0;
        head->node.next.next=NULL;
    }
    head->node.next.count++;
    head->node.next.next=realloc(head->node.next.next,head->node.next.count*sizeof(AST*));
    head->node.next.next[head->node.next.count-1]=next;
    return head;
}