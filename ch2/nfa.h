#define	START	1
/*----------------------------------
 * Nfa state;
 */

//typedef unsigned short     _SETTYPE ;		   /* one cell in bit map   */

typedef struct _set_
{
    unsigned short defmap[8];	/* The map itself		  */
    unsigned short *map   ;		/* Pointer to the map		  */
    unsigned	  nbits  ;		/* Number of bits in map	  */
    unsigned char nwords ;		/* Number of words in map	  */
    unsigned char compl	 ;		/* is a negative true set if true */

} SET;

typedef struct nfa
{
	SET *bitset; /*Set to store character classes. */
	struct nfa *next; /*Next state (or NULL if none) */
	struct nfa *next2; /*Another next state if edge==EPSILON */
	char * accept;/*NULL if not an accepting state, else */
	int anchor;/*Says whether pattern is anchored and, if */
		   /*so, where (uses #defines above)	*/
	int edge;	

} NFA;

#define EPSILON -1
#define CCL -2
#define EMPTY -3

#define NONE 0
#define START 1
#define END 2
#define BOTH (START|END)

#define NFA_MAX 768 /*Maximum number of NFA states in a */
#define STR_MAX (10*1024)

void new_macro(char*definition);
void printmacs(void);
NFA *thompson(char*(*input_funct)(),int*max_state,NFA**start_state);
void print_nfa(NFA*nfa,int len,NFA*start);

