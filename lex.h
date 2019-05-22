#ifndef __LEX_H__
#define __LEX_H__
#include <stdint.h>


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

#endif // __LEX_H__
