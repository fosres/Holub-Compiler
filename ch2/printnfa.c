#include <stdio.h>
#include "nfa.h"
#include "set.h"
static void printccl(SET*);

static char *plab(NFA*,NFA*);

void print_nfa(NFA*,size_t,NFA*);

/*---------------------------------
 * PRINTNFA.C Routine to print out a NFA structure in human-readable form.
 */

static void printccl(SET*set)
{
	static size_t i=0;

	putchar('[');

	for(i=0;i<=0x7f;i++)
	{
		if(TEST(set,i))
		{
			if(i<' ')
			{
				printf("^%c",i+'@');
			}

			else
			{
				printf("%c",i);
			}
		}
	}

	putchar(']');
}

/*---------------------------------------------*/

static uint8_t *plab(NFA*nfa,NFA*state)
{
	/*Return a pointer to a buffer containing the state number. The
	 *buffer is overwritten on each call so don't put more than one
	 *plab() call in an argument to printf().
	 */

	static char buf[32];

	if(!nfa||!state){return "--";

	snprintf(buf,"%2d",32,state-nfa);

	return buf;	
}

/*---------------------------------------------*/

void print_nfa(NFA*nfa,size_t len,NFA*start)
{
	NFA*s=nfa;printf("\n------------------NFA------------\n");

	for(;--len>=0;nfa++)
	{
		printf("NFA state %s: ",plab(s,nfa));

		if(!nfa->next)
		{
			printf("(TERMINAL)");
		}

		else
		{
			printf("--> %s ",plab(s,nfa->next));

			printf("(%s) on ",plab(s,nfa->next2));

			switch(nfa->edge)
			{
				case CCL:{printccl(nfa->bitset); break;}

				case EPSILON:{printf("EPSILON ");break;}

				default:{pchar(nfa->edge,stdout);break;}
			}
		}
		
		if(nfa==start)
		{printf(" (START STATE)");}

		if(nfa->accept){printf(" accepting %s<%s>%s",nfa->anchor&START?"^":"",						nfa->accept,nfa->anchor&END?"$":"");}
		printf("\n");	
	}

	printf("\n-------------------------\n");
}
