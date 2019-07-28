#include "lex_exp.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

uint8_t 	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
uint8_t 	*yycurrent	= 0x0;
uint64_t	yyleng		= 0; 	/* Lexeme length.		*/
uint64_t	yylineno	= 0;	/* Input line number		*/
uint8_t		input[1024]; /*		stores line input		*/
uint64_t 	Lookahead 	= 0xff;
FILE 	*	input_file	= 0x0;

uint64_t lex(void)
{

	yycurrent = yytext + yyleng;	/* Skip yycurrent lexeme */

	while (1)			/* Get the next one	*/
	{
		while ( *yycurrent == 0x0 )
		{
			/* Get new lines, skipping any leading white space on the line,
			 * until a nonblank line is found.
			 */

			yycurrent = input;
			
		if ( input_file == NULL )
		{		
			if ( fgets(input,1024*sizeof(uint8_t),stdin) == NULL )
			{
				*yycurrent = 0x0;
				
				return EOI;
			}
		}

		else
		{
			if ( fgets(input,1024*sizeof(uint8_t),input_file) == NULL )
			{
				*yycurrent = 0x0;
				
				return EOI;
			}

		}	
			yylineno++;

			while ( isspace(*yycurrent) )
			{
				yycurrent++;
				
			}
		}

		while ( isspace(*yycurrent) )
		{
			yycurrent++;
				
		}

		for ( ; *yycurrent != 0x0 ; yycurrent++  )
		{
			/* Get the next token	*/

			yytext = yycurrent;
			yyleng = 1;

			switch(*yycurrent)
			{
			case 0xff:	return EOI	;
			case ';':	return SEMI	;
			case '=':	return EQUALS	;
			case '+':	return PLUS	;
			case '-': 	return MINUS	;
			case '*':	return ASTK	;
			case '/': 	return DIVIDE	;
			case '%': 	return MODULUS	;
			case '(':	return LP	;
			case ')':	return RP	;
			case '[':	return LB	;
			case ']':	return RB	;

			case '\n':
			case '\t':
			case ' ' : break;

			default:
			{
				   if ( !isalnum(*yycurrent) && (*yycurrent != '_' ) )
				   {
					  fprintf(stderr,"%llu:%llu:Error:Invalid lexeme: Neither starts with an alphanumeric character nor \'_\'\n",
						  yylineno,yytext-&input[0]
						 ); 
					   
					   return INV;
				   }

				   else if ( isdigit(*yycurrent) )
				   {
					while ( isdigit(*yycurrent) )
					{
						yycurrent++;	

						
					}

					yyleng = yycurrent - yytext;

					return NUM;
				   }

				   else // either its one of the below keywords or an identifier
				   {
					while ( isalnum(*yycurrent) || ( *yycurrent == '_' ) )
					{
						yycurrent++;
						
						
					}


					yyleng = yycurrent - yytext;
					

					if ( strcmp(yytext,"...\0") == 0 )
					{
						return ELLIPSIS;
					}

// Storage-class specifier
					else if ( strcmp(yytext,"auto\0") == 0 )
					{
						return AUTO;
					}
	
					else if ( strcmp(yytext,"static\0") == 0 )
					{
						return STATIC;
					}

					else if ( strcmp(yytext,"extern\0") == 0 )
					{
						return EXTERN;
					}
				
					else if ( strcmp(yytext,"typedef\0") == 0 )
					{
						return TYPEDEF;
					}
					
					else if ( strcmp(yytext,"const\0") == 0 )
					{
						return CONST;
					}
					
					else if ( strcmp(yytext,"volatile\0") == 0 )
					{
						return VOLATILE;
					}
// type-specifiers below			
		
					else if ( strcmp(yytext,"void\0") == 0 )
					{
						return VOID;
					}

					else if ( strcmp(yytext,"bool\0") == 0 )
					{
						return BOOL;
					}
					
					else if ( strcmp(yytext,"char\0") == 0 )
					{
						return CHAR;
					}
					
					else if ( strcmp(yytext,"short\0") == 0 )
					{
						return SHORT;
					}
					
					else if ( strcmp(yytext,"int\0") == 0 )
					{
						return INT;
					}
					
					else if ( strcmp(yytext,"long\0") == 0 )
					{
						return LONG;
					}	
					
					else if ( strcmp(yytext,"float\0") == 0 )
					{
						return FLOAT;
					}	
					
					else if ( strcmp(yytext,"double\0") == 0 )
					{
						return DOUBLE;
					}	
					
					else if ( strcmp(yytext,"signed\0") == 0 )
					{
						return SIGNED;
					}	
					
					else if ( strcmp(yytext,"unsigned\0") == 0 )
					{
						return UNSIGNED;
					}	

					else if ( strcmp(yytext,"const\0") == 0 )
					{
						return CONST;
					}

					else if ( strcmp(yytext,"volatile\0") == 0 )
					{
						return VOLATILE;
					}

					else
					{
						return ID;
					}
					
				   }

				   break;
			}

			}

		}

	}

}


bool match(uint64_t token)
{
	//Return true if "token" matches yycurrent lookahead symbol

	if ( Lookahead == 0xff )
	{
		Lookahead = lex();
	}

	return token == Lookahead;
}

void advance(void)
{
	//Advance lookahead to next symbol
	
	Lookahead = lex();
}

