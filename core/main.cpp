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
    exit(1);
    cout<<"parsing exps"<<endl;
    for(auto b:exps)
    {
        cout<<b->Kind;
    }
    cout<<endl;
    filterExp(exps,expDest);
    cout<<"======================Translation====================="<<endl;
    TranslatorMain(dest,head);
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