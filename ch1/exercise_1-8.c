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

void clear_all(void);

void carriage_return(void);

uint64_t lisp_lex(void)
{
	yycurrent = yytext + yyleng;
	
	while ( *yycurrent == 0x0 )
	{
#if 0
Get new lines, skipping any leading white space on the line, until a nonblank line is found.
#endif
	
	yycurrent = infix;

	if ( fgets(infix,1024,stdin) == NULL )
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
					
					
					*yytext = *yycurrent = 0x0;
				}

				break;
			} //end default block

		} // end switch statement
		
		yycurrent++; 
	} //end while loop for tokens

	lisp_lex();

}

void clear_all(void)
{
	memset(infix,0x0,1025);

	memset(prefix,0x0,2050);
	
	memset(stack,0x0,513);

	infix_p = &infix[0];

	prefix_p = &prefix[0];

	stack_p = stack - 1;

	yycurrent = &infix[0];

	yytext = &infix[0];

	yyleng = 0;
	
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

bool operator(void)
{
	switch(Lookahead)
	{
		case PLUS: {  return 1; }
		case MINUS: { return 1; }
		case ASTK: {  return 1; }
		case DIVIDE: {  return 1; }
		default:
		{
			return 0;
		}
	}

}

void infix_expr(void)
{
	while ( !match(NL) )
	{
		if ( match(NUM) )
		{
			advance(); is_valid_expression = 1;

			if ( match(RP) || match(NL) ) { return; }
			
			if ( !operator() )
			{
			
				fprintf(stderr,"%llu:Missing operator: '+', '-', '*', or "
					"'/'\n",
					yytext - &infix[0]
			       );
					
			}

			is_valid_expression = 0;

			advance();

		}

		else if ( operator() ) // this is causing the error message bug
		{
			is_valid_expression = 0; advance();
		}
		
		else if ( match(RP) || match(NL) ) { return; }

		else if ( match(LP) )
		{
			advance();

			infix_expr();

			if (!is_valid_expression || !match(RP) )
			{
				if ( !match(RP) )
				{
					fprintf(stderr,"%llu: Error: Missing "
							"right-parenthesis\n",
							yytext - &infix[0]
					       );
				}

				is_valid_expression = 0; return;
			}

			is_valid_expression = 1; advance();
		}

		else //illegimate token
		{
			fprintf(stderr,"%llu: Error: Illegal lexeme\n",
					yytext - &infix[0]
			       );

			is_valid_expression = 0; return;
			
		}
	}

}


void statements(void)
{

	infix_expr();

	if ( !match(NL) )	
	{
		fprintf(stderr,"%llu: Error: Missing newline character\n",
				yytext - &infix[0]
		       );

		is_valid_expression = 0;

		return;
	}


}

int main(void)
{

	while (1)
	{	
		statements();	

		printf("is_valid_expression: %llu\n",is_valid_expression);

		clear_all();	
		
		is_valid_expression = 0;

		advance();
	}
}

