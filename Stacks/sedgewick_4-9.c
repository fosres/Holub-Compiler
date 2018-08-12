//sedgewick_4-9.c

#include <stdio.h>
#include "stack.h"
#define MAX_SIZE 1000

STACK_TYPE stack[MAX_SIZE]; STACK_TYPE *stack_p = (&stack[0]-1);

void push(STACK_TYPE in)
{ *++stack_p = in; }

STACK_TYPE pop()
{ return *stack_p--; }

void infix_to_postfix(char * s)
{

while(*s != '\0')
{
		if (*s == '(' || *s == ' ')
		{ }
		else if(*s >= '0' && *s <= '9')
		{ printf("%c ",*s);}
		else if (*s == '*' || *s == '-' || *s == '+' || *s == '/')
		{ push(*s); }
		else // *s == ')'
		{ printf("%c ",pop()); }

	s++;
}

}

int main()
{
char s[MAX_SIZE]; char *s_p = s;
char c;
while ( (c = getchar()) != '\n')
{
	*s_p++ = c;
}

infix_to_postfix(s);

return 0;
}
