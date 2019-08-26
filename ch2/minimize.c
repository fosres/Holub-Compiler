#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include "dfa.h"
#include "globals.h"

static SET*Groups[DFA_MAX]; /*Groups of equivalent states in Dtran */

static size_t Numgroups=0;//Number of groups in Groups

static size_t Ingroups[DFA_MAX]; //The Inverse Group

/*-----------------------------------------------------------------
 * Prototypes for subroutines in this file:
 */

static void fix_dtran(ROW*[],ACCEPT**);

static void init_groups(size_t,ACCEPT*);

static size_t min_dfa(uint8_t*(*)(),ROW*[],ACCEPT**);

static void minimize(size_t,ROW*[],ACCEPT**);

static void pgroups(size_t);

/*---------------------------------------------------------------*/

size_t min_dfa(uint8_t*(*funct)(),ROW*(dfap[]),ACCEPT*(*acceptp));

/* Make a minimal DFA< eliminating equivalent states. Return the
 * number of states in the minimized machines. *sstatep=the new start
 * state.
 */

size_t nstates=0;/*Number of states*/

memset(Groups,0,sizeof(Groups));

memset(Ingroup,0,sizeof(Ingroup));

Numgroups=0;

nstates=dfa(ifunct,dfap,acceptp);

minimize(nstates,dfap,acceptp);

return Numgroups;

}

static void init_groups(size_t nstates,ACCEPT*accept)
{
	SET**last=0;
	size_t i=0,j=0;

	last=&Groups[0];

	Numgroups=0;

	for(i=0;i<nstates;i++)
	{
/*Check to see if a group already exists that has the same
 * accepting string as the current state. If so, add the
 * current state to the already existing group. Note that
 * silence all nonaccepting states have NULL accept strings,
 * this loop puts all of these together into a single group.
 * Also note that the test in the for loop always fails for
 * group 0, which cannot be an accepting states.
 */	
		for(j=i;--j>=0;)
		{
			ADD(Groups[Ingroup[j]],i);
			Ingroup[i]=Ingroup[j];
			
			if(accept[i].string==accept[j].string)
			{
				ADD(Groups[Ingroup[j]],i);

				Ingroup[i]=Ingroup[j];

				goto match;

			}
		}

		/*Create a new group and put the current state into it.
		 *Note that ADD() has side effects, so "last" cannot
		 *be incremented in the ADD invocation.
		 */

		*last=newset();

		ADD(*last,i);

		Ingroup[i]=Numgroups++;
		
		last++;

		match: 
	}

	if(Verbose>1)
	{
		printf("Initial groupings:\n");

		pgroups(nstates);
	}
}

static void minimize(size_t nstates,ROW*(dfap[]),accept*accepting)
{
	size_t old_numgroups=0;

	size_t c=0;

	SET**current=0;

	SET**new=0;

	size_t first=0;

	size_t next=0;

	size_t goto_first=0;

	size_t goto_next=0;

	ROW*dtran=*dfap;

	ACCEPT*accept=*acceptp;

	init_groups(nstates,accept);

	do
	{
		old_numgroups=Numgroups;

		for(current=&Groups[0];current<&Groups[Numgroups];++current)
		{
			if (num_ele(*current)<=1)
			{
				continue;	
			}

			new = &Groups[Numgroups];

			*new=newset();

			next_member(0);

			while(next=next_member(*current) )	
			{
				for(c=MAX_CHARS;--c>=0)
				{
					goto_first=dtran[first][c];

					goto_next=dtran[next][c];

					if(	goto_first!=goto_next
						&&
						(goto_first==F||goto_next==F
						 ||
						 Ingroup[goto_first]!=Ingroup[goto_next]
						)
					  )

					{
						REMOVE(*current,next);

						ADD(*new,next);

						Ingroup[next]=Numgroups;

						break;		
					}

				}	
			}
		}

		if(IS_EMPTY(*new))
		{
			delset(*new);
		}

		else
		{
			++Numgroups;
		}

	}while(old_numgroups!=Numgroups);	

	if(Verbose>1)
	{
		printf("\nStates grouped as follows after minimization:\n");

		pgroups(nstates);
	}

	fix_dtran(dfap,acceptp);
}

/*----------------------------------------------------------*/

static void fix_dtran(ROW*(dfap[]),ACCEPT*(*acceptp))
{
	/* Reduce the size of the dtran, using the group set made by minimize().
	 * Return the state number of the start state. The original dtran and
	 * accept arrays are destroyed and replaced with the smaller versions.
	 * Consider the first element of each group (current) to be a
	 * "representative" state. Copy the state to the new transition table,
	 * modifying all the transitions in the representatives state so that
	 * they will go to the group within which the old state is found.
	 */

	SET**current=0;

	ROW*newdtran=0;

	ACCEPT*newaccept=0;

	size_t state=0;

	size_t i=0;

	size_t *src=0,*dest=0;

	ROW*dtran=*dfap;

	ACCEPT*accept=*acceptp;

	newdtran(ROW*)calloc(Numgroups,sizeof(ROW));

	newaccept(ACCEPT*)calloc(Numgroups,sizeof(ACCEPT));

	if(!newdtran||!newaccept)
	{
		error_msg("Stack overflow Error!\n",lineno,charno);	
	}

	next_member(0);

	for(current=&Groups[Numgroups];--current>=Groups;)
	{
		dest=&newdtran[current-Groups][0];

		state=next_member(*current);

		src=&dtran[state][0];

		newaccept[current-Groups]=accept[state];

		for(i=MAX_CHARS;--i>=0;src++,dest++)
		{
			*dest=(*src==F)?F:Ingroup[*src];
		}
	}

	free(*dfap);

	free(*acceptp);

	*dfap=newdtran;

	*acceptp=newaccept;
}

static void pgroups(size_t nstates)
{
	SET**current=0;

	for(current=&Groups[Numgroups];--current>=Groups;)
	{
		printf("\tgroup %d: {",current-Groups);

		pset(*current,fprintf,stdout);

		printf("}\n");
	}

	while(--nstates>=0)
	{
		printf("\tstate %2d is in group %2d\n",nstates,Ingroup[nstates]);
	}
}
