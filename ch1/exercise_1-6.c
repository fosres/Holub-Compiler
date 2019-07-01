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

uint8_t * yytext = 0x0;

uint8_t * yycurrent = 0x0;

uint64_t yyleng = 0;

uint64_t yycharno = 0;

uint64_t Lookahead = 0xff;

static bool is_valid_expression;

bool isendofline(void)
{
	return ((*infix_p == 0x0) ^ (*infix_p == 0xa));		
}

bool isEOI(void)
{
	return (*infix_p == 0xff);
}

void advance(void)
{
	Lookahead = lex();
}

bool match(uint64_t token)
{
	if ( Lookahead == 0xff )
	{ Lookahead = lex(); }
	
	return ( Lookahead == token );
}

bool isoperator(uint8_t in)
{
	switch (Lookahead)
	{
		case PLUS:
		case MINUS:
		case ASTK:
		case DIVIDE:
		case MODULUS:
		{
			return 1;
		}

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

uint64_t lex(void) //returns next token from stdin
{
	if ( isendofline() )
	{
		if ( fgets(infix,1024,stdin) == NULL )
		{
			fprintf(stderr,"Error: Failed to get next line\n");
			
			exit(1);
		}
		
		yycharno = 0;

		infix_p = &infix[0];
	}
	

	if ( *infix_p == 0xff ) { exit(1); }

	if ( *infix_p == 0xa ) { goto newline; }

	while ( !isendofline() && !isEOI() )
	{
		while ( isspace(*infix_p) && !isendofline() && !isEOI() )
		{ infix_p++; yycharno++; }

		if ( isEOI() ) { exit(1); }

		yytext = yycurrent = infix_p;

		if ( isendofline() ) { goto newline; }

//!isspace(*infix_p) && !isendofline() && !isEOI() )

		while ( isdigit(*infix_p) ) 		
		{ *yycurrent++ = *infix_p++; yycharno++; }

		if ( isEOI() )
		{ exit(1); }

		yyleng = yycurrent - yytext;

		switch(*yytext)
		{
			case 0xff: {exit(1); break; }
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				return NUM;
			}

			case '+':
			{
				return PLUS;
			}

			case '-':
			{
				return MINUS;
			}
			
			case '*':
			{
				return ASTK;
			}

			case '/':
			{
				return DIVIDE;
			}

			case '%':
			{
				return MODULUS;
			}

			default:
			{
				fprintf(stderr,"%llu:Error: Invalid lexeme\n",yycharno);
				
				goto newline;

				break;

			}

		}		
	}

	newline:
	lex();
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
		
		if ( match(RP) )
		{
			is_valid_expression = 1;

		}
			if ( isendofline() ) { return; }
	
	}

	else // incorrect starting token for expression
	{
		fprintf(stderr,"%llu:Error: Expected either an integer-constant or"
			"left-parenthesis \'(\'\n",yycharno);

		is_valid_expression = 0;

		return;
	}

	if ( isendofline() )
	{
		return;
	}

	advance();

	if ( match(RP) )
	{ return; }
	
	else if ( !isoperator(*infix_p) )
	{
		fprintf(stderr,"%llu:Error:Missing operator or right-parenthesis \')\'\n",
			yycharno);

		is_valid_expression = 0; return;
	}

	is_valid_expression = 0;

	if ( isendofline() )
	{ return; }

	advance();

	expression();

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
			{ *prefix_p++ = *infix_p==; }

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
	return 0;
}
