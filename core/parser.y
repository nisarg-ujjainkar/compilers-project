/* 
Reference: http://dinosaur.compilertools.net/bison/bison_5.html
*/
%error-verbose
%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>  
#include<ctype.h>
#include<math.h>
#include<stdbool.h>
#include "table.h"  /* Contains definition of `symrec'        */
int  yylex(void);
void yyerror (char  *); 
%}
%union {
int integer;
double val;  /* For returning numbers.                   */
struct symrec  *tptr;   /* For returning symbol-table pointers      */
}



%token <inte> INT
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
code:	/* Empty */
		| code line
		| code block;

block:	'{' code '}';

line:	WhileStm
		| IfStm
		| assn ';';

assn: exp '=' exp;

IfStm:	IF '(' cond ')' block
		| IF '(' cond ')' line
		| IF '(' cond ')' block ELSE block
		| IF '(' cond ')' line ELSE block
		| IF '(' cond ')' line ELSE line
		| IF '(' cond ')' block ELSE line;

WhileStm: WHILE '(' cond ')' block
		  | WHILE '(' cond ')' line;

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


