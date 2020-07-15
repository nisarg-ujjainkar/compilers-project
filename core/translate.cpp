#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include"translate.hpp"
#include"table.hpp"
#include<string.h>
#include<string>
#include<iomanip>

using namespace std;
vector<string> instructions;
vector<string> whileStack;
vector<string> ifStack;
map<symrec*,int> location;
int whileCount=0;

void linearise(std::vector<AST*>source,std::vector<AST*>&dest)  //this is working
{
    int count=0;
    bool foundBlock=false;
    for (auto a:source)
    {
        count=0;
        if(a->Kind==AST::A_IfStm || a->Kind==AST::A_IfElse || a->Kind==AST::A_WhileStm)
        {
            count++;
            for (auto it=dest.begin();it!=dest.end();it++)
            {
                if(a->Kind==AST::A_WhileStm)
                {
                    auto tmp=a->node.While.code;
                    // cout<<"While block"<<endl;
                    // cout<<tmp->Kind<<endl;
                    if(a->node.While.code==*it)
                    {
                        foundBlock=true;
                        dest.insert(it,a);
                        break;
                    }  
                }
                if(a->Kind==AST::A_IfStm)
                {
                    if(a->node.If.code==*it)
                    {
                        foundBlock=true;
                        dest.insert(it,a);
                        break;
                    }
                }
                if(a->Kind==AST::A_IfElse)
                {
                    if(a->node.IfElse.codeif==*it)
                    {
                        foundBlock=true;
                        dest.insert(it,a);
                        break;
                    }
                }
            }
            if(foundBlock==false)
                dest.push_back(a);
        }
        else
        {
            dest.push_back(a);
        }
    }
}

string allocRegToVar(symrec *var)
{
    cout<<"Allocating Reg to var"<<endl;
    int loc=location[var];
    string ins="\tMOV ";
    ins.append("r1, #");
    stringstream stream;
    stream<<"0x"<<hex<<loc;
    ins.append(stream.str());
    instructions.push_back(ins);
    ins.clear();
    ins="\tLDR r2, [r1]";
    instructions.push_back(ins);
    return "r2";
}

string allocRegToNum(double val)
{
    cout<<"allocating reg to num"<<endl;
    string ins="\tMOV r3, #";
    ins.append(to_string(val));
    instructions.push_back(ins);
    return "r3";
}

string trExpression(AST *node)
{
    cout<<"Translating Expression"<<endl;
    if(node->Kind==AST::A_var)
    {
        cout<<"\texpression goes to var"<<endl;
        string reg=allocRegToVar(node->node.variable);
        return reg;
    }
    else if(node->Kind==AST::A_num)
    {
        cout<<"\texpression goes to num"<<endl;
        string reg=allocRegToNum(node->node.val);
        return reg;
    }
    else
    {
        cout<<"This fucker is complex"<<endl;
        auto regLeft=trExpression(node->node.Expression.left);
        if(regLeft=="r0")
        {
            string ins="\tADD r1,r0,#0";
            instructions.push_back(ins);
            regLeft="r1";
        }
        auto regRight=trExpression(node->node.Expression.right);
        switch (node->node.Expression.Operator)
        {
        case '+':
        {
            string ins="\tADD r0, ";
            ins.append(regLeft);
            ins.append(",");
            ins.append(regRight);
            instructions.push_back(ins);
        }
        case '-':
        {
            if(regLeft=="nr")
            {
                string ins="\tRSB r0, ";
                ins.append(regRight);
                ins.append(",#0");
                instructions.push_back(ins);
            }
            else
            {
                string ins="\tSUB r0, ";
                ins.append(regLeft);
                ins.append(",");
                ins.append(regRight);
                instructions.push_back(ins);
            }
        }
        case '*':
        {
            string ins="\tMUL r0, ";
            ins.append(regLeft);
            ins.append(",");
            ins.append(regRight);
            instructions.push_back(ins);
        }
        case '/':
        {
            string ins="\tSDIV r0, ";
            ins.append(regLeft);
            ins.append(",");
            ins.append(regRight);
            instructions.push_back(ins);
        }
        default:
        {
            cout<<"Wrong Operator in the expression"<<endl;
            exit(1);
        }
        }
    }
    return "r0";
}

string trCond(AST *cond)
{
    cout<<"translating cond"<<endl;
    cout<<"\tcond ha type "<<cond->Kind<<endl;
    cout<<"\tleft has kind "<<cond->node.cond.left->Kind<<endl;
    cout<<"\tright has kind "<<cond->node.cond.right->Kind<<endl;
    exit(1);
    auto regLeft=trExpression(cond->node.cond.left);
    if(regLeft=="r0")
    {
        string ins="\tADD r1,r0,#0";
        instructions.push_back(ins);
        regLeft="r1";
    }
    auto regRight=trExpression(cond->node.cond.right);
    string ins="\tCMP ";
    ins.append(regLeft);
    ins.append(",");
    ins.append(regRight);
    instructions.push_back(ins);
    string ret;
    if(strcmp(cond->node.cond.op,">=")==0)
        // condStack.push_back("bge ");
        ret="\tBGE ";
    else if(strcmp(cond->node.cond.op,"<=")==0)
        // condStack.push_back("ble ");
        ret="\tBLE ";
    else if(strcmp(cond->node.cond.op,"==")==0)
        // condStack.push_back("beq ");
        ret="\tBEQ ";
    else if(strcmp(cond->node.cond.op,"!=")==0)
        // condStack.push_back("bne ");
        ret="\tBNE ";
    else if(strcmp(cond->node.cond.op,">")==0)
        // condStack.push_back("bgt ");
        ret="\tBGT ";
    else if(strcmp(cond->node.cond.op,"<")==0)
        // condStack.push_back("blt ");
        ret="\tBLT ";
    else
    {
        cout<<"Wrong operator in cond"<<endl;
        exit(1);
    }
    return ret;
}

void trWhile(AST *While, AST *next)
{
    cout<<"translating while"<<endl;
    ++whileCount;
    string tmp="while";
    tmp.append(to_string(whileCount));
    instructions.push_back(tmp);
    whileStack.push_back(tmp);
    tmp.clear();
    string ins=trCond(While->node.While.cond);
    tmp="end_while";
    // ins.append("end_while");
    tmp.append(to_string(whileCount));
    ins.append(tmp);
    instructions.push_back(ins);
    whileStack.push_back(tmp);
    if(While->node.While.code!=next)
    {
        if(While->node.While.code->Kind==AST::A_Assn)
            trAssignment(While->node.While.code);
        else if(While->node.While.code->Kind==AST::A_IfStm)
            trIf(While->node.While.code);
        else if(While->node.While.code->Kind==AST::A_IfElse)
            trIfElse(While->node.While.code);
        else
        {
            cout<<"There is some problem here. Exiting"<<endl;
            exit(1);
        }
    }
}

void trAssignment(AST *assn)
{
    cout<<"translating assignment"<<assn->Kind<<endl;
    cout<<"\trhs has type "<<assn->node.Assignment.rhs->Kind<<endl;
    // exit(1);
    int loc;
    string destReg;
    if(location.find(assn->node.Assignment.variable)->second==0)
    {
        int temp=1000;
        if(location.empty()==false)
        {
            temp=location.rbegin()->second+4;
        }
        loc=temp;
        location[assn->node.Assignment.variable]=loc;
    }
    else
    {
        loc=location.find(assn->node.Assignment.variable)->second;
    }
    auto regRight=trExpression(assn->node.Assignment.rhs);
    if(regRight=="r3")
        destReg="r1";
    else
        destReg="r3";
    string ins="\tMOV ";
    ins.append(destReg);
    ins.append(", #");
    stringstream stream;
    stream<<"0x"<<hex<<loc;
    ins.append(stream.str());
    instructions.push_back(ins);
    ins.clear();
    ins="\tSTR ";
    ins.append(regRight);
    ins.append(", ");
    ins.append("[");
    ins.append(destReg);
    ins.append("]");
    instructions.push_back(ins);
}

void trIf(AST *If)
{
    return;
}

void trIfElse(AST *IfElse)
{
    return;
}

void TranslatorMain(vector<AST*>head,vector<AST*>dest)
{
    if(dest.size()==0)
    {
        return;
    }
    auto it1=*(head.begin());
    auto it2=*(dest.begin());
    cout<<it2->Kind<<endl;
    
    if(it2->Kind==AST::A_WhileStm)
    {
        cout<<"before deref"<<endl;
        cout<<"\tCond has type "<<it2->node.While.cond->Kind<<endl;
    }
    head.erase(head.begin());
    dest.erase(dest.begin());
    
    if(it2->Kind==AST::A_WhileStm)
    {
        cout<<"after deref"<<endl;
        cout<<"\tCond has type "<<it2->node.While.cond->Kind<<endl;
        exit(1);
    }
    switch (it2->Kind)
    {
    case AST::A_Assn:
        trAssignment(it2);
    case AST::A_WhileStm:
        trWhile(it2,it1);
    case AST::A_IfStm:
        break;
    case AST::A_IfElse:
        break;
    default:
        break;
    }
    if(it1->Kind==AST::A_WhileStm)
    {
        auto tmp1=*(whileStack.rbegin());
        whileStack.pop_back();
        auto tmp2=*(whileStack.rbegin());
        whileStack.pop_back();
        string ins="\t b ";
        ins.append(tmp2);
        instructions.push_back(ins);
        ins.clear();
        instructions.push_back(tmp1);
    }
}