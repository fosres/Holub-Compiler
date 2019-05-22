#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "lex.h"

uint8_t *yytext = "";		

uint64_t yyleng = 0;

uint64_t yylineno = 0;

unsigned int lex(void)
{
	static uint8_t input_buffer[1024];

	static uint8_t * input_buf_p;

	input_buf_p = yytext + yyleng; //initial value of *current == 0x0

	while (1)
	{
#if 0
In while loop below:

Get new lines, skipping any leading white space on the line,

until a nonblank line is found
#endif	
		
		while ( *input_buf_p == 0x0 )
		{
			if ( fgets(input_buffer,1024*sizeof(char),stdin) == 0x0 )
			{
				*input_buf_p = 0x0;

				return EOI;
			}

			yylineno++;

			while ( isspace(*input_buf_p) )
			{
				input_buf_p++;
			}	
		}

		for ( ; *input_buf_p != 0x0 ; input_buf_p++ )
		{
			// Getting next token for next time lex() is called

			yytext = input_buf_p;

			yyleng = 1;

			switch(*input_buf_p)
			{
				case 0xff: {return EOI;}
				case ';': {return SEMI;}
				case '+': {return PLUS;}
				case '*': {return TIMES;}
				case '(': {return LP;}
				case ')': {return RP;}

				case '\n':
				case '\t':
				case ' ': {break;}

				default:
				{
					if ( !isalnum(*input_buf_p) )
					{
						fprintf(stderr,"Ignoring illegal inpt <%c>\n",*input_buf_p);
					}

					else
					{
						while ( isalnum(*input_buf_p) )
						{
							input_buf_p++;
						}

						yyleng = input_buf_p - yytext;

						return NUM_OR_ID;
					}
					
					break;
				}
			}
		}

	}

	

}
static uint8_t Lookahead = 0xff;

uint8_t match(uint8_t token)
{
	//Return true if "token" matches current lookahead symbol
	
	if ( Lookahead == 0xff )
	{
		Lookahead = lex();

		return token == Lookahead;
	}
}

void advance()
{
	//Advance lookahead to next symbol
	
	Lookahead = lex();
}


int main(void)
{
	return 0;
}


