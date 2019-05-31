#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#if 0
For reference see 3.7.0 in The Algorithm Design Manual
#endif

#define CHAR_MAP_SIZE 63
#define M 1572869
static uint8_t map[CHAR_MAP_SIZE];

size_t strnlen_s(const char * s,size_t maxsize)
{
	if ( s == NULL )
	{ return 0; }

	size_t i = 0;

	while ( (i < maxsize) && ( s[i] != '\0' ) )
	{
		i++;
	}

	return i;

}

#if 0
charmap =

{0..9,'A'..'Z','a'..'z','_'}

#endif
void initialize_charmap(void)
{
	size_t i = 0;

	size_t cur = 0;

	uint8_t digit =  0x30;

	uint8_t CAPITAL = 0x41;

	uint8_t lowercase = 0x61;

	while ( i < 10 )
	{
		map[i] = (digit + i);

		i++;			
	}

	cur = i;

	while ( i < (cur+26) )
	{
		map[i] = CAPITAL++; 	
		
		i++;
	}

	cur = i;

	while ( i < (cur+26) )
	{
		map[i] = lowercase++; 

		i++;
	}
	
	map[i] = '_';

}

uint8_t charmap(uint8_t c)
{
	uint8_t i = 0xff;

	while ( ++i, map[i] != c)
		;
	
	return i;
}

#if 0

m == 1572869

#endif

uint64_t hash(uint8_t * s)
{
	uint8_t * s_p = s;
	
	size_t i = 0;

	const size_t slen = strnlen_s(s,31*sizeof(uint8_t));

	double hash = 0.0;

	while ( (i < 31) && ( *s_p != 0x0 ) )
	{
		hash += pow(CHAR_MAP_SIZE,slen-(i+1))*charmap(*s_p);
		
		i++;

		s_p++;
	}

	hash = fmod(hash,M);

	return ( ( (uint64_t)hash ) % M );
}
int main(int argc, uint8_t ** argv)
{
	initialize_charmap();	
	
	printf("%llu\n",hash(argv[1]));	
	return 0;	
}
