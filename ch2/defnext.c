#include <stdio.h>

void defnext(FILE*fp,uint8_t*name)
{
	/* Print the default yy_next(s,c) subroutine for an uncompressed table. */

	static uint8_t comment_text[]=
	{
		"yy_next(state,c) is give the current state and input character and evaluates to the next state."
	
	};

	comment(fp,comment_text);

	fprintf(fp,"#define yy_next(state,c) %s[state][c]\n",name);
}


