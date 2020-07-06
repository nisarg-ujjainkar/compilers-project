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
	#include "ast.h"  /* Info for ast */
	#include "table.h"  /* Contains definition of `symrec'        */
	int  yylex(void);
	void yyerror (const char *); 
	std::vector<AST::AST*> head;
%}
%union 
{
	double val;  /* For returning numbers.                   */
	struct symrec *tptr;   /* For returning symbol-table pointers      */
	struct AST::AST *ast;
	char *op;
}

%define parse.error verbose
%token  <val> NUM        /* Simple double precision number   */
%token <tptr> VAR   /* Variable and Function            */
%token WHILE
%token FOR
%token IF
%token ELSE
/* %token GEQ
%token LEQ
%token GT
%token LT
%token EQ
%token NEQ
%token AND
%token OR
%token join relop */
%type  <ast> exp code block line assn IfStm WhileStm cond
%token <op> GEQ LEQ GT LT EQ NEQ AND OR
%type <op> join relop
//%type <op>	GEQ LEQ GT LT EQ NEQ AND OR join relop


%left GEQ LEQ LT GT EQ NEQ
%left AND OR
%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* Negation--unary minus */
 //%right '^'    /* Exponentiation        */

/* Grammar follows */

%%


code:	/* Empty */
		| code line		{head.push_back($1);};

block:	'{' code '}'	{$$=$2;};

line:	WhileStm				{$$=$1;}
		| IfStm					{$$=$1;}
		| block					{$$=$1;}
		| assn ';'				{$$=$1;};

assn: VAR '=' exp		{$$=AST::genAssignment($1,$3);};

IfStm:	IF '(' cond ')' line				{$$=AST::genIf($3,$5);}
		| IF '(' cond ')' line ELSE line	{$$=AST::genIfElse($3,$5,$7);};	
		
WhileStm: WHILE '(' cond ')' line			{$$=AST::genWhile($3,$5);};

cond:	cond join cond			{$$=AST::genCondJoin($1,$3,$2);}
		| exp relop exp			{$$=AST::genCond($1,$3,$2);};

join:	AND		{$$=$1;}
		| OR	{$$=$1;};

relop:	GEQ		{$$=$1;}
		| LEQ 	{$$=$1;}
		| LT 	{$$=$1;}
		| GT	{$$=$1;}
		| EQ 	{$$=$1;}
		| NEQ	{$$=$1;};

exp:	VAR						{ $$=AST::genVariable($1); }
		| NUM					{ $$=AST::genNumber($1); }
		| exp '+' exp			{ $$=AST::genExpression($1,$3,'+'); }
		| exp '-' exp			{ $$=AST::genExpression($1,$3,'-'); }
		| exp '*' exp			{ $$=AST::genExpression($1,$3,'*'); }
		| exp '/' exp			{ $$=AST::genExpression($1,$3,'/'); }
		| '-' exp %prec NEG		{ $$=AST::genExpression(NULL,$2,'-'); }
		| '(' exp ')'			{ $$ = $2; };

/* End of grammar */
%%





int main ()
{
	yyparse ();
}

void yyerror (const char *s)  /* Called by yyparse on error */
{
	printf ("Line %d: %s\n", lineNum,s);
}


