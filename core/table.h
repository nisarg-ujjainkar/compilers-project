/* Data type for links in the chain of symbols.      */
#ifndef TABLE_H
#define TABLE_H
struct symrec
{
	char *name;  /* name of symbol                     */
	int type;    /* type of symbol: either VAR or FNCT */
  	double value;           /* value of a VAR          */
  	struct symrec *next;    /* link field              */
};

typedef struct symrec symrec;

/* The symbol table: a chain of `struct symrec'.     */
extern symrec *sym_table;

extern int lineNum;

symrec *putsym ();
symrec *getsym ();

#endif