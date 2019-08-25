/*------------------------------------------
 * DFA.H: The following definitions are used in
 * dfa.c and in minimize.c to represent DFA's.
 */

#define DFA_MAX 254 /* Maximum number of DFA states. If this
		     * number >= 255, you will have to change
		     * the output routines and driver. States
		     * are numbered from 0 to DFA_MAX-1
		     */
#if 0
This is the type of the output DFA	

transition table (the internal one is  	*/
#endif

typedef unsigned char TTYPE; 					

#define F -1; /* Marks failure states in the table */
#define MAX_CHARS 128 /*Maximum width of dfa transition table */

#if 0
One full row of Dtran, which is itself

an array, DFA_MAX elements long, of ROWs.
#endif

typedef size_t ROW[MAX_CHARS]; 

/*-----------------------------------------------*/

typedef struct ACCEPT
{
    uint8_t*string;	/* Accepting string; NULL if nonaccepting.            */
    int	  anchor;	/* Anchor point, if any. Values are defined in NFA.H. */

} ACCEPT;


typedef struct accept
{
	uint8_t*string;
	size_t anchor;
}ACCEPT;

/*-----------------------------------------------
* External subroutines:
*/

SET*e_closure(SET*,uint8_t**,size_t*);/*terp.c*/

void free_nfa(void);/*terp.c*/

SET*move(SET*,int); /*terp.c */

size_t nfa(uint8_t*(*)());	/*terp.c*/

size_t dfa(uint8_t*(*)(),ROW*[],ACCEPT**);/*dfa.c*/

size_t min_dfa(uint8_t*(*)(),ROW*[],ACCEPT**); /* minimize.c*/

size_t ttypesize(int); // printdfa.c

void pdefines(FILE*,int); //pdefines.c

size_t columns(FILE*,ROW*,int,int,uint8_t*);//columns.c

void cnext(FILE*,uint8_t*);//columns.c

void pheader(FILE*fp,ROW dtranp[],size_t nrows,ACCEPT*accept); //print.c

void pdriver(FILE*output,size_t nrows, ACCEPT*accept); //print.c



