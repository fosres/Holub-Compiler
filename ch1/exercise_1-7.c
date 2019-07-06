#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex_exp.h"
#include "lisp.h" 

uint8_t infix[1025]; 

uint8_t stack[513];

uint8_t prefix[2050];

uint8_t * infix_p = infix;

uint8_t * stack_p = &stack[0] - 1;

uint8_t * prefix_p = &prefix[0];

static bool is_valid_expression;

uint8_t 	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
uint8_t 	*yycurrent	= 0x0;
uint64_t	yyleng		= 0; 	/* Lexeme length.		*/
uint64_t	yylineno	= 0;	/* Input line number		*/

uint64_t Lookahead = 0xff;

uint64_t lisp_lex(void)
{
	yycurrent = yytext + yyleng;
	
	while ( *yycurrent == 0x0 )
	{
#if 0
Get new lines, skipping any leading white space on the line, until a nonblank line is found.
#endif
	
	yycurrent = prefix;

	if ( fgets(prefix,1024,stdin) == NULL )
	{
		exit(1); 
	}

	while ( isspace(*yycurrent)  )
	{	
		yycurrent++;	
	}
	
	}
	
	while ( isspace(*yycurrent) && (*yycurrent != 0xa) )
	{	yycurrent++; 	}

	while ( *yycurrent != 0x0 )
	{
		yytext = yycurrent;

		yyleng = 1;

		switch(*yycurrent)
		{
			case 0xff: { exit(1); }
			case '+': return PLUS;
			case '-': return MINUS;
			case '*': return ASTK;
			case '/': return DIVIDE;
			case '%': return MODULUS;
			case '(': return LP;
			case ')': return RP;
			case '\n': return NL;
			case '\t':
			case ' ':
			{ break; }
			
			default:
			{
				if ( isdigit(*yycurrent) )
				{
					while ( isdigit(*yycurrent) )
					{
						yycurrent++; 
					}

					yyleng = yycurrent - yytext;

					return NUM;
				
				}
				
				else
				{
					fprintf(stderr,"Error: Invalid lexeme\n"
						);

					yytext = yycurrent = &prefix[1023];
				}

				break;
			} //end default block

		} // end switch statement
		
		yycurrent++; 
	} //end while loop for tokens

	lisp_lex();

}

bool match(uint64_t token)
{
	//Return true if "token" matches yycurrent lookahead symbol

	if ( Lookahead == 0xff )
	{
		Lookahead = lisp_lex();
	}

	return token == Lookahead;
}

void advance(void)
{
	//Advance lookahead to next symbol
	
	Lookahead = lisp_lex();
}

bool isoperator(uint8_t in)
{
	switch (in)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case ')':
		case '(':
		{
			return 1;
		}

		default: {break;}
	}

	return 0;
}


void prefix_expr(void)
{
	if ( match(NL) )
	{	return;	}

	if ( !match(LP) && !match(NUM) )
	{	
		fprintf(stderr,"%llu: Error: Expected a left-parenthesis or"

		" integer constant\n",
		
		prefix_p - &prefix[0]
		);

		is_valid_expression = 0;

		return;
	}

	
	if ( match(LP) )
	{
		advance();
		
		if ( !isoperator(*prefix_p) )
		{
			fprintf(stderr,"%llu: Missing operator\n",
				prefix_p - &prefix[0]
				);
			
			is_valid_expression = 0;

			return;
		}

		if ( isoperator(*prefix_p) )
		{
			advance();

			if ( match(NL) )
			{	is_valid_expression = 0; return;	}

			prefix_expr();

			advance();

			if ( match(NL) )
			{	is_valid_expression = 0; return;	}

			prefix_expr();

		}
		
		advance();

		if ( !match(RP) || !is_valid_expression )
		{	
			is_valid_expression = 0;	

			if ( !match(RP) )
			{
				fprintf(stderr,"%llu: Error: Missing right parenthesis\n",
					prefix-&prefix[0]
					);
			}

			return;
		}

	}		
	
	is_valid_expression = 1;	
	
}

int main(void)
{
	while ( 1 )
	{
		prefix_expr();

		printf("%llu\n",is_valid_expression);
	}
	
	return 0;
}
