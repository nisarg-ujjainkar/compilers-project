%{
	#include "ast.hpp"
	#include "parser.hpp"
	#include "table.hpp"
    #include<string.h>
	#include <stdlib.h>
    #include<string>
    #define YY_DECL extern "C" int yylex()
	symrec *sym_table = (symrec *)0;
	int lineNum=0;
%}
floatpt [0-9]+"."?[0-9]*
integer [0-9]+
id [a-zA-Z]+[a-zA-Z0-9_]*
%%
"while" {return WHILE;}
"if"    {return IF;}
"for"   {return FOR;}
"else"  {return ELSE;}
"<="    {yylval.op=strdup(yytext);return LEQ;}
">="    {yylval.op=strdup(yytext);return GEQ;}
"<"     {yylval.op=strdup(yytext);return LT;}
">"     {yylval.op=strdup(yytext);return GT;}
"=="    {yylval.op=strdup(yytext);return EQ;}
"!="    {yylval.op=strdup(yytext);return NEQ;}
"&&"    {yylval.op=strdup(yytext);return AND;}
"||"    {yylval.op=strdup(yytext);return OR;}
{floatpt} {sscanf(yytext,"%lf",&yylval.val);return NUM;}
{id} {symrec *s;s=getsym(yytext); if(s==0) s=putsym(yytext,VAR);yylval.tptr=s;return VAR;}
[ \t] {/* Remove White Space */}
[\n]  {lineNum++;}
.  {return yytext[0];}
%%

/* The symbol table: a chain of `struct symrec'.  */


symrec * putsym (char *sym_name,int sym_type)
{
    symrec *ptr;
    ptr = (symrec *) malloc (sizeof (symrec));
    ptr->name = (char *) malloc (strlen (sym_name) + 1);
    strcpy (ptr->name,sym_name);
    ptr->type = sym_type;
    ptr->value = 0; /* set value to 0 even if fctn.  */
    ptr->next = (struct symrec *)sym_table;
    sym_table = ptr;
    return ptr;
}

symrec *getsym (char *sym_name)
{
    symrec *ptr;
    for(ptr = sym_table; ptr != (symrec *) 0;
    ptr = (symrec *)ptr->next)
    if (strcmp (ptr->name,sym_name) == 0)
    	return ptr;
    return 0;
}