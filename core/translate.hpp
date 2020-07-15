#ifndef TRANSLATE_H
#define TRANSLATE_H

#include"ast.hpp"
#include<map>

extern std::map<symrec*,int> location;
extern std::vector<std::string> instructions;
extern std::map<int,symrec*> registers;
extern std::map<std::string,bool> regStatus;
extern std::vector<std::string> whileStack;
extern std::vector<std::string> ifStack;
extern std::vector<std::string> ifElseStack;
extern std::vector<std::string> condStack;
extern int whileCount;


void linearise(std::vector<AST*>source,std::vector<AST*>&dest);

std::string trExpression(AST*);
void trAssignment(AST*);
void trIf(AST*);
void trIfElse(AST*);
void trWhile(AST*,AST*);
std::string trCond(AST*);
void trCondJoin(AST*);
std::string allocRegToVar(symrec*);
std::string allocRegToNum(double val);
void TranslatorMain(std::vector<AST*>head,std::vector<AST*>dest);

#endif