/* NFA.C--Make an NFA from a LeX input file using Thompson's Construction */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "nfa.h" /* defines for NFA, EPSILON, CCL */
#include "globals.h" /* externs for Verbose, etc. */
#include "set.h"
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
	
	while (++S_input <= Input ){putc('_',stderr);}

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
 * macro SUPPORT
 */
#define MAC_NAME_MAX 34 /* Maximum name length */
#define MAC_TEXT_MAX 80 /* Maximum amount of expansion text */

typedef struct
{
	char name[MAC_NAME_MAX];
	char text[MAC_TEXT_MAX];
}macro;

static HASH_TAB *macros; /* Symbol table for macro definitions */

/*----------------------------------------------------*/

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

	hash_tab * p;

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

void *newsym(size_t size)
{
	/* Allocate space for a new symbol; return a pointer to the user space. */

	bucket * sym = 0;	

	if ( (sym = (bucket*)calloc(size+sizeof(bucket),1)) )
	{
		fprintf(stderr,"Can't get memory for BUCKET\n");

		raise(SIGABRT);
		return 0;
	}

	return (void *)(sym+1);
}

/*-------------------------------------------------------------------------*/

void freesym(void*sym)
{
	free((bucket*)sym-1);
}

void new_macro(char*def)
{
	unsigned hash_add();

	char*name;
	char*text;
	char*edef;
	macro*p;
	static int first_time=1;

	if (first_time)
	{
		first_time=0;
		macros=maketab(31,hash_add,strcmp);
	}

	for (name = def; *def && !isspace(*def);def++)
		;
	if(*def){*def++=0;}

	/*Isolate the definition text. This process is complicated because
	 * you need to discard any trailing whitespace on the line. The first
	 * while loop skips the preceding whitespace. The for loop is looking
	 * for end of string. If you find the white character (and the \n at
	 * the end of string is white), remember the position as a potential
	 * end of string.
	 */

	while (isspace(*def)){++def;} /* skip up to macro body */
	
	text = def;

	edef = 0;

	while(*def){if (!isspace(*def)){++def;}else{for(edef=def++;isspace(*def);++def);}}
	if (edef){*edef=0;} /*Add the macro to the symbol table*/
	
	p = (macro*)newsym(sizeof(macro));

	strncpy(p->name,name,MAC_NAME_MAX);
	strncpy(p->text,MAC_TEXT_MAX);
	addsym(macros,p);
}

/*---------------------------------------------------------------*/

static char *expand_macro(char**namep)
{
	/*Return a pointer to the contents of a macro having the
	 * indicated name. Abort with a message if no macro
	 * exists. The macro name includes the brackets, which
	 * are destroyed by the expansion process. *namep is
	 * modified to point past the close brace.
	 */

	char * p = 0; macro*mac = 0;

	if (!(p=strchr(++(*namep),'}'))){parse_err(E_BADMAC);}
	
	else
	{
		*p++=0;

		if(!(mac=(macro*)findsym(macros,*namep))){parse_err(E_NOMAC);}

		*namep=p; // Update name pointer

		return mac->text;
	}

	return "ERROR";
}

//Workhouse function needed by  ptab() call in

static print_a_macro(macro*mac) 
{
	printf("%-16s--[%s]--\n",mac->name,mac->text);	
}

void printmacs()
{
	if(!macros){printf("\tThere are no macros\n");}
	else{printf("\nMACROS:\n");ptab(macros,print_a_macro,0,1);}
}

typedef enum token
{	EOS=1, /* end of string*/
	ANY,	/*	.	*/
	AT_BOL,	/*	^	*/
	AT_EOL, /*	$	*/
	CCL_END,/*	]	*/
	CCL_START,/*	[	*/
	CLOSE_CURLY,/*	}	*/
	CLOSE_PAREN,/*	)	*/
	CLOSURE,/*	*	*/
	DASH,	/*	-	*/
	END_OF_INPUT/*	EOF	*/
	L,	/*	literal character	*/
	OPEN_CURLY,/*	{	*/
	OPEN_PAREN,/*	(	*/
	OPTIONAL,/*	?	*/
	OR,	/*	|	*/
	PLUS_CLOSE/*	+	*/
	
}TOKEN;

static TOKEN Tokmap[]=
{
	/*	^@ ^A ^B ^C ^D ^E ^F ^G ^H ^I ^J ^K ^L ^M ^N 	*/
		L,  L, L, L, L, L, L, L, L, L, L, L, L, L, L,
	
	/*	^O ^P ^Q ^R ^S ^T ^U ^V ^W ^X ^Y ^Z ^[ ^\ ^]	*/
		L,  L, L, L, L, L, L, L, L, L, L, L, L, L, L,
	
	/*	^^ ^_ SPACE ! \" #  $ 	    %  &  '			*/
		L, L, L,    L,L  L, AT_EOL, L, L, L, 
	
	/*	(  	    ) 		* 	 + 	     ,  -     .   */
		OPEN_PAREN, CLOSE_PAREN,CLOSURE, PLUS_CLOSE, L, DASH, ANY,

	/*	/  0  1  2  3  4  5  6  7  8  9  :  ;  <  = */
		L, L, L, L, L, L, L, L, L, L, L, L, L, L, L,
	
	/* > 		?					    */
	   L,		OPTIONAL,

	/* @  A  B  C  D  E  F  G  H  I  J  K  L  M  N */
	   L, L, L, L, L, L, L, L, L, L, L, L, L, L, L,
	
	/* O  P  Q  R  S  T  U  V  W  X  Y  Z 		*/
	   L, L, L, L, L, L, L, L, L, L, L, L,
	
	/* [	     \ 	 ] 	    ^			*/
	   CCL_START L,  CCL_END,   AT_BOL,
	
	/* _   '  a  b  c  d  e  g  h  i  j  k  l  m	*/
	   L,  L, L, L, L, L, L, L, L, L, L, L, L, L,
	
	/* n   o  p  q  r  s  t  u  v  w  x  y  z 	*/
	   L,  L, L, L, L, L, L, L, L, L, L, L, L
	
	/*  {	 	|	}		DEL
	    OPEN_CURLY	OR,	CLOSE_CURLY,    L
	};

	static uint8_t*(*Ifunct)(); /* Input function pointer */
	static uint8_t*Input="";    /* Current position in input string */
	static uint8_t*S_input;	    /* Beginning of input string */
	static TOKEN Current_tok;   /* Current token */
	static size_t Lexeme;	    /* Value associated with LITERAL */

	#define MATCH(t) (Current_tok==(t))
/*-----------------------------------------------------------------
 * Lexical analyzer:
 * 
 * Lexical analysis is trivial because all lexemes are single-character values.
 * The only complications are escape sequences and quoted strings, both of
 * which are handled by advance(), below. This routine advances past the 
 * current tokenk putting the new token into Current_tok and the equivalent
 * lexeme into Lexeme. If the character was escaped, Lexeme holds the actual
 * value. For example, if a "\s" is encountered, Lexeme will hold a space
 * character. The MATCH(x) macro returns true

static size_t advance()
{
	static size_t inquote = 0;

	size_t saw_esc;

	static uint8_t *stack[SSIZE];	/* Input-source stack */

		       **sp = 0;	/* and stack pointer */
					
		       			/* Initialize sp.  */
	if (!sp)			
	{
		sp=stack-1; 		/* Necessary for large model */
	}

	if (Current_tok==EOS)		/* Get another line */	
	{
		if(inquote){parse_err(E_NEWLINE);}

		do
		{
			if(!(Input=(*Ifunct)()))
			{
				Current_tok=END_OF_INPUT;
				goto exit;
			}
			
			while(isspace(*Input)){Input++;} /* Ignore leading */
							
							/*Remember start of line */	
		}while(!*Input);

		S_input = Input;		/*for eorror messages */	

	}

	while(*Input==0x0)
	{
		if(INBOUNDS(stack,sp) )
		{
			Input=*sp--;
			continue;
		}

		Current_tok = EOS;

		Lexeme = 0;

		goto exit;
	}

	if(!inquote)
	{
		while(*Input=='{')
		{
			*++sp=Input;
			Input=expand_macro(sp);

			if(TOOHIGH(stack,sp))
			{
				parse_err(E_MACDEPTH); /* Stack overflow */
			}
		}
	}

	if (*Input=='\"')
	{
		inquote = ~inquote;

		if(!*++Input)
		{
			Current_tok=EOS;
			Lexeme=0;
			goto exit;
		}
	}

	saw_esc=(*Input=='\\');

	if (!inquote)
	{
		if(isspace(*Input))
		{
			Current_tok=EOS;

			Lexeme='\0';

			goto exit;
		}

		Lexeme=esc(&Input);
	}

	else
	{
		if(saw_esc&&Input[1]=='"')
		{
			Input+=2;
			Lexeme='"';
		}	

		else
		{
			Lexeme=*Input++;
		}
	}

	Current_tok=(inquote||saw_esc)?L:Tokmap[Lexeme];

	exit:
		return Current_tok;

}

static size_t advance(void);

static void cat_expr(NFA**,NFA**);

static void discard(NFA*);

static void dodash(SET*);

static void expr(NFA**,NFA**);

static void factor(NFA**,NFA**);

static size_t first_in_cat(TOKEN);

static NFA *machine(void);

static NFA *new(void);

static void parse_err(ERR_NUM);

static NFA *rule(void);

static char*save(char*);

static void term(NFA**,NFA**);

/*------------------------------------------
 * The Parser:
 * A simple recursive descent parser that
 * creates a Thompson NFA for a regular
 * expression. The access routine [thompson()]
 * is at the bottom. The NFA is created a
 * directed graph, with each node containing
 * pointer's to the next node. Since the
 * structures are allocated from an array,
 * the machine can also be considered as an
 * array where the state number is the array
 * index.
 */

static NFA *machine(void)
{
	NFA*start;
	NFA*p;

	ENTER("machine");

	p=start=new();

	p->next=rule();

	while(!MATCH(END_OF_INPUT))
	{
		p->next2=new();
		p=p->next2;
		p->next=rule();

	}

	LEAVE("machine");
	return start;
}

/*-------------------------------------------*/
static NFA*rule(void)
{
	/* rule --> expr EOS action
	 * 	 	^expr EOS action
	 * 	 	expr$ EOS action
	 * action --> <tabs> <string of characters>
	 * 		epsilon
	 */

	NFA*p;

	NFA*start=0;
	NFA*end=0;
	size_t anchor=NONE;

	ENTER("rule");	

	if(MATCH(AT_BOL))
	{
		start=new();
		start->edge='\n';
		anchor|=START;
		advance();
		expr(&start->next,&end);
	}
	
	else{expr(&start,&end);

	if(MATCH(AT_EOL))
	{
		/*pattern followed by a carriage-return or linefeed (use a
		 *character class).
		 */
		
		advance();
		end->next=new();
		end->edge=CCL;

		if(!(end->bitset=newset()))
		{	parse_err(E_MEM);	}

		ADD(end->bitset,0xa);

		if(!Unix){ADD(end->bitset,'\r');}

		end=end->next;

		anchor|=END;
	}

	while(isspace(*Input)){Input++;}

	end->accept=save(Input);
	end->anchor=anchor;
	advance(); /* skip past EOS */
	LEAVE("rule");
	return start;
}

static void expr(NFA**startp,NFA**endp)
{
	/*Because a recursive descent compiler cannot handle left
	 *recursion, the productions:
 	 *
	 * expr	-> 	expr OR cat_expr
	 * 		| cat_expr
	 * must be translated into:
	 *
	 * expr	->	cat_expr expr'
	 * expr'->	OR cat_expr expr'
	 * 		epsilon
	 *
	 * which can be implemented with this loop:
	 *
	 * cat_expr
	 *
	 * while(match(OR))
	 * 	cat_expr
	 * 	do this OR
	 */

	NFA*e2_start=0; /*expression to right of | */
	NFA*e2_end=0;
	NFA*p=0;

	ENTER("expr");

	cat_expr(startp,endp);

	while(MATCH(OR))
	{
		advance();
		cat_expr(&e2_start,&e2_end);
		
		p=new();
		p->next2=e2_start;
		p->next=*startp;
		*startp=p;
		
		p=new();
		(*endp)->next=p;
		e2_end->next=p;
		*endp=p;	
	}

	LEAVE("expr");
}

/*--------------------------------------------------------*/

static void cat_expr(NFA**startp,NFA**endp)
{
	/* The same translations that were needed in the expr rules are needed
	 * again here:
	 *
	 * cat_expr->cat_expr|factor
	 * 			factor
	 * is translated to:
	 *
	 * cat_expr -> factor cat_expr'
	 * cat_expr'->|factor cat_expr'
	 * 		epsilon
	 */

	NFA*e2_start,*e2_end;

	ENTER("cat_expr");

	if(first_in_cat(Current_tok)){factor(startp,endp);}

	while(first_in_cat(Current_tok))
	{
		factor(&e2_start,&e2_end);
		
		memcpy(*endp,e2_start,sizeof(NFA));

		discard(e2_start);	

		*endp=e2_end;
	}

	LEAVE("cat_expr");
}

/*--------------------------------------------------------------*/

static size_t first_in_cat(TOKEN tok)
{
	switch(tok)
	{
		case CLOSE_PAREN:
		case AT_EOL:
		case OR:
		case EOS:
		{
			return 0;
		}
		case CLOSURE:
		case PLUS_CLOSE:
		case OPTIONAL:
		{	parse_err(E_CLOSE); return 0; }
		
		case CCL_END:{parse_err(E_BRACKET);return 0;}
		case AT_BOL:{parse_err(E_BOL);return 0;}
	}

	return 1;
}

static void factor(NFA**startp,NFA**endp)
{
	/*factor	-->	term* | term+ | term?
	 */

	NFA*start=0,*end=0;

	ENTER("factor");

	term(startp,endp);

	if(MATCH(CLOSURE)||MATCH(PLUS_CLOSE)||MATCH(OPTIONAL))
	{
		start=new();
		end=new();
		start->next=*startp;
		(*endp)->next=end;
		
		if(MATCH(CLOSURE)||MATCH(OPTIONAL))
		{
			start->next2=end;
		}

		if(MATCH(CLOSURE)||MATCH(PLUS_CLOSE))
		{
			(*endp)->next2=*startp;
		}

		*startp=start;
		*endp=end;
		advance();
	}

	LEAVE("factor");
}

static void term(NFA**startp,NFA**endp)
{
	/*Process the term productions:
	 *
	 * term --> [...] | [^...]| [] | [^] | . | (expr) | <character>
	 *
	 * The [] is nonstandard. It matches a space, tab, formfeed, or newline,
	 * but not a carriage return (\r). All of these are single nodes in the
	 * NFA.
	 */

	NFA*start=0;size_t c=0;

	ENTER("term");

	if(MATCH(OPEN_PAREN))
	{
		advance();
		expr(startp,endp);
		if(MATCH(CLOSE_PAREN)){advance();}
		else{parse_err(E_PAREN);}
	}

	else
	{
		*startp=start;
		*endp=start->next=new();

		if(!(MATCH(ANY)||MATCH(CCL_START) ) )
		{
			start->edge=Lexeme;
			
			advance();
		}

		else
		{
			start->edge=CCL;

			if(!(start->bitset=newset())){parse_err(E_MEM);}

			if(MATCH(ANY) )
			{
				ADD(start->bitset,0xa);
				if(!Unix){ADD(start->bitset,'\r');}
				
				COMPLEMENT(start->bitset);
			}

			else
			{
				advance();

				if(MATCH(AT_BOL))
				{
					advance();

					ADD(start->bitset,0xa); /* Don't include \n in class */
					if(!Unix){ADD(start->bitset,'\r');}

					COMPLEMENT(start->bitset);
				}

				if(!MATCH(CCL_END)){dodash(start->bitset);}

				else
				{
					for(c=0;c<=' ';c++){ADD(start->bitset,c);}
				}
			}

			advance();
		}

	}
		LEAVE("term");
}

/*-------------------------------------------------*/

static void dodash(SET*set) /*Pointer to ccl character set */
{
	register int first;

	for(;!MATCH(EOS)&&!MATCH(CCL_END);advance())
	{
		if(!MATCH(DASH))
		{
			first=Lexeme;

			ADD(set,Lexeme);
		}

		else
		{
			advance();

			for(;first<=Lexeme;first++)
			{
				ADD(set,first);
			}
		}
	}
}
