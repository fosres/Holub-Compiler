#include "lex_exp.h"
#include <stdio.h>
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

				   else // identifier
				   {
					while ( isalnum(*current) || ( *current == '_' ) )
					{
						current++;
					}

					yyleng = current - yytext;

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

