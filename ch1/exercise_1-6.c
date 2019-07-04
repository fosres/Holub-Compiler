#if 0
Copyright 2019-INFINITY. Tanveer Salim

Transpiler Grammar:

expression -> NUM |
		( expression operator_opt expression_opt )

Transpiler Lexicon:

operand ==  NUM -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

operator -> + | - | * | / | ( | )

Infix To Prefix Algorithm:

This transpiler  reads input line by line and for each line outputs the LISP prefix

expression.

Step 1: Read the given infix expression into input buffer ( MAX LENGTH 1024 ).

Step 2: Reading the input string backwards one character at a time:

	If the read character is an operand, then concatenate the operand to

		the prefix string.
	
	Else the read character is an operator. You must:
	
		1. First check if the stack is empty.

		2. If so, then check if the top operator in the stack has a
			
		higher precedence than the read operator.

			A. If so, then while the top operator has higher precedence

			OR 

			while the top operator is NOT a right-parenthesis:
				
				1. Pop the stack and concetanate what was just popped
			
				to the prefix string.

			B. Else, just push operator into the stack, even for a right
			
			parenthesis.

		3. If the read operator is a left-parenthesis:

			A. Pop the stack and concatenate what was just popped

			UNTIL

			the top stack operator is a right-parenthesis.

			B. Now, pop the right-parenthesis off the stack

			BUT

			DO 

			NOT

			concatenate the right-parenthesis to the prefix

			string.

Step 3: Repeat Step 2 until all characters in the infix string are read.

Step 4: If the stack is not empty, then pop the operator from the stack and

add this operator to the prefix string,

UNLESS IT IS A RIGHT PARENTHESIS.

Step 5: Repeat Step 4 until until THE STACK IS EMPTY.

Step 6: While reading the PREFIX string backwards AND skipping whitespace characters:
	
	A. 
		1. If an operator, print " ( operator-here " 

		2. Call two consecutive, recursive calls to 

		the function processing the prefix string.

		3. Print " ) "

	B. Else if an operand, print the entire operand 

#endif


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
		case ')':
		case '(':
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
	if ( 
		!is_stack_empty() 
		
		&& ( stack_top() != ')' ) 
		
		&& ( op_prec(stack_top() ) > op_prec(in) ) 
	
	)
	
	{ 
			while ( 
				
				!is_stack_empty() 
				
				&& ( op_prec(stack_top() ) > op_prec(in) ) 

				&& ( ( stack_top() != ')' ) )
			)
			{	pop_stack();	}

	}

	 *++stack_p = in; 
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
			
				if ( stack_p >= &stack[0] )
				{		
					pop_stack(); //get rid of the topmost right-parenthesis
				}

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
	
	prefix_p = &prefix[strlen(prefix)-1];

	printf("\nPrefix: ");

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

	return 0;	
}
