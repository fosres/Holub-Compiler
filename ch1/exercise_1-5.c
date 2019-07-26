#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex_exp.h"

bool is_valid_expression = 0;

#if 0

	int * (*(*a[4])(int,int,int))(int,int);
	
	int *** volatile const **** p;

	int (*arr[10]) (long,bool,int);
	
	int d = 0, e = 0, g = 0; float h = 1.0;

	int (*var)(int,int,int); 

	long (((*array)))(long,bool,bool);			

	unsigned one = 0;

	int (arr_2[4]);

	int (*arz[10]) (long,bool,int);

	long long arroz(long,bool,int);
	
	int (*czar(long))[10]; // valid: a function that accepts a long argument and returns an array of 10 int elements

	int (*czar_second[10])(long); // valid array of 10 pointers to functions each accepting a long as an argument and each returning an int argument

	int (*czar_third)(long)[10];

#if 0
declare czar_third as an array of functions(long) that each returns a pointer to an int
#endif

#if 0	
invalid: viewed as an array of functions each returning an int. You cannot

declare an array of functions.
#endif
	int (*czar_fourth(long))(long);

#if 0	
   declare czar_fourth as function (long) 
   returning pointer to function 
   (long) returning int
#endif
	int (*czar_fifth(long));

   	int (* arr_pointers(long)) [100];
#if 0
declare arr_pointers as function (long) returning pointer to array 100 of int
#endif

	int (*func_pointers(long))[100];

#if 0
Declare a function that accepts argument long that returns a pointer to an array of 100 ints
#endif

	int (*func_p[45][34][99])(long,long,int);

	int (*func_2p)(long,long)(long); //invalid, you cannot make a function returning a function

	int (*func_nested(int *func_nested_two(long)))[45]; //actually invalid but gcc does not catch error

	int (*func_3p(long))[45];

	int (*func_4p(long,long))[56][65][78];

	int poisson()(); //invalid: cannot declare a function that returns a function

	int (**poison(long,long));

	int spec(int[],int ***)[45]; //invalid: function cannot return an array
	
	int (*buf[45])(unsigned long long, unsigned long);
#endif		

void storage_class_specifier(uint8_t dec_specs)
{
	switch(Lookahead)
	{
		case AUTO: 
		case STATIC: 
		case EXTERN: 
		case TYPEDEF:
		{
			dec_specs |= 0b1;

			advance();
		}

		default: {break;}
	}
}

void declaration_specifiers(void)
{
	uint8_t dec_specs = 0;

	while ( 
		( Lookahead > ( 0b1 << 8 ) )

		&&

		( Lookahead < ( 0b1 << 24 ) )		

	      )
	{
		storage_class_specifier(dec_specs);

		if ( match(UNSIGNED) || match(SIGNED) )
		{
			if ( ( dec_specs >> 2 ) & 0b1 )
			{
				fprintf(stderr,"%llu:%llu:Error: Unsigned/signed collision with float, double, or void\n",
					yylineno,yytext-&input[0]
				       );
			}

			dec_specs |= 0b10; is_valid_expression = 1;
		}

		else if ( match(INT) )
		{
			if ( 
				( ( dec_specs >> 5 ) & 0b1 )
			   )
			{
				fprintf(stderr,"%llu:%llu:Error:Extra int\n",
					yylineno,yytext-&input[0]
				       );
			}

			dec_specs |= 0b100000;

			is_valid_expression = 1;
		}

		else if ( match(FLOAT) || match(DOUBLE) || match(VOID) )
		{
			if ( 
				( ( dec_specs >> 1 ) & 0b1 )	

			   )
			{
				fprintf(stderr,"%llu:%llu:Error: Collision with signed/unsigned keyword\n",
					yylineno,yytext-&input[0]
				       );
			}

			if (
				( ( dec_specs >> 2 ) & 0b1 )
			   )
			{
				fprintf(stderr,"%llu:%llu:Error:Collsion with"
						" float,double, or void\n",
					yylineno,yytext-&input[0]
				       );
			}

			dec_specs |= 0b100; is_valid_expression = 1;
		}

		else if ( match(LONG) )
		{
			if (
				( ( dec_specs >> 2 ) & 0b1 )

			   )
			{
				fprintf(stderr,"%llu:%llu:Error:Collision with float"
						" double, or void specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if (
				( 
				 ( ( dec_specs >> 3 ) & 0b1 )
				)	
			   )
			{
				if (
					( ( dec_specs >> 4 ) & 0b1 )
				   )
				{
					fprintf(stderr,"%llu:%llu:Error:Too many"
							" \'long\' type-specifier\n",
						yylineno,yytext-&input[0]
					       );
				}

				dec_specs |= 0b10000;
			}

			else
			{
				dec_specs |= 0b1000;
			}

			is_valid_expression = 1;
		}

		else if ( match(SHORT) )
		{
			if (
				( ( dec_specs >> 2 ) & 0b1 )
			   )
				{
					fprintf(stderr,"%llu:%llu:Error:Collision of"
							" short type specifier with"
							"another type-specifier with"
							"another short, char, or"
							"long\n",
						yylineno,yytext-&input[0]
						);						
				}
				
				is_valid_expression = 1;
		}
		
		advance();		
	} //end while loop for declaration_specifiers

}

void init_declarator_list();

void pointer(void)
{
	while ( match(ASTK) )
	{
		while ( match(ASTK) )
		{
			advance();
		}

		if ( match(CONST)||match(VOLATILE) )
		{
			while (match(CONST)||match(VOLATILE))
			{
				advance();
			}
		}

	}
}

void declaration(void)
{
	while (!match(EOI))
	{
		declaration_specifiers();

		if(!is_valid_expression)
		{
			fprintf(stderr,"%llu:%llu:Error:Missing at least one"
					" type-specifier\n",
				yylineno,yytext-&input[0]
			       );
		}

		init_declarator_list();

		if (!match(SEMI))
		{
			fprintf(stderr,"%llu:%llu:Missing semicolon\n",
				yylineno,yytext-&input[0]
			       );
		}
	}
}

void init_declarator(void);

void init_declarator_list(void)
{
	while (!match(SEMI))
	{
		init_declarator();

		if (!match(COMMA) && !match(SEMI))
		{
			fprintf(stderr,"%llu:%llu:Error:Missing comma or semicolon\n",
				yylineno,yytext-&input[0]
			       );
		}

		if (match(COMMA)) { advance(); }
	}
}

void declarator(void);

void init_declarator(void)
{
	declarator();
}

void pointer(void);

void direct_declarator(void);

void declarator(void)
{
	pointer(); direct_declarator();
}

void direct_declarator(void)
{
	if ( match(ID) )
	{	advance();	}

	else if ( match(RP) )
	{
		return;
	}

	else if ( match(LP) )
	{
		advance();

		declarator();

		if ( !match(RP) )
		{
			fprintf(stderr,"%llu:%llu:Error:Missing right-parenthesis\n",
				yylineno,yytext-&input[0]
			       );
		}

		advance();
	}

	else
	{
		fprintf(stderr,"%llu:%llu:Error:Missing identifier or (declarator)\n",
			yylineno,yytext-&input[0]
		       );
	}

	if ( match(LB) )
	{
		while ( match(LB) )
		{
			advance();

			if ( !match(NUM) )
			{
				fprintf(stderr,"%llu:%llu:Error:Missing integer-"
						"constant in array\n",
					yylineno,yytext-&input[0]
				       );
			}

			advance();

			if ( !match(RB) )
			{
				fprintf(stderr,"%llu:%llu:Missing right-bracket\n",
					yylineno,yytext-&input[0]
				       );
			}

			advance();
		}
	}

	else if ( match(LP) ) //parameter_type_list
	{
		advance();

		parameter_type_list();

		if ( !match(RP) )
		{
			fprintf(stderr,"%llu:%llu:Error:Missing right-parenthesis\n",
				yylineno,yytext-&input[0]
			       );
		}	
	}	
}


int main(void)
{
	return 0;
}
