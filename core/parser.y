/* 
Reference: http://dinosaur.compilertools.net/bison/bison_5.html
*/

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
double   val;  /* For returning numbers.                   */
struct symrec  *tptr;   /* For returning symbol-table pointers      */
}



%token  <val> NUM        /* Simple double precision number   */
%token <tptr> VAR   /* Variable and Function            */
%token <tptr> FUN
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
input:   /* empty */
        | input line
;

line:
          '\n'
        | while
        | exp ';' '\n'   { printf ("\t%lf\n", $1); }
        | error ';' '\n' { yyerrok;                }
;

exp:      NUM                { $$ = $1;                         }
        | VAR                { $$ = $1->value;                  }
        | VAR '=' exp        { $$ = $3; $1->value = $3;         }
        | exp '+' exp        { $$ = $1 + $3;                    }
        | exp '-' exp        { $$ = $1 - $3;                    }
        | exp '*' exp        { $$ = $1 * $3;                    }
        | exp '/' exp        { $$ = $1 / $3;                    }
        | FUN '(' exp ')'    { if(strcmp($1->name,"SIN")) $$=sin($3); else $$=cos($3);}
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


