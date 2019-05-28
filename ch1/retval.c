#include <stdio.h>
#include <stdint.h>
#include "lex.h"

uint8_t * factor_retval(void);

uint8_t * term_retval(void);

uint8_t * expression_retval(void);

extern uint8_t * newname_retval(void);

extern uint8_t * freename_retval(uint8_t *s);

void statements_retval(void)
{
	/* statements -> expression SEMI | expression SEMI statements */

	uint8_t * tempvar = 0x0;

	while ( match(EOI) != 1 )
	{
		tempvar = expression();

		if ( match(SEMI) == 1 )
		{
			advance();
		}

		else
		{
			fprintf(stderr,"%llu: Inserting missing semicolon\n",yylineno);
		}

		freename(tempvar);
	}

}

uint8_t * expression_retval(void)
{
	/* expression -> term expression'
	 * expression' -> PLUS term expression' | epsilon
	 */

	uint8_t * tempvar = 0x0, * tempvar2 = 0x0;

	tempvar = term();

	while ( match( PLUS ) == 1 )
	{
		advance();

		tempvar2 = term();

		printf("%s += %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}

	while ( match( MINUS ) == 1 )
	{
		advance();

		tempvar2 = term();

		printf("%s -= %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}

}

uint8_t * term_retval(void)
{
	uint8_t * tempvar = 0x0, *tempvar2 = 0x0;

	tempvar = factor();

	while ( match( TIMES ) == 1 )
	{
		advance();

		tempvar2 = factor();

		printf("%s *= %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}

	while ( match( DIVIDE ) == 1 )
	{
		advance();

		tempvar2 = factor();

		printf("%s /= %s\n",tempvar,tempvar2);

		freename(tempvar2);
	}

	return tempvar;
}

uint8_t * factor_retval(void)
{
	uint8_t * tempvar = 0x0;

	if ( match(NUM_OR_ID) == 1 )
	{
		/* Print the assignment instruction. THe %0.*s conversion is a form
		 * of %X.Ys, where X is the field width and Y is the maximum number
		 * of characters that will be printed ( even if the string is
		 * longer). I'm using hte %0.*s to print the string because it is
		 * not \0 terminated. The field has a default width of 0, but it
		 * will grow the size needed to print the string. The ".*" tells
		 * printf to take the maximum-number-of-characters count from the
		 * next argument (yyleng).
		 */
		
		printf("%s = %0.*s\n",tempvar = newname(), yyleng, yytext);

		advance();
	}

	else if ( match(LP) == 1 )
	{
		advance();

		tempvar = expression();

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
		fprintf(stderr,"%llu: Number or identifier expectedn",yylineno);
	}

	return tempvar;
}


	
