#include <stdio.h>
#include <stdint.h>

uint8_t * Names[] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7" };

uint8_t ** Names_p = Names;

uint8_t * newname(void)
{
	if ( Names_p >= &Names[sizeof(Names)/sizeof(*Names) ] )
	{
		fprintf(stderr,"%llu: Expression too complex\n",yylineno);

		exit(1);
	}

	return (*Names_p++);
}

void freename(uint8_t * s)
{
	
	if ( Names_p > Names )
	{
		*--Names_p = s;
	}

	else
	{
		fprintf(stderr,"%llu: (Internal error) Name stack underflow\n",yylineno);
	}

}
