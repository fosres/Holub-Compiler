#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "lex.h"

#define MAXFIRST 16
#define SYNCH SEMI

#if 0
Simple error detection and recovery. Arguments

are 0-terminated list of those tokens that

can be legitimately come next in the input.

If the list is empty, the end of file must come

next. Print an error message if necessary. Error

recovery is performed by discarding all input

symbols until one that is in the input list is

found.

Returns true if there is no error or if we

recovered from the error, false if we can not

recover.

All expressions must start with NUM_OR_ID or LP
#endif

bool legal_lookahead(uint64_t first_arg,...)
{
	va_list args;

	uint64_t tok = 0;

	static uint64_t lookaheads[MAXFIRST];

	uint64_t *p = lookaheads, *current = 0x0;

	uint64_t error_printed = 0;

	bool rval = 0;

	va_start(args, first_arg);

	if ( first_arg == 0 )
	{
		if ( match(EOI) == 1 )
		{
			rval = 1;
		}
	}

	else
	{
		*p++ = first_arg;

		while ( ( tok = va_arg(args,int) ) && p < &lookaheads[MAXFIRST] )
		{
			*++p = tok;
		}

		while ( match(SYNCH) == 0 )
		{
			for ( current = lookaheads; current < p ; current++ )
			{
				if ( match( *current ) == 1 )
				{
					rval = 1;

					goto exit;
				}
			}

			if ( error_printed == 0 )
			{
				fprintf(stderr,"Line %llu: Syntax error\n",yylineno);

				error_printed = 1;
			}

			advance();
		}
	}

	exit:
	{
		va_end(args);

		return rval;
	}	

}


void expression_improved(void)
{
	/*
	 * expression -> term expression'
	 * expression -> PLUS term expression' | epsilon
	 */

	if ( legal_lookahead( NUM_OR_ID,LP,0 ) == 0 )
	{
		return;
	}

	term_improved();

	while ( match(PLUS) == 1 )
	{
		advance();

		term_improved();
	}

}

void term_improved(void)
{
	if ( legal_lookahead( NUM_OR_ID,LP,0 ) == 0 )
	{
		return;
	}

	factor_improved();

	while ( match(TIMES) == 1 )
	{
		advance();

		factor_improved();
	}
	
}

void factor_improved(void)
{
	if ( legal_lookahead(NUM_OR_ID,LP,0) == 0 )
	{
		return;
	}

	if ( match(NUM_OR_ID) == 1 )
	{
		advance();
	}

	else if ( match(LP) == 1 )
	{
		advance();

		expression_improved();

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

void statements_improved(void)
{
	// statements -> expression SEMI | expression SEMI statements 
	
	while ( match(EOI) != 1 )
	{
		expression_improved();

		if ( match(SEMI) )
		{
			advance();
		}

		else
		{
			fprintf(stderr,"%llu: Inserting missing semicolon\n",yylineno);
		}
	}
}

