#include <stdio.h>
#include <stdint.h>
#define FCON 1
#define ICON 2

#ifdef __NEVER__
/*-------------------------------------
 * DFA ( start state is 0 ) is:
 *
 * State 0 [nonaccepting]
 * 	goto 3 on .
 * 	goto 1 on 0123456789
 * State 1 [accepting, line 7 <return ICON;>]
 * 	goto 2 on .
 * 	goto 1 on 0123456789
 * 	goto 5 on e
 * State 2 [accepting, line 8 <return\tFCON;>]
 * 	goto 2 on 0123456789
 * 	goto 5 on e
 * State 3 [nonaccepting]
 * 	goto 2 on 0123456789
 * State 4 [accepting, line 8 <return\tFCON;]
 * 	goto 4 on 0123456789
 * State 5 [nonaccepting]
 * 	goto 4 on 0123456789#define FCON 
 * 1
 * #define ICON 2
 *
 * #ifdef __NEVER__
 */*----------------------------------* DFA ( start state is 0 ) is:

State 0 [nonaccepting]
	goto 3 on .
	goto 1 on 0123456789
State 1 [accepting, line 7 <return ICON;>]
	goto 2 on .
	goto 1 on 0123456789
	goto 5 on e
State 2 [accepting, line 8 <return\tFCON;>]
	goto 2 on 0123456789
	goto 5 on e
State 3 [nonaccepting]
	goto 2 on 0123456789
State 4 [accepting, line 8 <return\tFCON;]
	goto 4 on 0123456789
State 5 [nonaccepting]
	goto 4 on 0123456789#define FCON 
1
#define ICON 2

#ifdef __NEVER__
/*-------------------------------------
 * DFA ( start state is 0 ) is:
 *
 * State 0 [nonaccepting]
 * 	goto 3 on .
 * 	goto 1 on 0123456789
 * State 1 [accepting, line 7 <return ICON;>]
 * 	goto 2 on .
 * 	goto 1 on 0123456789
 * 	goto 5 on e
 * State 2 [accepting, line 8 <return\tFCON;>]
 * 	goto 2 on 0123456789
 * 	goto 5 on e
 * State 3 [nonaccepting]
 * 	goto 2 on 0123456789
 * State 4 [accepting, line 8 <return\tFCON;]
 * 	goto 4 on 0123456789
 * State 5 [nonaccepting]
 * 	goto 4 on 0123456789
 */

#endif

static YY_TYPE Yy_nxt[6][128] =
/* 00 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1, 3,-1, 1, 1,
	   1, 1, 1, 1, 1, 1, 1, 1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1
	}

/* 01 */ { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1, 2,-1, 1, 1,	
	    1, 1, 1, 1, 1, 1, 1, 1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1, 5,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	
	   -1,-1,-1,-1,-1,-1,-1,-1
	},
/* 02 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
          -1,-1,-1,-1,-1,-1,-1,-1, 2, 2,
           2, 2, 2, 2, 2, 2, 2, 2,-1,-1,	  
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1, 5,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1
	},
/* 03 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1, 2, 2,
	   2, 2, 2, 2, 2, 2, 2, 2, -1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	},

/* 04 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	   4, 4, 4, 4, 4, 4, 4, 4,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1
	},

/* 05 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1, 4, 4,
	   4, 4, 4, 4, 4, 4, 4, 4,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  -1,-1,-1,-1,-1,-1,-1,-1
	}
};

/*--------------------------------------
 * yy_next(state,c) is given the current
 * state and input character and 
 * evaluates to the next state.
 */

#define yy_next(state,c) Yy_next[state][c]

#if 0
 * The Yy_cmap[] and Yy_rmap arrays are
 * used as follows:
 *
 * next_state= Yy_nxt[Yy_rmap[current_state] ][ Yy_cmap[input_char] ];
 *
 * Character positions in the Yy_cmap array are:
 */

Character positions in the Yy_cmap array are:

^@ ^A ^B ^C ^D ^E ^F ^G ^H ^I ^J ^K ^L ^M ^N ^O
^P ^Q ^R ^S ^T ^U ^V ^W ^X ^Y ^Z ^[ ^\ ^] ^^ ^_
    !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /
 0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
 @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
 P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
 '  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o
 p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  DEL

#endif

static unsigned char Yy_cmap[128] =
{0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 1, 0,
 2, 2, 2, 2,2, 2, 2, 2,2, 2, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 3, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0,
};

static unsigned char Yy_nxt[5][4]
=
{
/*00*/ {-1, 3, 1, -1 },
/*01*/ {-1, 2, 1, 5 },
/*02*/ {-1, -1, 2, 5},
/*03*/ {-1, -1, 2,-1},
/*04*/ {-1, -1, 4, -1}
};

#if 0
yy_next(state,c) is given the current state number and
evaluates to the next state.
#endif

#define yy_next(state,c) (Yy_nxt[Yy_rmap[state]][Yy_cmap[c]])

static unsigned char Yy_nxt0[] = 
{
11,'.',3,'0',1,'1',1,'3',1,'4',1,'5',1,'6',1,'7',1,'8',1,'9',1
};

static unsigned char Yy_nxt1[] =
{
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1, 2,-1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1, 5,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1
};

static unsigned char Yy_nxt2[]=
{11,'0',2,'1',2,'2',2,'3',2,'4',2,
'5',2,'6',2,'7',2,'8',2,'9',2,'e',
5};

static unsigned char Yy_nxt3[]=
{10,'0',2,'1',2,'2',2,'3',2,'4',2,
'5',2,'6',2,'7',2,'8',2,'9',2
};

static unsigned char Yy_nxt4[]=
{
10,'0',4,'1',4,'2',4,'3',4,'4',4,
'5',4,'6',4,'7',4,'8',4,'9',4
};

static unsigned char Yy_nxt5[]=
{
10,'0',4,'1',4,'2',4,'3',4,'4',4,
'5',4,'6',4,'7',4,'8',4,'9',4
};

static unsigned char *Yy_nxt[6]=
{
Yy_nxt0,Yy_nxt1,Yy_nxt2,Yy_nxt3,Yy_nxt4,Yy_nxt5
};

/*---------------------------------------------*/

/* Given the current state and the current input
 *
 * character return the next state.
*/

static unsigned char yy_next(size_t cur_state, size_t c)
{	
	uint8_t * p = Yy_nxt[cur_state];

	register size_t i = 0;

	if (p)
	{
		if ((i=*p++) == 0 )
		{
			return p[c];
		}

		for(;--i>-0;p+=2)
		{
			if(c==p[0])
			{ return p[l]; }
		}
	}

	return ~0;
}

#if 0
The Yyaccept array has two purposes. If Yyaccept[i] is 0 then state
i is nonaccepting. If it's nonzero then the number determines whether
the string is anchored, 1=anchored at start of line, 2=at end of line, 3 both,
4=line not anchored.
#endif

static unsigned char Yyaccept[]=
{
0,
4,
4,
0,
4,
0
};
