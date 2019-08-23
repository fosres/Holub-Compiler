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
	
}
