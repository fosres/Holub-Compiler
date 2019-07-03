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

size_t op_prec(uint8_t token)
{
	switch(token)
	{
		case ')':
		case '(':
		{
			return 2;
		}

		case '*':
		case '/':
		case '%':
		{
			return 1;
		}
		
		case '+':
		case '-':
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

	yycurrent = &infix[0];

	yytext = &infix[0];

	yyleng = 0;
	
}

uint8_t stack_top(void)
{
	return *stack_p;
}

bool is_stack_empty(void)
{	return ( stack_p < &stack[0] );	}


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
			fprintf(stderr,"Error: Missing right-parenthesis \')\'\n"
				);
			
			is_valid_expression = 0; 
			
			return;
		
		}	
			
	}

	else // incorrect starting token for expression
	{
		fprintf(stderr,":Error: Expected either an integer-constant or"
			" left-parenthesis \'(\'\n");

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
		fprintf(stderr,"Error: Missing operator or right-parenthesis"
			" \')\'\n"
			);

		is_valid_expression = 0;

		return;
	}	
	
	is_valid_expression = 0;
	
	advance();

	if ( match(NL) )
	{
		fprintf(stderr,"Error: Missing integer-constant\n");
		 
		return; 
	}

	expression();

}

void prefix_expr(void)
{
	if ( prefix_p < &prefix[0] )
	{ putchar(0xa); return; }
	
	if ( isspace(*prefix_p) )
	{
		while ( isspace(*prefix_p)  )
		{ prefix_p--; }
	}

	if ( isoperator(*prefix_p) )
	{
		printf(" ( %c ",*prefix_p);

		prefix_p--;

		prefix_expr(); 

		prefix_expr();
		
		printf(" ) ");
	}

	else if ( isdigit(*prefix_p) )
	{
		while ( isdigit(*prefix_p) )
		{ putchar(*prefix_p); prefix_p--; }
		
		putchar(0x20);
#if 0	
		while ( isspace(*prefix_p) )
		{	prefix_p--;	}
		
		if ( isdigit(*prefix_p) )
		{
			while ( isdigit(*prefix_p) )
			{ putchar(*prefix_p); prefix_p--; }
			
			putchar(0x20);			
			return;
		
		}
#endif
	}

//	prefix_expr(); //unnecessary since second prefix_expr in operator conditional

}

void pop_stack(void)
{
	if ( stack_top() != ')' )
	{
		*prefix_p++ = *stack_p--;
	}
	
	else
	{
		stack_p--;	
	}
}

void push_stack(uint8_t in)
{
	if ( !is_stack_empty() && ( op_prec(stack_top() ) > op_prec(in) ) )
	{ 
		if ( stack_top() != ')' )
		{
			while ( !is_stack_empty() && ( op_prec(stack_top() ) > op_prec(in) ) )
			{	pop_stack();	}
		}

		else
		{	pop_stack();	}

		*++stack_p = in; 
	}

	else
	{ *++stack_p = in; }
}

void convert_expression(void)
{
	infix_p = &infix[strlen(infix)-1]; prefix_p = &prefix[0];
	
	while ( infix_p >= infix )
	{
		if ( isoperator(*infix_p) )
		{ 

			if ( *infix_p == '(' )
			{
				while ( stack_top() != ')' && !is_stack_empty() )
				{
					pop_stack(); 
				}
			
				pop_stack(); //get rid of the topmost right-parenthesis

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

		else // isspace() or 0x0
		{ infix_p--; }
		

	}
	
	while ( !is_stack_empty() )
	{	pop_stack();	}
	
	printf("\nStack: %s\nReverse Prefix: %s\n",stack,prefix);	
	
	prefix_p = &prefix[strlen(prefix)-1];

	printf("Prefix: ");

	prefix_expr();

	putchar(0xa); putchar(0xa);


}

void transpiler(void)
{
	while (1)
	{
		expression();
		
		if ( is_valid_expression == 1 )
		{	convert_expression();	}

		clear_all();

		advance(); 
		
		is_valid_expression = 0;
	}
}

int main(void)
{
	transpiler();

#if 0
	while ( 1 )
	{
		expression();

		printf("is_valid_expression:%llu %s\n%s\n",is_valid_expression,infix,prefix);
		clear_all(); 
		
		advance();

		is_valid_expression = 0;
	}
#endif	
	return 0;	
}
