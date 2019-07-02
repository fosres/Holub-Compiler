#ifndef __LISP_H__
#define __LISP_H__
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define NL	0xa

bool isendofline(void);

bool isEOI(void);

bool isoperator(uint8_t in);

size_t op_prec(uint64_t token);

extern uint8_t infix[1025]; 

extern uint8_t stack[513];

extern uint8_t prefix[1025];

extern uint8_t * infix_p; 

extern uint8_t * stack_p; 

extern uint8_t * prefix_p; 
#endif // __LISP_H__
