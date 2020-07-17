#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<iterator>
#include"translate.hpp"
#include"table.hpp"
#include<string.h>
#include<string>
#include<iomanip>
#include<algorithm>

using namespace std;
vector<string> instructions;
vector<string> whileStack;
vector<string> ifStack;
vector<string> AccessPattern;
map<symrec*,int> location;
map<string,bool> regStatus;
map<string,symrec*> varReg;
int whileCount=0;
int ifCount=0;


void init()
{
    string str;
    for (int i=2;i<13;i++)
    {
        str="r";
        str.append(to_string(i));
        regStatus[str]=false;
        if(i>3)
            varReg[str]=NULL;
    }
}


void filterExp(vector<AST*>source,vector<AST*>&dest)  //this is also working
{
    vector<vector<AST*>::iterator> toRem;
    for(auto a:source)
    {
        if(a->Kind==AST::A_Exp)
        {
            for(auto it=source.begin();*it!=a;++it)
            {
                if(*it==a->node.Expression.left)
                {
                    toRem.push_back(it);
                    // cout<<"found Left"<<endl;
                }
            }
            for(auto it=source.begin();*it!=a;++it)
            {
                if(*it==a->node.Expression.right)
                {
                    toRem.push_back(it);
                    // cout<<"found right"<<endl;
                }
            }
        }
    }
    auto it=toRem.begin();
    for(auto it1=source.begin();it1!=source.end();++it1)
    {
        if(*it==source.end())
            break;
        if(*it==it1)
            it++;
        else
        {
            dest.push_back(*it1);
        }
    }
}

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

string freeRegister()
{
    auto it=AccessPattern.begin();
    auto reg=*it;
    auto var=varReg[reg];
    auto loc=location[var];
    if(regStatus[reg]==true)
    {
        string ins="\tMOV r1, #0x";
        stringstream stream;
        stream<<hex<<loc;
        ins.append(stream.str());
        instructions.push_back(ins);
        ins.clear();
        ins="\tSTR ";
        ins.append(reg);
        ins.append(", [r1]");
        instructions.push_back(ins);
        ins.clear();
    }
    return reg;
}

string allocRegToVar(symrec *var,bool isAssn=false)
{
    cout<<"\tAllocating Reg to var"<<endl;
    if(location.find(var)->second==0)
    {
        cout<<"This variable \""<<var->name<<"\" was not initialised"<<endl;
        exit(1);
    }
    // bool found=false;
    string reg;
    for (auto it=varReg.begin();it!=varReg.end();++it)
    {
        if(it->second==var)
        {
            reg=it->first;
            auto temp=find(AccessPattern.begin(),AccessPattern.end(),reg);
            AccessPattern.erase(temp);
            AccessPattern.push_back(reg);
            return reg;
        }
    }
    int loc=location.find(var)->second;
    string ins;
    for (auto it=varReg.begin();it!=varReg.end();++it)
    {
        if(it->second==NULL and it->first!="r2" and it->first!="r3")
        {
            reg=it->first;
            varReg[reg]=var;
            AccessPattern.push_back(reg);
            if(isAssn=false)
            {
                ins="\tMOV r1, #0x";
                stringstream stream;
                stream<<hex<<loc;
                ins.append(stream.str());
                instructions.push_back(ins);
                ins.clear();
                ins="\tLDR ";
                ins.append(reg);
                ins.append(", [r1]");
                instructions.push_back(ins);
            }
            return reg;
        }
    }
    reg=freeRegister();
    auto temp=find(AccessPattern.begin(),AccessPattern.end(),reg);
    AccessPattern.erase(temp);
    AccessPattern.push_back(reg);
    varReg[reg]=var;
    if(isAssn=false)
    {
        ins="\tMOV r1, #0x";
        stringstream stream;
        stream<<hex<<loc;
        ins.append(stream.str());
        instructions.push_back(ins);
        ins.clear();
        ins="\tLDR ";
        ins.append(reg);
        ins.append(", [r1]");
        instructions.push_back(ins);
    }
    return reg;
}

string allocRegToNum(double val)
{
    cout<<"\tAllocating reg to num"<<endl;
    string ins;
    string reg;
    if(regStatus["r2"]==true)
        {
            ins="\tMOV r3, #";
            reg="r3";
            regStatus["r3"]=true;
        }
    else
        {
            ins="\tMOV r2, #";
            reg="r2";
            regStatus["r2"]=true;
        }
    // string ins="\tMOV r3, #";
    ins.append(to_string((int)val));
    instructions.push_back(ins);
    return reg;
}

string trExpression(AST *node)
{
    cout<<"Translating Expression"<<endl;
    if(node==NULL)
        return "nr";
    else if(node->Kind==AST::A_var)
    {
        cout<<"\tExpression goes to var"<<endl;
        string reg=allocRegToVar(node->node.variable,false);
        return reg;
    }
    else if(node->Kind==AST::A_num)
    {
        cout<<"\tExpression goes to num"<<endl;
        string reg=allocRegToNum(node->node.val);
        return reg;
    }
    else
    {
        cout<<"\tComplex Expression"<<endl;
        char op=node->node.Expression.Operator;
        string ins;
        auto regLeft=trExpression(node->node.Expression.left);
        if(regLeft=="r0")
        {
            ins="\tADD r1, ";
            ins.append(regLeft);
            ins.append(", #0");
            instructions.push_back(ins);
            ins.clear();
            regLeft="r1";
        }
        auto regRight=trExpression(node->node.Expression.right);
        if(op=='+')
        {
            ins="\tADD r0, ";
            ins.append(regLeft);
            ins.append(", ");
            ins.append(regRight);
            instructions.push_back(ins);
            ins.clear();
        }
        else if(op=='*')
        {
            ins="\tMUL r0, ";
            ins.append(regLeft);
            ins.append(", ");
            ins.append(regRight);
            instructions.push_back(ins);
            ins.clear();
        }
        else if(op=='/')
        {
            ins="\tSDIV r0, ";
            ins.append(regLeft);
            ins.append(", ");
            ins.append(regRight);
            instructions.push_back(ins);
            ins.clear();
        }
        else if(op=='-')
        {
            if(regLeft=="nr")
            {
                ins="\tRSB r0, ";
                ins.append(regRight);
                ins.append(", #0");
                instructions.push_back(ins);
                ins.clear();
            }
            else
            {
                ins="\tSUB r0, ";
                ins.append(regLeft);
                ins.append(", ");
                ins.append(regRight);
                instructions.push_back(ins);
                ins.clear();
            }
        }
        else
        {
            cout<<"wrong operator \""<<op<<"\" in expression"<<endl;
            exit(1);
        }
        regStatus["r2"]=false;
        regStatus["r3"]=false;
    }
    return "r0";
}

string trCond(AST *cond)
{
    cout<<"Translating Condition with op"<<cond->node.cond.op<<endl;
    string op=cond->node.cond.op;
    // cout<<"opsize is "<<op.size()<<endl;
    auto lhs=*(expDest.begin());
    expDest.erase(expDest.begin());
    auto regLeft=trExpression(lhs);
    if(regLeft=="r0")
    {
        string ins="\tADD r1,r0,#0";
        instructions.push_back(ins);
        regLeft="r1";
    }
    auto rhs=*(expDest.begin());
    expDest.erase(expDest.begin());
    auto regRight=trExpression(rhs);
    string ins="\tCMP ";
    ins.append(regLeft);
    ins.append(",");
    ins.append(regRight);
    instructions.push_back(ins);
    string ret;
    if(op==">=")
        // condStack.push_back("bge ");
        ret="\tBLT ";
    else if(op=="<=")
        // condStack.push_back("ble ");
        ret="\tBGT ";
    else if(op=="==")
        // condStack.push_back("beq ");
        ret="\tBEQ ";
    else if(op=="!=")
        // condStack.push_back("bne ");
        ret="\tBNE ";
    else if(op==">")
        // condStack.push_back("bgt ");
        ret="\tBLE ";
    else if(op=="<")
        // condStack.push_back("blt ");
        ret="\tBGE ";
    else
    {
        cout<<"Wrong operator in cond"<<endl;
        exit(1);
    }
    return ret;
}

void trWhile(AST *While)
{
    // cout<<"Translating While"<<endl;
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
}

void trAssignment(AST *assn)
{   
    cout<<"Translating Assn"<<endl;
    int loc;
    if(location.find(assn->node.Assignment.variable)==location.end())
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
    auto rhs=*(expDest.begin());
    // cout<<"\tRhs is ot type"<<rhs->Kind<<endl;
    auto regRight=trExpression(rhs);
    // cout<<"Rigister allocated "<<regRight<<endl;
    expDest.erase(expDest.begin());
    string regLeft=allocRegToVar(assn->node.Assignment.variable,true);
    regStatus[regLeft]=true;
    string ins="\tADD ";
    ins.append(regLeft);
    ins.append(", ");
    ins.append(regRight);
    ins.append(", #0");
    instructions.push_back(ins);
    ins.clear();
}

void trIf(AST *If)
{
    cout<<"Translating If"<<endl;
    ++ifCount;
    string tmp="if";
    tmp.append(to_string(ifCount));
    instructions.push_back(tmp);
    tmp.clear();
    string ins=trCond(If->node.If.cond);
    tmp="end_if";
    tmp.append(to_string(ifCount));
    ins.append(tmp);
    instructions.push_back(ins);
    ifStack.push_back(tmp);
}

void trIfElse(AST *IfElse)
{
    return;
}

void TranslatorMain(vector<AST*>::iterator &it1,vector<AST*>::iterator &it2)
{
    if((*it1)->Kind==AST::A_Assn)
        trAssignment(*it1);
    else if((*it1)->Kind==AST::A_WhileStm)
        trWhile(*it1);
    else if((*it1)->Kind==AST::A_IfStm)
        trIf(*it1);
    else
    {
        cout<<"wrong case"<<endl;
        exit(1);
    }
    if(*it1==*it2 and ((*it1)->Kind==AST::A_WhileStm or (*it1)->Kind==AST::A_IfStm))
    {
        advance(it1,1);
        advance(it2,1);
        TranslatorMain(it1,it2);
    }
    // if((*it2)->Kind==AST::A_WhileStm)
    // {
    //     auto tmp1=*(whileStack.rbegin());
    //     whileStack.pop_back();
    //     auto tmp2=*(whileStack.rbegin());
    //     whileStack.pop_back();
    //     string ins="\tB ";
    //     ins.append(tmp2);
    //     instructions.push_back(ins);
    //     instructions.push_back(tmp1);
    // }
    // if((*it2)->Kind==AST::A_IfStm)
    // {
    //     auto tmp1=(*ifStack.rbegin());
    //     ifStack.pop_back();
    //     instructions.push_back(tmp1);
    // }
}