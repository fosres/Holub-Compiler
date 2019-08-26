#include <stdio.h>

#define NCOLS	10
#define TYPE	"YY_TYPE"
#define SCLASS	"YYPRIVATE"
#define D_SCLASS"YYPRIVATE"

/* PAIRS.C This module contains the routines to compress a table horizontally
 * 	   (using char/next pairs) and then print the compressed table. The
 * 	   compressed table. The compressed array looks like this:
 *
 * 	   Yy_next:	Yy_nxtDD:
 * 	   +------+	+-------+----------------------------------------+
 * 	   |   *--|---->|	| Next state array, indexed by character |
 * 	   +------+	+-------+----------------------------------------+
 *	   |	  |
 *	   +------+	+-------+-------------------------+
 *	   |   *--|---->| count | c1 | s1 | c2 | s2 | ... |
 *	   +------+	+-------+-------------------------+
 *         |      |
 *	   +------+
 *         | NULL |
 *	   +------+
 */

typedef size_t atype;


/*--------------------------------------------------------------*/

#if 0
Generate the C source code for a pair-compressed DTRAN. Returns the

number of cells used for the YySDD arrays. The "numbers" argument

determines the output format of the character part of a character/

next-state pair. If numbers is true, then normal numbers are used,

otherwise ASCII characters are used. For example: 'a',100 as

compared to: 97,100 ('a'==0x61 == 97, decimal)
#endif

uint8_t*bin_to_ascii(size_t c,size_t use_hex)
{
	static unsigned char buf[8];

	c&=0xff;

	if(' '<=c && c < 0x7f && c!='\'' && c!='\\')
	{
		buf[0]=c;

		buf[1]=0x0;
	}

	else
	{
		buf[0]='\\';

		buf[1]='\0';

		switch(c)
		{
			case '\\':buf[1]='\\';break;
			case '\'':buf[1]='\'';break;
			case '\b':buf[1]='b';break;
			case '\f':buf[1]='f';break;
			case '\t':buf[1]='t';break;
			case '\r':buf[1]='r';break;
			case '\n':buf[2]='n';break;
			default:sprintf(&buf[1],use_hex?"x%03x":"%03o",c);break;
		}
	}

	return buf;
}

#if 0
Print an argv-like array of pointers to strings, one string per line.
The array must be NULL terminated.
#endif

void printv(FILE*fp,uint8_t**argv)
{
	while(*argv)
	{
		fprintf(fp,"%s\n",*argv++);
	}	
}

//Works like printv except that the array is printed as a C comment.

void comment(FILE*fp,uint8_t**argv)
{
	fprintf(fp,"\n/*-----------------------------------------\n");

	while(*argv){fprintf(fp," * %s\n",*argv++);

	fprintf(fp," */\n\n");	
}

size_t pairs(FILE*fp,atype*array,size_t nrows,size_t ncols,uint8_t *name,size_t threshold,size_t numbers)
{
	size_t i=0,j=0,ntransitions=0,size_t nprinted,ncommas=0;

	size_t num_cells=0;atype*p=0;

	for (i=0;i<nrows;i++)
	{
		ntransitions = 0;

		for(p=array+(i*ncols),j=ncols;--j>=0;p++)
		{
			if(*p!=(~0))
			{
				++ntransitions;	
			}
		}

		if(ntransitions>0)
		{
			fprintf(fp,"%s %s %s%-2d[]={",SCLASS, TYPE, name,i);

			++num_cells;

			if(ntransitions>threshold)
			{
				fprintf(fp,"0,\n");
			}

			else
			{
				fprintf(fp,"%2d, ",ntransitions);

				if(threshold>5)
				{
					fprintf(fp,"\n		");
				}
			}

			nprinted=NCOLS;

			ncommas=ntransitions;

			for(p=array+(i*ncols),j=0;j<ncols;j++,p++)
			{
				if(ntransitions>threshold)
				{
					++num_cells;

					--nprinted;

					fprintf(fp,"%3d",*p);

					if(j<ncols-1)
					{
						fprintf(fp,", ");	
					}	
				}

				else if(*p!=(~0))
				{
					num_cells+=2;

					if(numbers)
					{
						fprintf(fp,"%d,%d",j,*p);
					}

					else
					{
						fprintf(fp,"'%s',%d",bin_to_ascii(j,0),*p);
					}
				}
			}
		}		
	}
		
}

//Print out a next(state,c) subroutine for a table compressed
//into char/next-state pairs by the routines in pairs.c

void pnext(FILE*fp,uint8_t*name)
{
	static uint8_t*toptext[]=
	{
		"unsigned int c;",
		"int cur_state;",
		"{",
		"	/* Given the current state and the current input character, return",
		"	/* Given the current state and the current input character, return",
		"	* the next state.",
		"	*/",
		"",
		0
	};

	static uint8_t*boptext[]=
	{
		"	int i;",
		"",
		"	if( p )",
		"	{",
		"	if(i = *p++) == 0 )",
		"	return p[c];",
		"",
		"	for(;--i>=0;p+=2)",
		"	if(c==p[0])",
		"	return p[1];",
		"	}",
		"	return YYF;",
		"}",
		0
	};

	fprintf(fp,"\n/*----------------------------*/\n");

	fprintf(fp,"%s %s yy_next(cur_state,c)\n",D_SCLASS,TYPE);

	printv(fp,toptext);	
}
