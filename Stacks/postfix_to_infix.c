// sedgewick_4-15.c postfix_to_infix.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000

static char stack[MAX_SIZE][MAX_SIZE];

static int i = -1;

static int num_newlines = 0;

void reverse(char s[])
{
	for (int i = 0, j = strlen(s)-1; i < j; i++, j--)
	{
		char temp = s[i];

		s[i] = s[j];

		s[j] = temp;
	}
}

void push(char * s)
{
	i++;

	stack[i][0] = '\0';

	strcat(stack[i],s);

	strcat(stack[i],"\0");
}

char * pop()
{ return stack[i--]; }

char * top()
{ return stack[i]; }

void postfix_to_infix(char * postfix)
{
	while ( *postfix != '\0' )
	{

		if ( iscntrl(*postfix) && *postfix == '\n' )
		{
			num_newlines++;

			postfix++;

		}

		else if (isspace(*postfix) )
		{
			while ( isspace(*postfix) )
			{ postfix++; }

		}

		else if ( isalnum(*postfix) )
		{
			static char num[MAX_SIZE]; 
			
			#if 0	

			compiler skips line 54 after initialization, despite iteration

			#endif

			int i = 0;

			while ( isalnum(*postfix) )
			{ num[i++] = *postfix++; }

			num[i] = '\0';

			push(num);
		}

		else // *postfix is an operator (+,-,*,/,=)
		{
			static char temp[MAX_SIZE];

			temp[0] = '\0';

			strcat(temp,")");	

			reverse(top());

			strcat(temp,pop()); // right operand

			static char op[1];

			op[0] = *postfix++;

			strcat(temp,op);

			reverse(top());

			strcat(temp,pop()); // left operand

			strcat(temp,"(");

			strcat(temp,"\0");

			reverse(temp);

			push(temp);
		}

	}

	int print_index = 0;

	putchar('\n');

	while ( print_index < num_newlines )
	{ printf("%s\n",stack[print_index++]); }

}

int main()
{
	static char postfix[MAX_SIZE];

	static char * postfix_p = &postfix[0];

	char c;

	while ((c = getchar()) != EOF)
	{ 
		*postfix_p++ = c;
	}

	postfix_to_infix(postfix);

}

