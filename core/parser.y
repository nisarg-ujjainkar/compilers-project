/* 
Reference: http://dinosaur.compilertools.net/bison/bison_5.html
*/
%error-verbose
%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>  
#include<ctype.h>
#include<stdbool.h>
#include "table.h"  /* Contains definition of `symrec'        */
#include "ast.h"  /* Info for ast */
int  yylex(void);
void yyerror (char  *); 
%}
%union {
double val;  /* For returning numbers.                   */
struct symrec  *tptr;   /* For returning symbol-table pointers      */
AST *ast
}



%token  <val> NUM        /* Simple double precision number   */
%token <tptr> VAR   /* Variable and Function            */
%token WHILE
%token FOR
%token IF
%token ELSE
%token GEQ
%token LEQ
%token GT
%token LT
%token EQ
%token NEQ
%token AND
%token OR
%type  <val>  exp


%left GEQ LEQ LT GT EQ NEQ
%left AND OR
%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* Negation--unary minus */
 //%right '^'    /* Exponentiation        */

/* Grammar follows */

%%
code:	/* Empty */		{$$=0;}
		| code line;

block:	'{' code '}'	{$$=$2;};

line:	WhileStm				{$$=$1;printf("While Found on line %d\n",lineNum);}
		| IfStm					{$$=$1;}
		| block					{$$=$1;}
		| assn ';'				{$$=$1;printf("Assignment Found on line %d\n",lineNum);};

assn: VAR '=' exp;

IfStm:	IF '(' cond ')' line				{printf("If Found on line %d\n",lineNum);}
		| IF '(' cond ')' line ELSE line	{printf("If eLSE Found on line %d\n",lineNum);};	
		
WhileStm: WHILE '(' cond ')' line;

cond:	cond join cond
		| exp relop exp;

join:	AND
		| OR;

relop:	GEQ 
		| LEQ 
		| LT 
		| GT 
		| EQ 
		| NEQ;

exp:	VAR						{ $$ = $1->value; }
		| NUM					{ $$ = $1; }
		| exp '+' exp			{ $$ = $1 + $3; }
		| exp '-' exp			{ $$ = $1 - $3; }
		| exp '*' exp			{ $$ = $1 * $3; }
		| exp '/' exp			{ $$ = $1 / $3; }
		| '-' exp %prec NEG		{ $$ = -$2; }
		| '(' exp ')'			{ $$ = $2; }

/* End of grammar */
%%





int main ()
{
	yyparse ();
}

void yyerror (char *s)  /* Called by yyparse on error */
{
	printf ("%s\n", s);
}


