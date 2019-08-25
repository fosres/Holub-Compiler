#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "dfa.h"
#include "globals.h"

static SET*Groups[dfa_max]; /* Groups of equivalent states in Dtran*/
static size_t Numgroups; // Number of groups in Groups
static size_t Ingroup[dfa_max]; // the inverse of group

/*------------------------------------------
 * Prototypes for subroutines in this file:
 */

static void pgroups(int);
	
