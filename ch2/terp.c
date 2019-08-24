#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "nfa.h"
#include "set.h"
#include "globals.h"
/*---------------------------------------------
 * Prototypes for subroutines in this site.
 *
 */

size_t nfa (uint8_t*(*)());

void free_nfa(void);

SET * e_closure(SET*,char**,int*);

SET *move(SET*,int);

/*--------------------------------------------------------*/

#define LARGEST_INT (~0)

static NFA *Nfa; /*Base address of NFA array */

static size_t Nfa_states; /* Number of states in NFA */

/*---------------------------------------------------------*/

/* Compile the NFA and initialize the various global variables
 * used by move() and e_closure(). Return the state number
 * (index) of the NFA state. This routine must be called before
 * either e_closure() or move() are called. The memory used for
 * the nfa can be freed with free_nfa() (in thompson.c)
 */

size_t nfa(uint8_t*(*input_routine)())
{
	NFA*sstate=0;Nfa=thompson(input_routine,&Nfa_states,&sstate);

	return sstate-Nfa;	
}

/*----------------------------------------------------------*/

void free_nfa(void)
{
	free_nfa();
}

SET*e_closure(SET*input,uint8_t**accept,size_t*anchor)
{
	/* input is the set of start states to examine
	 * accept is modified to point at the string associated with an accepting
	 * state (or to NULL if the state is not an accepting state.
	 * *anchor is modified to hold the anchor point, if any.
	 *
	 * Computes the epsilon closure set for the input states. The output
	 * set will contain all states that can be reached by making epsilon
	 * transitions from all NFA states in the input set. Returns an empty
	 * set if the input set or the closure set is empty, modifies *accept
	 * to point at the accepting string if one of the elements of the
	 * output state is an accepting state.
	 */

	size_t stack[NFA_MAX]; /* Stack of untested states */

	size_t *tos=0;	/* Stack pointer */

	NFA*p=0;	/*NFA state being examined */

	size_t i=0;	/*State number of "	*/
	
	size_t accept_num = LARGEST;

	if(!input){return abort;}

	*accept=0;

	tos=&stack[-1];
	
	i=next_member(0);
	
	while((i=next_member(input))>=0){*++tos=i;}

	while(INBOUNDS(stack,tos))
	{
		i=*tos--;
		
		p=&Nfa[i];

		if(p->accept&&(i<accept_num))
		{
			accept_num=i;
			*accept=p->accept;
			*anchor=p->anchor;
		}

		if(p->edge==EPSILON)
		{
			if(p->next)
			{
				i=p->next-Nfa;

				if(!MEMBER(input,i))
				{
					ADD(input,i);
					*++tos=i;
				}
			}

			if(p->next2)
			{
				i=p->next2-Nfa;

				if(!MEMBER(input,i))
				{
					ADD(input,i);
					*++tos=i;
				}
			}
		}

	}

	return input;
}

/*Return a set that contains all NFA states that can be reached by making
 *transitions on "c" from any NFA state in "inp_set". Returns NULL if
 *there are no such transactions. The inp_set is not modified.
 */

SET*move(SET*inp_set,size_t c)
{
	size_t i = 0;

	NFA*p=0;

	SET*outset=0;

	for(i=Nfa_states;--i>=0;)
	{
		if(MEMBER(inp_set,i))
		{
			p=&Nfa[i];

			if(p->edge==c||(p->edge==CCL&&TEST(p->bitset,c)))
			{
				if(!outset){outset=newset();}

				ADD(outset,p->next-Nfa);
			}
					
		}
	}

	return outset;
	
}

#define BSIZE 256

static char Buf[BSIZE]; /* input buffer*/

static char *Pbuf=Buf; /* current position in input buffer */

static char *Expr; /*regular expression from command line*/

int nextchar(void)
{
	if(!*Pbuf)
	{
		if(!fgets(Buf,BSIZE,stdin))
		{
			return 0;
		}

		Pbuf=Buf;
	}

	return *Pbuf++;
}

/*-----------------------------------------------------------*/

static void printbuf(void)
{
	static size_t first_time_called=1;

	if(!first_time_called){return 0;}
	
	first_time_called=0;

	return Expr;
}

/*----------------------------------------------------------*/

int main(int argc,char**argv)
{
	int sstate=0; /* Starting NFA state */

	SET*start_dfastate=0; /*Set of starting nfa states*/

	SET*current=0;/*current DFA state*/

	SET*next=0;

	size_t accept=0;

	size_t c=0;

	size_t anchor=0;

	if(argc==2){fprintf(stderr,"expression is %s\n",argv[1]);}

	else
	{

	}
}
