/* NFA.C--Make an NFA from a LeX input file using Thompson's Construction */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "nfa.h" /* defines for NFA, EPSILON, CCL */
#include "globals.h" /* externs for Verbose, etc. */

#ifdef DEBUG
	int Lev = 0;
#define ENTER(f) (printf("%*senter %s [%c] [%1.10s] \n",Lev++ *4,"",f,Lexeme,Input))
#define LEAVE(f) (printf("%*sleave %s [%c] [%1.10s]\n",--Lev*4,"",f,Lexeme,Input))
#else
#	define ENTER(f)
#	define LEAVE(f)
#endif

/*---------------------------
 *	Error processing stuff. Note that all errors are fatal.
 *---------------------------
*/

typedef enum _err_num_
{
	E_MEM,/*Out of memory*/
	E_BADEXPR,/*Malformed regular expressions*/
	E_PAREN,/*Missing close parenthesis*/
	E_STACK,/*Internal error: Discard stack full*/
	E_LENGTH,/*Too many regular expressions*/
	E_BRACKET,/*Missing [ in character class ] */
	E_BOL,/*^must be at start of expr or ccl */
	E_CLOSE,/*+ ? or * must follow expression */
	E_STRINGS,/*Too many characters in accept actions */
	E_NEWLINE,/*Newline in quoted string */
	E_BADMAC,/*Missing } in macro expansion */
	E_NOMAC,/*Macro doesn't exist */
	E_MACDEPTH /*Macro expansions nested too deeply. */
}ERR_NUM;		

static char *Errmsgs[] = /* Indexed by ERR_N */
{
	"Not enough memory for NFA",
	"Malformed regular expression",
	"Missing close parenthesis",
	"Internal error: Discard stack full",
	"Too many regular expressions or expression too long",
	"Missing [ in character class",
	"^ must be at start of expression or after [",
	"+ ? or * must follow an expression or after [",
	"Too many characters in accept actions",
	"Newline in quoted string, use \\n to get newline into expression",
	"Missing } in macro expansion",
	"Macro doesn't exist",
	"Macro expansions nested too deeply",
};

static void parse_err(ERR_NUM type)
{
	fprintf(stderr,"ERROR (line %d) %s\n%s\n",Actual_lineno,
						Errmsgs[(int)type],S_input);
	
	while (++S_input <= Input ){putc('_',stderr);

	fprintf(stderr,"^\n");
	exit(1);
}

static NFA *Nfa_states; /*State-machine array */
static int Nstates=0;/* # of NFA states in machine */
static int Next_alloc;/* Index of next element of the array */

#define SSIZE 32

static NFA *Sstack[SSIZE]; /* Stack used by new() */
static NFA **Sp=&Sstack[-1]; /* Stack pointer */

#define NUMELE(a)	(sizeof(a)/sizeof(*(a)))
#define LASTELE(a)	((a) + (NUMELE(a)-1))
#define TOOHIGH(a,p)	((p) - (a) > (NUMELE(a) - 1))
#define TOOLOW(a,p)	((p) - (a) <  0 )
#define INBOUNDS(a,p)	( ! (TOOHIGH(a,p) || TOOLOW(a,p)) )

#define STACK_OK() (INBOUNDS(Sstack,Sp) )
#define STACK_USED() ( (Sp-Stack)+1)/*slots used */
#define PUSH(x)	(*++Sp=(x)	) /* put X on stack */
#define POP()	( *Sp--		) /* get X from stack */

/*-----------------------------------------------------*/

static int *Strings; /*Place to save accepting strings */
static int *Savep;   /*Current position in Strings array. */

/*-----------------------------------------------------*/

static NFA *new()	/*NFA management functions */
{
	NFA *p = 0;
	static int first_time = 1;

	if (first_time)
	{
		if ( !(Nfa_states = (NFA *)calloc(NFA_MAX,sizeof(NFA)) ) )
		{
			parse_err(E_MEM);
		}

		first_time = 0;

		Sp = &Sstack[-1];
	}

	if ( ++Nstates >= NFA_MAX )
	{
		parse_err(E_LENGTH);
	}

	/* If the stack is not ok, it's empty. */

	p = !STACK_OK() ? &Nfa_states[Next_alloc++] : POP();

	p->edge = EPSILON;
	
	return p;
}

/*--------------------------------------------------------*/

static void discard(NFA * nfa_to_discard)
{
	--Nstates;

	memset(nfa_to_discard,0,sizeof(NFA));
	nfa_to_discard->edge = EMPTY;
	PUSH(nfa_to_discard);

	if (!STACK_OK() )
	{
		parse_err(E_STACK);
	}

}

/*-------------------------------------------------------*/

static char *save(char *str)
{
	char *textp,*startp;

	int len = 0;

	static int first_time = 1;

	if ( first_time )
	{
		if ( !(Savep = Strings = (int *)malloc(STR_MAX)) )
		{
			parse_err(E_MEM);
		}

		first_time = 0;
	}

	if (*str == '|' )
	{
		return (char *)(Savep+1);
	}

	*Savep++ = Lineno;

	for ( textp = (char *)Savep; *str; *textp++ = *str++ )
	{
		if ( textp >= (char*)(Strings+(STR_MAX-1)) )
		{
			parse_err(E_STRINGS);
		}
	}

	*textp++=0;

	/*Increment Savep past the text. "len" is initialized to the
	 *string length. The "len/sizeof(int)" truncates the size
	 *down to an even multiple of the current in size. The "+(
	 *len % sizeof(int) != 0 )" adds 1 to the truncated size if
	 *the string length is not an even multiple of the int size
	 *(the != operator evaluates to 1 or 0). Return a pointer
	 *to the string itself.
	 */

	startp = (char *)Savep;

	len = textp - startp;

	Savep += (len/sizeof(int)) + (len % sizeof(int) != 0 );

	return startp;
}

/*-----------------------------------------------------
 * MACRO SUPPORT
 */
#define MAC_NAME_MAX 34 /* Maximum name length */
#define MAC_TEXT_MAX 80 /* Maximum amount of expansion text */

typedef struct
{
	char name[MAC_NAME_MAX];
	char text[MAC_TEXT_MAX];
}MACRO;

static HASH_TAB *Macros; /* Symbol table for macro definitions */

/*----------------------------------------------------*/

void new_macro(char*def)
{
	unsigned hash_add();

	char*name;
	char*text;
	char*edef;
	MACRO*p;
	static int first_time=1;

	if (first_time)
	{
		first_time=0;
		Macros=maketab(31,hash_add,strcmp);
	}
}

typedef struct bucket
{
	struct bucket *next;
	struct bucket **prev;

}bucket;

typedef struct hash_tab
{
	int size;/* Max number of elements in table */
	int numsyms; /* number of elements currently in table */
	unsigned (*hash)(); /*hash function */
	int (*cmp)(); /* comparision function, cmp(name,bucket_p) */
	bucket *table[1]; /*First element of actual hash table */

}hash_tab;

hash_tab *maketab(unsigned maxsym,unsigned (*hash_function), int (*cmp_function))
{
	/* Make a hash table of the indicated size. */

	HASH_TAB * p;

	if (!maxsym )
	{ maxsym = 127; }

	/* |<-------- space for table ------>|<- and header -->| */

	if ( p = (hash_tab*)calloc(1,(maxsym*sizeof(BUCKET*))+sizeof(HASH_TAB)))
	{
		p->size = maxsym;
		p->numsyms = 0;
		p->hash = hash_function;
		p->cmp = cmp_function;
	}

	else
	{
		fprintf(stderr,"Insufficient memory for symbol table\n");
		raise(SIGABRT);
		return NULL;
	}

	return p;
}
