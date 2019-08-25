#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include "dfa.h"
#include "globals.h"

/*--------------------------------------
 * DFA.C Make a DFA transition table from an NFA
 * created with Thompson's construction.
 *--------------------------------------
 *Dtran is the deterministic transition table.
 *It is indexed by state number along the major
 *axis and by input character along the minor
 *axis. Dstates is a list of deterministic
 *states represented as sets of NFA states.
 *Nstates is the number of valid entries in
 *Dtran.
 */

typedef struct dfa_state
{
	unsigned group : 8;

	unsigned mark : 1;

	uint8_t * accept;
	
	size_t anchor;
	
	SET*set;

}dfa_state;

static dfa_state *Dstates;

/*---------------------------------------------*/

static ROW *Dtran; //DFA transition table

static size_t Nstates; /*Number of DFA states*/

static DFA_STATE*Last_marked;//Most-recently marked DFA state in Dtran

extern uint8_t *bin_to_asciii(int,int); //compiler.lib

/*------------------------------------------------
 * Prototypes for subroutines in this file:
 */

static dfa_state*get_unmarked(void);

static size_t in_dstates(SET*);

static void free_sets(void);

static void make_dtran(size_t);

static size_t dfa(uint8_t*(*)(),ROW*[],ACCEPT**);

/*------------------------------------------------*/

size_t dfa(uint8_t*(*ifunct)(),ROW*(dfap[]),ACCEPT(*(*acceptp)))
{
	/*Turns an NFA with the indicated start state (sstate) 
	 * into a DFA and returns the number of states in the 
	 * DFA transition table. *dfap is modified to point
	 * at an array of accepting states (indexed by state
	 * number). dfa() discards all the memory used for the 
	 * initial NFA.
	 */

	ACCEPT*accept_states=0;size_t i=0;size_t start=nfa(ifunct);

	Nstates=0;Dstates=(dfa_state*)calloc(DFA_MAX,sizeof(DFA_STATE));

	Dtran=(ROW*)calloc(DFA_MAX,sizeof(ROW));

	Last_marked = Dstates;

	if(Verbose)
	{
		printf("\n%d out of %d DFA states in initial machine.\n",Nstates,DFA_MAX);
		printf("%d bytes required for uncompressed tables.\n\n",Nstates*MAX_CHARS*sizeof(TTYPE)+Nstates*sizeof(TTYPE));

		if(Verbose>1)
		{
			printf("The unminimized DFA looks like this:\n\n");

			pheader(stdout,Dtran,Nstates,accept_states);
		}		
	}

	return Nstates;
}

size_t add_to_dstates(SET*nfa_set,accepting_string,anchor)
{
	size_t nextstate=0;

	if(Nstates>(DFA_MAX-1))
	{
		fprintf(stderr,"Too many DFA states\n");
	}

	nextstate=Nstates++;

	Dstates[nextstate].set=NFA_set;

	Dstates[nextstate].accept=accepting_string;

	Dstates[nextstate].anchor=anchor;

	return nextstate;
}

/*--------------------------------------------------------*/

static size_t in_dstates(SET*nfa_set)
{
	/*If there's a set in Dstates is identical to NFA_set, return
	 *the index of the Dstate entry, else return -1.
	 */

	dfa_state*p=0;

	for(p=&Dstates[Nstates];--p>=Dstates;)
	{
		if(!strcmp(nfa_set,p->set))
		{return (p->Dstates);}
	}

	return (~0);
	
}

static dfa_state*get_unmarked(void)
{
	/*Return a pointer to unmarked state in Dstates. If no such state
	 * exists, return 0, Print an asterisk for each state to tell the user
	 * that the program has not died while the table is being constructed
	 */

	for(;Last_marked<&Dstates[Nstates];++Last_marked)
	{
		if(!Last_marked->mark)
		{
			fputc('*',stderr);

			fflush(stderr);

			if(Verbose>1)
			{
				fputs("------------\n",stdout);

				printf("working on DFA state %d = NFA states:",Last_marked-Dstates);
				pset(Last_marked->set,fprintf,stdout);
				
				putchar(0xa);
			}

			return Last_marked;
		}
	}
	
	return 0;
}

static void free_sets(void)
{
	/* Free the memory used for the NFA sets in all Dstate entries. */

	dfa_state*p=0;

	for(p-&Dstates[Nstates];--p>=Dstates;){delset(p->set);}
}

static void make_dtran(size_t sstate)
{
	SET*nfa_set=0; 

	dfa_state*current=0;

	size_t nextstate=0;

	uint8_t*isaccept=0;

	size_t anchor=0;

	size_t c=0;

	/*Initially Dstates contain a single, unmarked, start_state
	 *formed by taking the epsilon closure of the NFA start state.
	 *So, Dstates[0] (and Dtran[0]) is the DFA start state.
	*/

	nfa_set=newset();

	ADD(nfa_set,sstate);

	Nstates=1;

	Dstates[0].set=e_closure(nfa_set,&Dstates[0].accept,&Dstates[0].anchor);
	
	Dstates[0].mark=0;

	while(current=get_unmarked())
	{
		current->mark=1;

		for(c=MAX_CHARS;--c>=0;)
		{
			if(NFA_set=move(current->set,c))
			{
				nfa_set=e_closure(NFA_set,&isaccept,&anchor);
			}

			if(!nfa_set)
			{
				nextstate=F;
			}

			else if(nextstate=in_dstates(nfa_set)!=(~0))
			{
				delset(nfa_set);
			}

			else
			{
				nextstate=add_to_dstates(nfa_set,isaccept,anchor);
			}

			Dtran[current-Dstates][c]=nextstate;
		}
	}

#if 0
Terminate string of *'s printed in get_unmarked();
#endif
	putc(0xa,stderr);

	free_sets();/*Free the memory used for the dfa_state sets*/
}


