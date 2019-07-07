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
					
					
					*yytext = *yycurrent = 0x0;
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

void clear_all(void)
{
	memset(infix,0x0,1025);

	memset(prefix,0x0,2050);
	
	memset(stack,0x0,513);

	infix_p = &infix[0];

	prefix_p = &prefix[0];

	stack_p = stack - 1;

	yycurrent = &prefix[0];

	yytext = &prefix[0];

	yyleng = 0;
	
}

void carriage_return(void)
{
	infix_p = &infix[0];

	prefix_p = &prefix[0];

	stack_p = stack - 1;

	memset(stack,0x0,513);
	
	yycurrent = &prefix[0];

	yytext = &prefix[0];

	yyleng = 0;

}

uint8_t stack_top(void)
{
	return *stack_p;
}

uint8_t push_stack(uint8_t in)
{
	*++stack_p = in;
}


uint8_t pop_stack(void)
{
	return *stack_p--;
}

void prefix_expr(void)
{
	if ( match(NL) )
	{	is_valid_expression = 0; return;	}

	if ( !match(LP) && !match(NUM) )
	{	
		fprintf(stderr,"%llu: Error: Expected a left-parenthesis or"

		" integer constant\n",
		
		yycurrent - &prefix[0]
		);

		is_valid_expression = 0;

		return;
	}

	
	if ( match(LP) )
	{
		advance();
		
		if ( !isoperator(*yycurrent) )
		{
			fprintf(stderr,"%llu: Missing operator\n",
				yycurrent - &prefix[0]
				);
			
			is_valid_expression = 0;

			return;
 		}

		if ( isoperator(*yycurrent) )
		{
			advance();
			
			if ( match(NL) ) 
			{
				fprintf(stderr,"%llu: Error: Missing integer-constant"
						" or right-parenthesis\n",
						yycurrent - &prefix[0]
					);
				
				is_valid_expression = 0;
					
				return;
			}
	
			prefix_expr();
			
			if ( is_valid_expression == 0 )
			{ 	return;		}
	
			advance();

			if ( match(NL) ) 
			{
				fprintf(stderr,"%llu: Error: Missing integer-constant"
						" or right-parenthesis\n",
						yycurrent - &prefix[0]
					);
				
				is_valid_expression = 0;
					
				return;
			}
			
			if ( !match(NUM) && !match(LP) )
			{
				fprintf(stderr,"%llu: Error: Missing integer-constant"
						" or right-parenthesis\n",
						yycurrent - &prefix[0]
					);
				
				is_valid_expression = 0;
					
				return;
			}	

			while ( match(NUM) || match(LP) )
			{
				prefix_expr();
				
				if ( is_valid_expression == 0 )
				{	return;		}
	
				advance();

			}
			
			if ( match(NL) )
			{	
				is_valid_expression = 0; 
				
				fprintf(stderr,"%llu: Error: Expected integer-constant"
					" or left-parenthesis\n",
					yycurrent - &prefix[0]
					);
				
				return;	
			
			}

		
		}

		if ( !match(RP) || !is_valid_expression )
		{	
			is_valid_expression = 0;	

			if ( !match(RP) )
			{
				fprintf(stderr,"%llu: Error: Missing right parenthesis\n",
					yycurrent - &prefix[0]
					);
			}

			return;
		}

	}		
	
	is_valid_expression = 1;	
	
}



void iterative_LISP_to_infix(void)
{
	infix_p = &infix[0];

	prefix_p = &prefix[0];

	stack_p = stack - 1;

	memset(stack,0x0,513);
	
	yycurrent = &prefix[0];

	yytext = &prefix[0];

	yyleng = 0;

	advance();	
	
	while ( !match(NL) )
	{
		
		if ( match(LP) )
		{	
			printf("( ");	

			advance();
		}	

		else if ( match(RP) )
		{
					
			printf(") ");	

			advance();

			if ( match(NUM) || match(LP) )
			{	putchar(stack_top());		}

			else if ( match(RP) )
			{	pop_stack();	}
			
			putchar(0x20);
			
		}

		else if ( isoperator(*yycurrent) )
		{
			push_stack(*yycurrent);

			advance();
		}

		else if (  match(NUM) )
		{
			uint8_t * num_p = yytext;

			while ( num_p < yycurrent )
			{		putchar(*num_p); num_p++;	}
			
			
			putchar(0x20);
			
			advance();
			
			if ( match(NUM) || match(LP) )
			{	
				putchar(stack_top());

				putchar(0x20);	
			}

			else if ( match(RP) )
			{
				pop_stack();
				
				// putchar(0x20);
			}
		}
		
	}
		
	putchar(0xa);	
}

int main(void)
{
	while ( 1 )
	{
		prefix_expr();
		

		if ( is_valid_expression )
		{
			printf("Infix: ");
			
			iterative_LISP_to_infix();

		}
			
		clear_all(); 
		
		advance();
	}
	
	return 0;
}
