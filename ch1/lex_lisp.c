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
uint64_t 	yycharno 	= 0; 	/* Input char number		*/

uint64_t lex(void)
{

		yycurrent = yytext + yyleng;	/* Skip yycurrent lexeme */

		while ( *yycurrent == 0x0  )
		{
			/* Get new lines, skipping any leading white space on the line,
			 * until a nonblank line is found.
			 */

			yycurrent = infix;

//infix is external
			
			if ( fgets(infix,1024*sizeof(uint8_t),stdin) == NULL )
			{
				*yycurrent = 0x0;
				
				return EOI;
			}
			
			yylineno++;

			yycharno = 0;


			while ( isspace(*yycurrent) )
			{
				yycurrent++;
				
				yycharno++;
			}
		}

		while (isspace(*yycurrent) )
		{ yycurrent++; yycharno++; }

		for ( ; *yycurrent != 0x0 ; yycurrent++,yycharno++ )
		{
			/* Get the next token	*/

			yytext = yycurrent;
			yyleng = 1;

			switch(*yycurrent)
			{
			case 0xff:	return EOI	;
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

				   if ( isdigit(*yycurrent) )
				   {
					while ( isdigit(*yycurrent) )
					{
						yycurrent++;	

						yycharno++;
					}

					yyleng = yycurrent - yytext;

					return NUM;
				   }

				   else // either its one of the below keywords or an identifier
				   {
					fprintf(stderr,"%llu:Error: Invalid"
" lexeme\n",yycharno);
				
				   }

				   break;
			}

			}

		}
	
	lex();
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

