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

#define EOI		0xff	/* end of input		*/
#define SEMI		0x3b	/* ;			*/
#define EQUALS		0x3d	/* =			*/
#define PLUS		0x2b	/* +			*/
#define MINUS		0x45	/* -			*/
#define TIMES		0x2a	/* *			*/
#define DIVIDE		0x2f	/* /			*/	
#define MODULUS		0x25	/* %			*/
#define LP		0x28	/* (			*/
#define RP		0x29	/* )			*/
#define NUM		0x30	/* decimal number	*/
#define ID		0x5f	/* identifier name for a variable; it can
				   begin with an '_' 	*/	


extern uint8_t *yytext;		/* in lex.c		*/

extern uint64_t yyleng;

extern uint64_t yylineno;

static uint8_t Lookahead;

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
