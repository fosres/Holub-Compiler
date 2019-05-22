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

void term_prime()
{
	/* term' -> TIMES factor term'
	 * 	| epsilon
	 */

	if ( match(TIMES) == 1 )
	{
		advance();

		factor();

		term_prime();
	}
	
}

void factor()
{
	/* factor -> NUM_OR_ID
	 * 	| LP expression RP
	 */

	if ( match(NUM_OR_ID) == 1 )
	{
		advance();
	}

	else if ( match(LP) == 1 )
	{
		advance();

		expression();

		if ( match(RP) == 1 )
		{
			advance();
		}

		else
		{
			fprintf(stderr,"%llu: Mismatched parenthesis\n",yylineno);
		}
	}

	else
	{
		fprintf(stderr,"%llu: Number or identifier expected\n",yylineno);
	}
}
