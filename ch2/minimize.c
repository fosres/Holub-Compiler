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
