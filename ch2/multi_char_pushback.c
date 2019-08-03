#include <stdio.h>
#include <string.h>

#define SIZE 256

#if 0
Pbackbuf is the push_back stack

Pbackp 		is the stack pointer. The stack grows down, so a push is:
		*--Pbackp=c and a pop is: c=*Pbackp++

get()		evaluates to the next input character, either popping it off the
		stack (if it's not empty) or by calling getc().

unget(c)	pushes c back. It evaluates to c if successful, or to -1 if the
		pushback stack was full.
		
#endif

uint8_t  Pbackbuf[SIZE];

uint8_t * Pbackp = &Pbackbuf[SIZE];

uint8_t get(FILE * stream) { Pbackp < &Pbackbuf[SIZE] ? *Pbackp++ : getc(stream); }
uint8_t unget(uint8_t c)	{ Pbackp <= Pbackbuf	? -1	: (*--Pbackp=(c)); }

void ungets( uint8_t * start , size_t n )
{
#if 0
	Push back the last n characters of the string by working backwards

	through the string.	
#endif

	uint8_t * p = start + strlen(start); //Find the end of the string.

	while ( --p >= start && --n >= 0 )
	{
		if ( unget(*p) == -1 )
		{
			fprintf(stderr,"Pushback-stack overflow\n");
		}	
	}
}

int main(){}
