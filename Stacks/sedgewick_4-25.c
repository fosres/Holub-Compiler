#include <stdio.h>
#include <ctype.h>

static int operands[1000];

static char operators[1000];

static char * op_p = &operators[0]-1;

static int * operands_p = &operands[0]-1;

void push_operand(int in)
{
  *++operands_p = in;
}

int pop_operand()
{
  return *operands_p--;
}

void push_operator(char in)
{
  *++op_p = in;
}

char pop_operator()
{
  return *op_p--;
}

int infix_eval(char * in)
{
  char * in_p = &in[0];

  while (*in_p != '\0' )
  {

    if (*in_p == '(' || *in_p == ' ' )
    {
      in_p++;
    }
    else if ( isdigit(*in_p) )
    {
      push_operand( (*in_p - '0' ) );

      in_p++;

      while ( isdigit(*in_p) ) // can skip over a right parenthesis!!!
      {
        *operands_p *= 10;

        *operands_p += (*in_p - '0' );

        in_p++;

      }

    
    }

    else if (*in_p == '+' || *in_p == '-' || *in_p == '*' || *in_p == '/')
    {

      push_operator(*in_p);

      in_p++;

    }
    

    else // ( *in_p == ')' )
    {
      int op2 = pop_operand();

      char operator = pop_operator();
      
      if (operator == '+')
        {
          push_operand(pop_operand()+op2);
        }
      else if (operator == '-')
        {
          push_operand(pop_operand()-op2);
    
        }
      else if (operator == '*')
        {
          push_operand(pop_operand()*op2);
        
        }
      else // operator == '/'
        {
          push_operand(pop_operand()/op2);
        
        }

        in_p++;
      }

    }

  

  

  return operands[0];
}

int main(void) {
  
  static char s[1000];

  static char * s_p = &s[0];

  char c;

  while ( (c = getchar()) != '\n' )
  {
    *s_p++ = c;
  }

  *s_p++ = '\n';

  *s_p = '\0';

  printf("%d\n",infix_eval(s));
  
  return 0;
}
