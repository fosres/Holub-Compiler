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

uint8_t prefix[1025];

uint8_t * infix_p = infix;

uint8_t * stack_p = &stack[0] - 1;

uint8_t * prefix_p = &prefix[0];

static bool is_valid_expression;

uint8_t 	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
uint8_t 	*yycurrent	= 0x0;
uint64_t	yyleng		= 0; 	/* Lexeme length.		*/
uint64_t	yylineno	= 0;	/* Input line number		*/
uint64_t 	yycharno 	= 0; 	/* Input char number		*/

uint64_t Lookahead = 0xff;

uint64_t lisp_lex(void)
{
	yycurrent = yytext + yyleng;
	
	while ( *yycurrent == 0x0 )
	{
#if 0
Get new lines, skipping any leading white space on the line, until a nonblank line is found.
#endif
	
	yycurrent = infix;

	yycharno = 1;

	if ( fgets(infix,1024,stdin) == NULL )
	{
		exit(1); 
	}

	while ( isspace(*yycurrent)  )
	{	
		yycurrent++; yycharno++;	
	}
	
	}
	
	while ( isspace(*yycurrent) && (*yycurrent != 0xa) )
	{	yycurrent++; yycharno++;	}

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
						yycurrent++; yycharno++;
					}

					yyleng = yycurrent - yytext;

					return NUM;
				
				}
				
				else
				{
					fprintf(stderr,"%llu: Error: Invalid lexeme\n",
						yycharno
						);

					yytext = yycurrent = &infix[1023];
				}

				break;
			} //end default block

		} // end switch statement
		
		yycurrent++;
	} //end while loop for tokens

	lisp_lex();

}

void advance(void)
{
	//Advance lookahead to next symbol
	
	Lookahead = lisp_lex();
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


bool isendofline(void)
{
	return ((*infix_p == 0x0) ^ (*infix_p == 0xa));		
}

bool isEOI(void)
{
	return (*infix_p == 0xff);
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
		{
			return 1;
		}

		default: {break;}
	}

	return 0;
}

size_t op_prec(uint64_t token)
{
	switch(token)
	{
		case '*':
		case '/':
		case '%':
		{
			return 2;
		}
		
		case '+':
		case '-':
		{
			return 1;
		}

		case ')':
		case '(':
		{
			return 0;
		}
		
		default: {break;}

	}

}

void clear_all(void)
{
	memset(infix,0x0,1025);

	memset(prefix,0x0,1025);
	
	memset(stack,0x0,513);

	infix_p = &infix[0];

	prefix_p = &prefix[0];

	stack_p = stack - 1;
	
}

uint8_t stack_top(void)
{
	return *stack_p;
}


void expression(void)
{
	if ( match(NUM) )
	{
		is_valid_expression = 1;
	}

	else if ( match(LP) )
	{
		advance(); expression();
		
		if ( match(RP) && is_valid_expression )
		{
			is_valid_expression = 1;
		}
		
		else
		{ 
			fprintf(stderr,"%llu: Missing right-parenthesis \')\'\n",
				yycharno
				);
			
			is_valid_expression = 0; 
			
			return;
		
		}	
			
	}
	
	else if ( match(NL) ) { return; }

	else // incorrect starting token for expression
	{
		fprintf(stderr,"%llu:Error: Expected either an integer-constant or"
			" left-parenthesis \'(\'\n",yycharno);

		is_valid_expression = 0;

		return;
	}
	
	advance();
	
	if ( match(NL) )
	{
		return;
	}

	else if ( match(RP) )
	{ return; }

	else if ( !isoperator(*yytext) )
	{
		fprintf(stderr,"%llu: Error: Missing operator or right-parenthesis"
			" \')\'\n",
			yycharno
			);

		is_valid_expression = 0;

		return;
	}	
	
	is_valid_expression = 0;
	
	advance();

	if ( match(NL) )
	{
		fprintf(stderr,"%llu:Error: Missing integer-constant\n",yycharno);
		 
		return; 
	}

	expression();

}

void postfix_expr(void)
{
	if ( prefix_p < &prefix[0] )
	{ putchar(0xa); return; }

	if ( isspace(*prefix_p) || ( ( *prefix_p == 0xa ) ^ ( *prefix_p == 0x0 ) ) )
	{ prefix_p--; }

	else if ( isoperator(*prefix_p) )
	{
		printf(" %c ",*prefix_p);

		prefix_p--;

		postfix_expr(); printf(" ) ");
	}

	else if ( isdigit(*prefix_p) )
	{
		while ( isdigit(*prefix_p) )
		{ putchar(*prefix_p); prefix_p--; }
	}

	postfix_expr();

}

void pop_stack(void)
{
	*prefix_p++ = *stack_p--;
}

void push_stack(uint8_t in)
{
	if ( ( op_prec(*stack_p) > op_prec(in) ) && ( stack_p >= &stack[0] ) )
	{ pop_stack(); *++stack_p = in; }

	else
	{ *++stack_p = in; }
}

void convert_expression(void)
{
	infix_p = &infix[strlen(infix)-1]; prefix_p = &prefix[0];

	while ( infix_p >= &infix_p[0] )
	{
		if ( isoperator(*infix_p) )
		{ 
			if ( *infix_p == ')' )	
			{	push_stack(*infix_p); infix_p--; }

			else if ( *infix_p == '(' )
			{
			while ( *stack_p != ')' )
			{
				pop_stack(); 
			}
			
			pop_stack();

			infix_p--;
		
			}

		else
		{ push_stack(*infix_p); infix_p--; }	
		
		}

		else if ( isdigit(*infix_p) )
		{
			while ( isdigit(*infix_p) )
			{ *prefix_p++ = *infix_p--; }

			*prefix_p++ = 0x20; // space separates integer-constants
		}

		else // isspace() or isendofline()
		{ infix_p--; }
		

	}
	
	prefix_p = &prefix[strlen(prefix)-1];

	postfix_expr();

}

void transpiler(void)
{
	while (1)
	{
		expression();
		
		if ( is_valid_expression = 1 )
		{	convert_expression();	}

		clear_all(); advance(); is_valid_expression = 0;
	}
}

int main(void)
{
//	transpiler();
	
	expression();
	
	printf("is_valid_expression: %llu\n",is_valid_expression);
		
	return 0;
}
