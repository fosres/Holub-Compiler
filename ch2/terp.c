#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "nfa.h"
#include "set.h"
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

SET*move(SET*inp_set,size_t c)
