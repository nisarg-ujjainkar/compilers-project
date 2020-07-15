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
        TranslatorMain(it1,it2);
        ++it1;
        ++it2;
    }
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