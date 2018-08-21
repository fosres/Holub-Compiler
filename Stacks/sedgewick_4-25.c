#include <stdio.h> //infix evaluation
#include <ctype.h>

static int stack[1000];

static char postfix[1000];

static char * postfix_p = &postfix[0]-1;

static int stack_p = &stack[0]-1;

void push(int in)
{
	*++stack_p = in;
}

int pop()
{
	return *stack_p--;
}

void infix_to_postfix(char * s)
{

while(*s != '\0')
{
		if (*s == '(' || *s == ' ')
		{ }
		else if( isdigit(*s) )
		{
			while (isdigit(*s)
			*postfix_p++ = *s; }
		else if (*s == '*' || *s == '-' || *s == '+' || *s == '/')
		{ push(*s); }
		else // *s == ')'
		{ printf("%c ",pop()); }

	s++;
}

}


int eval_infix_int(char infix[])
{
	char * infix_p = &infix[0];

	while (*infix_p != '\0')
	{
		if (isdigit(*infix_p) )
		{
			push( (*infix_p - '0') ); infix++;

			while ( isdigit(*infix_p) )
			{
				*stack_p *= 10; *stack_p += *infix_p;

				infix_p++;
			}
			// remember stack began @ (&stack[0]-1)
		}
		
		else if ( *infix == '+' )
		{
			int op2 = pop();

			push(pop() + op2);

			infix_p++;
		}

		else if ( *infix_p == '-')
		{
			int op2 = pop();

			push(pop()-op2);

			infix_p++;
		}
		
		else if ( *infix_p == '*')
		{
			int op2 = pop();

			push(pop() * op2);

			infix_p++;
		}

		else // *infix_p == '/'
		{
			int op2 = pop();

			push(pop()/op2);

			infix_p++;
		}


	}

	// end of while (*infix_p != '\0')
	
	return stack[0];

}

int main()
{

	char * s = "
}
