#include "lex_exp.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

uint8_t 	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
uint64_t	yyleng		= 0; 	/* Lexeme length.		*/
uint64_t	yylineno	= 0;	/* Input line number		*/

unsigned int lex(void)
{
	static uint8_t input_buffer[1024];
	
	uint8_t * current = 0x0;

	current = yytext + yyleng;	/* Skip current lexeme */

	while (1)			/* Get the next one	*/
	{
		while ( *current == 0x0 )
		{
			/* Get new lines, skipping any leading white space on the line,
			 * until a nonblank line is found.
			 */

			current = input_buffer;
			
			if ( fgets(input_buffer,1024*sizeof(uint8_t),stdin) == NULL )
			{
				*current = 0x0;
				
				return EOI;
			}
			
			yylineno++;

			while ( isspace(*current) )
			{
				current++;
			}
		}

		for ( ; *current != 0x0 ; current++ )
		{
			/* Get the next token	*/

			yytext = current;
			yyleng = 1;

			switch(*current)
			{
			case 0xff:	return EOI	;
			case ';':	return SEMI	;
			case '=':	return EQUALS	;
			case '+':	return PLUS	;
			case '-': 	return MINUS	;
			case '*':	return TIMES	;
			case '/': 	return DIVIDE	;
			case '%': 	return MODULUS	;
			case '(':	return LP	;
			case ')':	return RP	;

			case '\n':
			case '\t':
			case ' ' : break;

			default:
				   if ( !isalnum(*current) && (*current != '_' ) )
				   {
					   fprintf(stderr,"Ignoring illegal input <%c>\n",*current); 
				   }

				   else if ( isdigit(*current) )
				   {
					while ( isdigit(*current) )
					{
						current++;	
					}

					yyleng = current - yytext;

					return NUM;
				   }

				   else // either its one of the below keywords or an identifier
				   {
					while ( isalnum(*current) || ( *current == '_' ) )
					{
						current++;
					}

					yyleng = current - yytext;

// Storage-class specifier
					if ( strcmp(current,"auto\0") == 0 )
					{
						return AUTO;
					}
	
					else if ( strcmp(current,"static\0") == 0 )
					{
						return STATIC;
					}

					else if ( strcmp(current,"extern\0") == 0 )
					{
						return EXTERN;
					}
				
					else if ( strcmp(current,"typedef\0") == 0 )
					{
						return TYPEDEF;
					}
					
					else if ( strcmp(current,"const\0") == 0 )
					{
						return CONST;
					}
					
					else if ( strcmp(current,"volatile\0") == 0 )
					{
						return VOLATILE;
					}
// type-specifiers below			
		
					else if ( strcmp(current,"void\0") == 0 )
					{
						return VOID;
					}
					
					else if ( strcmp(current,"char\0") == 0 )
					{
						return CHAR;
					}
					
					else if ( strcmp(current,"short\0") == 0 )
					{
						return SHORT;
					}
					
					else if ( strcmp(current,"int\0") == 0 )
					{
						return INT;
					}
					
					else if ( strcmp(current,"long\0") == 0 )
					{
						return LONG;
					}	
					
					else if ( strcmp(current,"float\0") == 0 )
					{
						return FLOAT;
					}	
					
					else if ( strcmp(current,"double\0") == 0 )
					{
						return DOUBLE;
					}	
					
					else if ( strcmp(current,"signed\0") == 0 )
					{
						return SIGNED;
					}	
					
					else if ( strcmp(current,"unsigned\0") == 0 )
					{
						return UNSIGNED;
					}	

//type-qualifer
					else if ( strcmp(current,"const\0") == 0 )
					{
						return CONST;
					}

					else if ( strcmp(current,"volatile\0") == 0 )
					{
						return VOLATILE;
					}
					
					return ID;
				   }

				   break;

			}

		}

	}

}

static uint8_t Lookahead = 0xff;

bool match(uint8_t token)
{
	//Return true if "token" matches current lookahead symbol

	va_start 	
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

