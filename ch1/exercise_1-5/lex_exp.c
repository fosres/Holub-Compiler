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
					

					if ( strstr(yytext,"...\0") == (char *)yytext )
					{
						return ELLIPSIS;
					}

// Storage-class specifier
					else if ( strstr(yytext,"auto\0") == (char *)yytext )
					{
						return AUTO;
					}
	
					else if ( strstr(yytext,"static\0") == (char *)yytext )
					{
						return STATIC;
					}

					else if ( strstr(yytext,"extern\0") == (char *)yytext )
					{
						return EXTERN;
					}
				
					else if ( strstr(yytext,"typedef\0") == (char *)yytext )
					{
						return TYPEDEF;
					}
					
					else if ( strstr(yytext,"const\0") == (char *)yytext )
					{
						return CONST;
					}
					
					else if ( strstr(yytext,"volatile\0") == (char *)yytext )
					{
						return VOLATILE;
					}
// type-specifiers below			
		
					else if ( strstr(yytext,"void\0") == (char *)yytext )
					{
						return VOID;
					}

					else if ( strstr(yytext,"bool\0") == (char *)yytext )
					{
						return BOOL;
					}
					
					else if ( strstr(yytext,"char\0") == (char *)yytext )
					{
						return CHAR;
					}
					
					else if ( strstr(yytext,"short\0") == (char *)yytext )
					{
						return SHORT;
					}
					
					else if ( strstr(yytext,"int\0") == (char *)yytext )
					{
						return INT;
					}
					
					else if ( strstr(yytext,"long\0") == (char *)yytext )
					{
						return LONG;
					}	
					
					else if ( strstr(yytext,"float\0") == (char *)yytext )
					{
						return FLOAT;
					}	
					
					else if ( strstr(yytext,"double\0") == (char *)yytext )
					{
						return DOUBLE;
					}	
					
					else if ( strstr(yytext,"signed\0") == (char *)yytext )
					{
						return SIGNED;
					}	
					
					else if ( strstr(yytext,"unsigned\0") == (char *)yytext )
					{
						return UNSIGNED;
					}	

					else if ( strstr(yytext,"const\0") == (char *)yytext )
					{
						return CONST;
					}

					else if ( strstr(yytext,"volatile\0") == (char *)yytext )
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

