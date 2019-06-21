#ifndef __LEX_H__
#define __LEX_H__
#include <stdint.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 1024

#ifndef __UINT8_T__
#define __UINT8_T__
typedef unsigned char uint8_t;
#endif


#if 0
All the hex representations are simply put--the ASCII hex codes
#endif

// Symbols that are part of the ASCII character field (Except ID)
#define EOI		0xff	/* end of input		*/
#define SEMI		0x3b	/* ;			*/
#define EQUALS		0x3d	/* =			*/
#define PLUS		0x2b	/* +			*/
#define MINUS		0x45	/* -			*/
#define TIMES		0x2a	/* *			*/
#define INDIR		0x2b	/* *			*/
#define DIVIDE		0x2f	/* /			*/	
#define MODULUS		0x25	/* %			*/
#define LP		0x28	/* (			*/
#define RP		0x29	/* )			*/
#define NUM		0x30	/* decimal number	*/

#define ID		0x5f	/* identifier name for a variable; it can
				   begin with an '_' 	*/	
#if 0
From this point onward, 

Storage-class specifiers, type-specifiers, and type-qualifiers

#endif

// Storage-class specifiers
#define AUTO		(  ( ( 0b001 ) << 8 ) | (0x100)  )	
#define STATIC		(  ( ( 0b010 ) << 8 ) | (0x100)  )
#define EXTERN		(  ( ( 0b011 ) << 8 ) | (0x100)  )
#define TYPEDEF		(  ( ( 0b100 ) << 8 ) | (0x100)  )	

// Type-specifiers
#define VOID		(  ( ( 0b0 ) << 12 ) | ( 0x1100 )  )
#define BOOL		(  ( ( 0b1 ) << 12 ) | ( 0x1100 )  )
#define CHAR		(  ( ( 0b101 ) << 14 ) | ( ( 0x1100 ) )  )
#define SHORT		(  ( ( 0b010 ) << 14 ) | ( ( 0x1100 ) )  )
#define INT		(  ( ( 0b011 ) << 14 ) | ( ( 0x1100 ) )  )
#define LONG		(  ( ( 0b100 ) << 14 ) | ( ( 0x1100 ) )  )
#define FLOAT		0x109
#define DOUBLE		0x10a
#define SIGNED		(  ( ( 0b1 ) << 13 ) | ( ( 0x1100 ) )  )
#define UNSIGNED	(  ( ( 0b0 ) << 13 ) | ( ( 0x1100 ) )  )

// Type-qualifiers
#define CONST		0x10e
#define VOLATILE	0x10f

//Structs and unions
#define STRUCT		0x110
#define UNION		0x111



extern uint8_t *yytext;		/* in lex.c		*/

extern uint64_t yyleng;

extern uint64_t yylineno;

extern static uint8_t Lookahead;

unsigned int lex(void);

bool match(uint8_t token);

void advance(void);

void expression(void);

void expression_improved(void);

void expr_prime(void);

void term(void);

void term_improved(void);

void term_prime(void);

void factor(void);

void factor_improved(void);

void statements(void);

void statements_improved(void);

uint8_t * newname(void);

void freename(uint8_t * s);

#endif // __LEX_H__
