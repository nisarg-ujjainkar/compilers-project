#ifndef TRANSLATE_H
#define TRANSLATE_H

#include"ast.hpp"
#include<map>

extern std::map<symrec*,int> location;
extern std::vector<std::string> instructions;
extern std::map<std::string,symrec*> varReg;
extern std::map<std::string,bool> regStatus;
extern std::vector<std::string> whileStack;
extern std::vector<std::string> ifStack;
extern std::vector<std::string> ifElseStack;
extern std::vector<AST*> expDest;
extern std::vector<string> AccessPattern;
extern std::vector<AST*> elseStack; 

extern int whileCount;
extern int ifCount;
extern int ifElseCount;
extern int ElseCount;


void linearise(std::vector<AST*>source,std::vector<AST*>&dest);
void filterExp(std::vector<AST*>source,std::vector<AST*>&dest);
std::string trExpression(AST*);
void trAssignment(AST*);
void trIf(AST*);
void trIfElse(AST*);
void trWhile(AST*,AST*);
std::string trCond(AST*);
void trCondJoin(AST*);
std::string allocRegToVar(symrec*,bool);
std::string allocRegToNum(double val);
std::string freeRegister();
void TranslatorMain(std::vector<AST*>::iterator&it1,std::vector<AST*>::iterator&it2);
void init(void);

#endif