#include "io.h"
void error_msg(char * error_msg,uint64_t lineno,uint64_t charno)
{
	printf("\e[38;5;208m"); //Orange	
	
	fprintf(stderr,"%llu:%llu:Error:%s\n",lineno,charno,error_msg);

	printf("\e[0m");

	printf("\e[38;5;201m"); //Purple					
	
	printf("%s",input); //no need for newline char, already at end of line!
	
	printf("\e[0m");

	uint8_t * char_p = input;

	printf("\e[38;5;208m"); //Orange	

	while ( char_p <  yytext )
	{
		putchar(0x20);

		char_p++;
	}

	putchar('^');

	char_p++;

	while ( char_p < yycurrent )
	{
		putchar('~');

		char_p++;
	}

	printf("\e[0m");

	putchar(0xa);

	putchar(0xa);
	

}

