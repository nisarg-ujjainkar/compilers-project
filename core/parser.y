/* 
Reference: http://dinosaur.compilertools.net/bison/bison_5.html
*/
%{
	#include<stdio.h>
	#include<string.h>
	#include<stdlib.h>  
	#include<ctype.h>
	#include<stdbool.h>
	#include<vector>
	#include "ast.hpp"  /* Info for ast */
	#include "table.hpp"  /* Contains definition of `symrec'        */
	extern "C" int yylex();  
	extern "C" FILE *yyin;
	void yyerror (const char *); 
	std::vector<AST*> head;
	std::vector<AST*> exps;
%}
%union 
{
	double val;  /* For returning numbers.                   */
	struct symrec *tptr;   /* For returning symbol-table pointers      */
	struct AST *ast;
	char *op;
}

%define parse.error verbose
%token  <val> NUM        /* Simple double precision number   */
%token <tptr> VAR   /* Variable and Function            */
%token WHILE
%token FOR
%token IF
%token ELSE
%type  <ast> exp code block line assn IfStm WhileStm cond IfElse
%token <op> GEQ LEQ LT GT EQ NEQ AND OR
%type <op> join relop


%left GEQ LEQ LT GT EQ NEQ

%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* Negation--unary minus */
 //%right '^'    /* Exponentiation        */

/* Grammar follows */

%%


code:	/* Empty */		{$$=NULL;}
		| code line		{if($$==NULL)$$=$2;head.push_back($2);};

block:	'{' code '}'	{$$=$2;};

line:	WhileStm				{$$=$1;}
		| IfStm					{$$=$1;}
		| IfElse				{$$=$1;}
		| assn ';'				{$$=$1;};

assn: VAR '=' exp				{$$=genAssignment($1,$3);};

IfStm:	IF '(' cond ')' line				{head.push_back($5);$$=genIf($3,$5);}
		| IF '(' cond ')' block				{$$=genIf($3,$5);};

IfElse:	IF '(' cond ')' line ELSE line			{head.push_back($5);$$=genIfElse($3,$5,$7);head.push_back($7);}	
		| IF '(' cond ')' line ELSE block		{head.push_back($5);$$=genIfElse($3,$5,$7);}	
		| IF '(' cond ')' block ELSE line		{$$=genIfElse($3,$5,$7);head.push_back($7);}	
		| IF '(' cond ')' block ELSE block		{$$=genIfElse($3,$5,$7);}	
		
WhileStm: WHILE '(' cond ')' line				{head.push_back($5);$$=genWhile($3,$5);}
		  | WHILE '(' cond ')' block			{$$=genWhile($3,$5);};

cond:	cond join cond			{$$=genCondJoin($1,$3,$2);}
		| exp relop exp			{$$=genCond($1,$3,$2);};

join:	AND		{$$=$1;}
		| OR	{$$=$1;};

relop:	GEQ		{$$=$1;}
		| LEQ	{$$=$1;}
		| NEQ	{$$=$1;}
		| EQ	{$$=$1;}
		| LT	{$$=$1;}
		| GT	{$$=$1;};


exp:	VAR						{ $$=genVariable($1); exps.push_back($$); }
		| NUM					{ $$=genNumber($1); exps.push_back($$);}
		| exp '+' exp			{ $$=genExpression($1,$3,'+'); exps.push_back($$);}
		| exp '-' exp			{ $$=genExpression($1,$3,'-'); exps.push_back($$);}
		| exp '*' exp			{ $$=genExpression($1,$3,'*'); exps.push_back($$);}
		| exp '/' exp			{ $$=genExpression($1,$3,'/'); exps.push_back($$);}
		| '-' exp %prec NEG		{ $$=genExpression(NULL,$2,'-'); exps.push_back($$);}
		| '(' exp ')'			{ $$ = $2; };

/* End of grammar */
%%


void yyerror (const char *s)  /* Called by yyparse on error */
{
	printf ("Line %d: %s\n", lineNum,s);
}


