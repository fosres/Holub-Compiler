//Basic parser, shows the structure but no code generation.
#include <stdio.h>
#include "lex.h"

void expression(void);

void expr_prime(void);

void term(void);

void term_prime(void);

void factor(void);

void statements(void)
{
	/*
	 * statements -> expression SEMI
	 * 		| expression SEMI statements
	 */

	expression();

	if ( match(SEMI) == 1 )
	{
		advance();
	}

	else
	{
		fprintf(stderr,"%llu: Inserting missing semicolonn",yylineno);
	}

	if ( match(EOI) != 0 )
	{
		statements();
	}
}

void expression(void)
{
	// expression -> term expression'
	
	term();

	expr_prime();
}

void expr_prime(void)
{
	/* expression' -> PLUS term expression;
	 * 		| epsilon
	 */

	if ( match(PLUS) == 1 )
	{
		advance();

		term();

		expr_prime();
	}

}

void term()
{
	// term -> factor term'
	
	factor();

	term_prime();
}
