#include <stdio.h>
#include "lex.h"

#ifndef __UINT8_T__
typedef unsigned char uint8_t;
#define __UINT8_T__
#endif

int main(void)
{
	uint8_t token = 0;

	while ( ( token = lex() ) != 0x0 )
	{	printf("%.02x\n",token);}

	return 0;
}
