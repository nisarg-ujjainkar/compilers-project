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
#include "calc.h"  /* Contains definition of `symrec'        */
int  yylex(void);
void yyerror (char  *); 
%}
%union {
int inte;
double   val;  /* For returning numbers.                   */
struct symrec  *tptr;   /* For returning symbol-table pointers      */
}



%token <inte> INT
%token  <val> FLOAT        /* Simple double precision number   */
%token <tptr> VAR   /* Variable and Function            */
%token WHILE
%token FOR
%token IF
%token ELSE
%type  <val>  exp


%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* Negation--unary minus */
 //%right '^'    /* Exponentiation        */

/* Grammar follows */

%%
code:   /* Empty */
        | code line;

line:   whilestm
        | ifstm
        | exp ';';

whilestm:       WHILE exp line
                | WHILE exp block;

ifstm:  IF exp line
        | IF exp block
        | IF exp line ELSE line
        | IF exp block ELSE line
        | IF exp block ELSE block
        | IF exp line ELSE block;

block:  '{' code '}';

exp:      NUM                { $$ = $1;                         }
        | VAR                { $$ = $1->value;                  }
        | VAR '=' exp        { $$ = $3; $1->value = $3;         }
        | exp '+' exp        { $$ = $1 + $3;                    }
        | exp '-' exp        { $$ = $1 - $3;                    }
        | exp '*' exp        { $$ = $1 * $3;                    }
        | exp '/' exp        { $$ = $1 / $3;                    }
        | '-' exp  %prec NEG { $$ = -$2;                        }
        | '(' exp ')'        { $$ = $2;                         }
;
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


