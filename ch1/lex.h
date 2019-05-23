#ifndef __LEX_H__
#define __LEX_H__
#include <stdint.h>
#include <stdbool.h>

#define EOI		0	/* end of input		*/
#define SEMI		1	/* ;			*/
#define PLUS		2	/* +			*/
#define TIMES		3	/* *			*/
#define LP		4	/* (			*/
#define RP		5	/* )			*/
#define NUM_OR_ID	6	/* decimal number or 

				   identifier		*/

extern uint8_t *yytext;		/* in lex.c		*/

extern uint64_t yyleng;

extern uint64_t yylineno;

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

#endif // __LEX_H__
