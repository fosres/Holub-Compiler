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

	