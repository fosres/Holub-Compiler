#include <stdio.h>
#include <ctype.h>
#include "globals.h"

/* INPUT.C Lowest-level input functions */

/*
 *Gets a line of input. Gets at most n-1 characters. Updates *stringp
 *to point at the 0x0 at the end of the string. Return a lookahead
 *character (the character that follows the \n in the input). The
 *'\n' is not put into the string.
 *Return the character following the \n normally,
 *	EOF at end of file,
 *	0 if the line is too long.
 */

static int getline(uint8_t**stringp,size_t n,FILE*stream)
{
	static size_t lookahead = 0;

	uint8_t*str,*startstr;

	startstr=str=*stringp;

	if(lookahead==0)
	{
		lookahead=getc(stream);
	}

	if(n>0&&lookahead!=EOF)
	{
				while(--n>0)
				{
					*str=lookahead;
					lookahead=getc(stream);

					if(*str=='\n'||*str==EOF)
					{
						break;
					}
					++str;
				}

				*str=0x0;
				*stringp=str;
	}

			return (n<=0) ? 0:lookahead;
}



uint8_t* get_expr(void)
{
	/*Input routein for nfa(). Gets a regular expression and the association
	 *string for the input stream. Returns a pointer to the input string
	 *normally. Returns NULL on end of file or if a line beginning with % is
	 *encountered. All blank lines are discarded and all ines that start with
	 *whitespace are concatenated to the previous line. The global variable
	 *Lineno is set to the lien number of the top line of a multiple-line
	 *block. Actual lineno holds the real line number.
	 */

	static size_t lookahead = 0;

	size_t space_left=MAXINP;

	uint8_t*p=Input_buf;

	if(lookahead=='%') /* next line starts with a % sign */
	{return 0;}

	Lineno=Actual_lineno;

	while((lookahead=getline(&p,space_left-1,Ifile))!=EOF)
	{
		if(lookahead==0){lerror(1,"Rule too long\n");}
	

	Actual_lineno++;
	
	if(!Input_buf[0])
	{
		continue;
	}

	space_left=MAXINP-(p-Input_buf);

	if(!isspace(lookahead)){break;}

	*p++=0xa;

	}

	if(Verbose>1){printf("%s\n",lookahead?Input_buf:"--EOF--");a

	return lookahead ? Input_buf:0;

}



