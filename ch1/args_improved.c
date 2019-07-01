#include <stdio.h>
#include "lex_exp.h"

#define MAXFIRST 16
#define SYNCH SEMI

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

void factor_args(uint8_t * tempvar);

void term_args(uint8_t * tempvar);

void expression_args(uint8_t * tempvar);

void statements(void)
{
	// statements -> expression SEMI | expression SEMI statements
	
	uint8_t * tempvar = 0x0;

	while ( match(EOI) == 0 )
	{
		expression_args( tempvar = newname() );

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

void expression_args(uint8_t * tempvar)
{
	/* expression -> term expression'
	 * expression' -> + term expression' | Ɛ
	 * expression' -> - term expression' | Ɛ
	 */

	uint8_t * tempvar2 = 0x0;

	term_args(tempvar);
	
		while ( match(PLUS) == 1 )
		{
			advance();

			term_args( tempvar2 = newname() );

			printf("%s += %s\n",tempvar,tempvar2);

			freename(tempvar2);
		}
	

		while ( match(MINUS) == 1 )
		{
			advance();

			term_args(tempvar2 = newname());

			printf("%s -= %s\n",tempvar,tempvar2);

			freename(tempvar2);
		}
}

void term_args(uint8_t * tempvar)
{
	/* term	-> factor term'
	 * term' -> * factor term'
	 * term' -> / factor term'
	 * term' -> % factor term'
	 * 	| Ɛ
	 */

	uint8_t * tempvar2 = 0x0;

	factor_args(tempvar);
			
			
				while ( match(ASTK) == 1 )
				{
			
					advance();

					factor_args( tempvar2 = newname() );

					printf("%s *= %s\n",tempvar,tempvar2);

					freename(tempvar2);
				}
			


				while ( match(DIVIDE) == 1 )
				{
					advance();

					factor_args(tempvar2 = newname() );

					printf("%s /= %s\n",tempvar,tempvar2);

					freename(tempvar2);
				}

	
				while ( match(MODULUS) == 1 )
				{
					advance();

					factor_args(tempvar2 = newname() );

					printf("%s %%= %s\n",tempvar,tempvar2);

					freename(tempvar2);

				}


}

void factor_args(uint8_t * tempvar)
{
	/* factor -> num_or_id
	 * 	| (expression)
	 */

	if ( match(NUM) || match(ID) )// legal_lookahead(NUM,ID,0)  == 1 )
	{
		printf("%s = %0.*s\n",tempvar,yyleng,yytext);

		advance();
	}

	else if ( match(LP) == 1 )
	{
		advance();

		expression_args(tempvar);

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
