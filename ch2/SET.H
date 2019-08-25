#ifndef __SET_H__
#define __SET_H__
typedef struct _set_
{
    unsigned short defmap[8];	/* The map itself		  */
    unsigned short *map   ;		/* Pointer to the map		  */
    unsigned	  nbits  ;		/* Number of bits in map	  */
    unsigned char nwords ;		/* Number of words in map	  */
    unsigned char compl	 ;		/* is a negative true set if true */

} SET;

typedef int	(*pset_t)	( void* param, char *fmt, int val ) pset_t;
extern	void	 delset		P(( SET*			 ));
extern	void	 pset		P(( SET*, pset_t, void* 	 ));


//typedef unsigned short     _SETTYPE ;		   one cell in bit map   

extern	int	 _addset ( SET* , int );
extern	void	 _set_op( int,	SET*, SET* 	 );

#define _DIV_WSIZE(x)      ((unsigned)(x) >> 4)

#define _GBIT(s,x,op) ( ((s)->map)[_DIV_WSIZE(x)] op (1 << _MOD_WSIZE(x)) )
#define ADD(s,x)     (((x) >= (s)->nbits) ? _addset(s,x) : _GBIT(s,x,|=  ) )
#define COMPLEMENT(s)	( (s)->compl = ~(s)->compl )
#define ASSIGN(d,s)	   _set_op( _ASSIGN,     d, s )


#endif //__SET_H__
