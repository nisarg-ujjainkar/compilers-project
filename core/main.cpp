#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"ast.hpp"
#include"parser.hpp"
#include"translate.hpp"

using namespace std;


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
    for(auto a:head)
    {
        if(a->Kind==AST::A_WhileStm)
        {
            cout<<"cond has type "<<a->node.While.cond->Kind<<endl;
            cout<<"code has type "<<a->node.While.code->Kind<<endl;
        }
    }
    cout<<"======================Translation====================="<<endl;
    TranslatorMain(head,dest);
    fstream file;
    file.open("out.s",ios::trunc| ios::out);
    for(auto str:instructions)
    {
        file<<str<<endl;
    }
    instructions.clear();
    location.clear();
    for(auto a:head)
    {
        delete a;
    }
    return 0;
}