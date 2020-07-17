#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"ast.hpp"
#include"parser.hpp"
#include"translate.hpp"

using namespace std;
vector<AST*> expDest;

int main()
{
    yyparse();
    cout<<head.size()<<endl;
    for (auto a:head)
    {
        cout<<a->Kind;
    }
    cout<<'\n';
    vector<AST*> dest;
    linearise(head,dest);
    for (auto a:dest)
    {
        cout<<a->Kind;
    }
    cout<<endl;
    cout<<"parsing exps"<<endl;
    for(auto b:exps)
    {
        cout<<b->Kind;
    }
    cout<<endl;
    filterExp(exps,expDest);
    cout<<"======================Translation====================="<<endl;
    init();
    auto it1=dest.begin();
    auto it2=head.begin();
    while(it1!=dest.end() and it2!=head.end())
    {
        auto temp=it2;
        TranslatorMain(it1,it2);
        while(temp!=it2)
        {
            if((*temp)->Kind==AST::A_WhileStm)
            {
                auto tmp1=*(whileStack.rbegin());
                whileStack.pop_back();
                auto tmp2=*(whileStack.rbegin());
                whileStack.pop_back();
                string ins="\tB ";
                ins.append(tmp2);
                instructions.push_back(ins);
                instructions.push_back(tmp1);
            }
            if((*temp)->Kind==AST::A_IfStm)
            {
                auto tmp1=(*ifStack.rbegin());
                ifStack.pop_back();
                instructions.push_back(tmp1);
            }
            ++temp;
        }
        if((*it2)->Kind==AST::A_WhileStm)
        {
            auto tmp1=*(whileStack.rbegin());
            whileStack.pop_back();
            auto tmp2=*(whileStack.rbegin());
            whileStack.pop_back();
            string ins="\tB ";
            ins.append(tmp2);
            instructions.push_back(ins);
            instructions.push_back(tmp1);
        }
        if((*it2)->Kind==AST::A_IfStm)
        {
            auto tmp1=(*ifStack.rbegin());
            ifStack.pop_back();
            instructions.push_back(tmp1);
        }
        ++it1;
        ++it2;
    }
    fstream file;
    file.open("out.s",ios::trunc| ios::out);
    file<<" AREA ASDF, CODE, READONLY"<<endl;
    file<<"\tENTRY"<<endl;
    for(auto str:instructions)
    {
        file<<str<<endl;
    }
    file<<"\tEND"<<endl;
    cout<<"======================Translatied====================="<<endl;
    cout<<"Static register map"<<endl;
    for(auto it=varReg.begin();it!=varReg.end();++it)
    {
        if(it->second!=NULL)
            cout<<"Variable: "<<it->second->name<<" Register: "<<it->first<<endl;
        else
            cout<<"empty register: "<<it->first<<endl;
    }
    instructions.clear();
    location.clear();
    for(auto a:head)
    {
        delete a;
    }
    return 0;
}