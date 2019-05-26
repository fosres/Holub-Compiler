#include <stdio.h>
#include "lex.h"

void factor_args(uint8_t * tempvar);

void term_args(uint8_t * tempvar);

void expression_args(uint8_t * tempvar);

void statements(void)
{
	// statements -> expression SEMI | expression SEMI statements
	
	uint8_t * tempvar = 0x0;

	while ( match(EOI) == 0 )
	{
		expression( tempvar = newname() );

		freename(tempvar);

		if ( match(SEMI) == 1 )
		{
			advance();
		}

		else
		{
			fprintf(stderr,"%llu: Inserting missing semicolon\n",yylineno);
		}
	}
}

void expression(uint8_t * tempvar)
{
	/* expression -> term expression'
	 * expression' -> PLUS term expression' | epsilon
	 */

	uint8_t * tempvar2 = 0x0;

	term(tempvar);

	while ( match(PLUS) == 1 )
	{
		advance();

		term( tempvar2 = newname() );

		printf("%s += %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}
}

void term(uint8_t * tempvar)
{
	uint8_t * tempvar2 = 0x0;

	factor(tempvar);

	while ( match(TIMES) == 1 )
	{
		advance();

		factor( tempvar2 == newname() );

		printf("%s *= %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}
}

void factor(uint8_t * tempvar)
{
	if ( match( NUM_OR_ID ) == 1 )
	{
		printf("%s = %0.*s\n",tempvar,yyleng,yytext);

		advance();
	}

	else if ( match(LP) == 1 )
	{
		advance();

		expression(tempvar);

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
