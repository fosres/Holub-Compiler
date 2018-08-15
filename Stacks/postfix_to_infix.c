// sedgewick_4-15.c postfix_to_infix.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000

static char stack[MAX_SIZE][MAX_SIZE];

static int i = -1;

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
	while ( *postfix != '\0' && !iscntrl(*postfix) )
	{
		if (isspace(*postfix) )
		{
			while ( isspace(*postfix) )
			{ postfix++; }

		}

		else if ( isdigit(*postfix) )
		{
			static char num[MAX_SIZE]; 
			
			#if 0	

			compiler skips this line after initialization, despite iteration

			#endif

			int i = 0;

			while ( isdigit(*postfix) )
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

	printf("%s\n",stack[0]);

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

