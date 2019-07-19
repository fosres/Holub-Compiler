#include "lex_exp.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define NL	0xa	/* Newline character */
uint8_t 	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
uint8_t 	*yycurrent	= 0x0;
uint64_t	yyleng		= 0; 	/* Lexeme length.		*/
uint64_t	yylineno	= 0;	/* Input line number		*/

uint64_t lex(void)
{
	uint8_t input_buffer[1024];

	yycurrent = yytext + yyleng;	/* Skip yycurrent lexeme */

	while (1)			/* Get the next one	*/
	{
		while ( *yycurrent == 0x0 )
		{
			/* Get new lines, skipping any leading white space on the line,
			 * until a nonblank line is found.
			 */

			yycurrent = input_buffer;
			
			if ( fgets(input_buffer,1024*sizeof(uint8_t),stdin) == NULL )
			{
				*yycurrent = 0x0;
				
				return EOI;
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

			case '\n':
			case '\t':
			case ' ' : break;

			default:
			{
				   if ( !isalnum(*yycurrent) && (*yycurrent != '_' ) )
				   {
					   fprintf(stderr,"Ignoring illegal input <%c>\n",*yycurrent); 
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

					if ( strcmp(yycurrent,"...\0") == 0 )
					{
						return ELLIPSIS;
					}

// Storage-class specifier
					else if ( strcmp(yycurrent,"auto\0") == 0 )
					{
						return AUTO;
					}
	
					else if ( strcmp(yycurrent,"static\0") == 0 )
					{
						return STATIC;
					}

					else if ( strcmp(yycurrent,"extern\0") == 0 )
					{
						return EXTERN;
					}
				
					else if ( strcmp(yycurrent,"typedef\0") == 0 )
					{
						return TYPEDEF;
					}
					
					else if ( strcmp(yycurrent,"const\0") == 0 )
					{
						return CONST;
					}
					
					else if ( strcmp(yycurrent,"volatile\0") == 0 )
					{
						return VOLATILE;
					}
// type-specifiers below			
		
					else if ( strcmp(yycurrent,"void\0") == 0 )
					{
						return VOID;
					}

					else if ( strcmp(yycurrent,"bool\0") == 0 )
					{
						return BOOL;
					}
					
					else if ( strcmp(yycurrent,"char\0") == 0 )
					{
						return CHAR;
					}
					
					else if ( strcmp(yycurrent,"short\0") == 0 )
					{
						return SHORT;
					}
					
					else if ( strcmp(yycurrent,"int\0") == 0 )
					{
						return INT;
					}
					
					else if ( strcmp(yycurrent,"long\0") == 0 )
					{
						return LONG;
					}	
					
					else if ( strcmp(yycurrent,"float\0") == 0 )
					{
						return FLOAT;
					}	
					
					else if ( strcmp(yycurrent,"double\0") == 0 )
					{
						return DOUBLE;
					}	
					
					else if ( strcmp(yycurrent,"signed\0") == 0 )
					{
						return SIGNED;
					}	
					
					else if ( strcmp(yycurrent,"unsigned\0") == 0 )
					{
						return UNSIGNED;
					}	

//type-qualifer
					else if ( strcmp(yycurrent,"const\0") == 0 )
					{
						return CONST;
					}

					else if ( strcmp(yycurrent,"volatile\0") == 0 )
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

}


uint64_t Lookahead = 0xff;

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

