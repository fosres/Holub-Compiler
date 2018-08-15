//sedgewick_4-15.c sedgewick_4-15.c

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_SIZE 1000

static char * stack[MAX_SIZE];

static char **stack_p = (&stack[0]-1);

static char ** stack_zero = &stack[0];

int strarrlen(char *s[]);

void reverse(char *s[],int len);

void push(char * in)
{
	++stack_p;
	char * temp = *stack_p;
	strcat(
	*++stack_p = in;
} //problem: assigns *stack_p the memory address of in, so if in changes anywhere in code so does *stack_p. That's why you get (34+34) 34 for "3 4 +"

char * pop()
{ return *stack_p--; }

void postfix_to_infix(char * postfix)
{ 
	while (*postfix != '\0')
	{
		if (isspace(*postfix) )
		{
			while (isspace(*postfix))
			{ postfix++; }

		}

		else if (isdigit(*postfix) )
		{
			static char num[1000];

			char * num_p = &num[0];

			while (isdigit(*postfix) )
			{ *num_p++ = *postfix++; }

			*num_p = '\0';

			push(num);

		}
		
		else // *postfix is an operator
		{
			#if 0
			
			Has to be an array of pointers to strings to 
			
			conserve order of multi-char parenthetical expressions
			
			#endif

			static char * par_expr[MAX_SIZE];

			static char ** par_expr_p = &par_expr[0];

			*par_expr_p++ = ")";

			*par_expr_p++ = pop();

			static char op[1];

			op[0] = *postfix++;

			*par_expr_p++ = op;

			*par_expr_p++ = pop(); 

			*par_expr_p++ = "(";

			reverse(par_expr,strarrlen(par_expr));

			//convert par_expr into a char * so you can push it into stack
			
			int i = 0;

			static char par_expr_char_arr[MAX_SIZE*MAX_SIZE];

			while (i < strarrlen(par_expr) )
			{ 
				strcat(par_expr_char_arr,par_expr[i]); 
				
				i++;
			}

			push(par_expr_char_arr);
		}

	} // end of while (*postfix != '\0')

	int i = 0;

	while (i < strarrlen(stack))
	{
		printf("%s ",stack[i]);

		i++;
	}

}

int strarrlen(char *s[])
{
	int i = 0;

	while (*s != NULL)
	{i++;s++;}

	return i;

}

void reverse(char *s[], int len)
{
	
	for ( int i = 0, j = len - 1; i < j; i++, j--)
	{
		char * temp = s[i];

		s[i] = s[j];

		s[j] = temp;

	}

}

int main()
{
	static char postfix_input[MAX_SIZE];

	static char * postfix_input_p = &postfix_input[0];

	char c;

	while ( (c = getchar()) != EOF )
	{	*postfix_input_p++ = c;	}


	postfix_to_infix(postfix_input);
}

