#include <stdio.h>

#define LT	1
#define GT	2
#define EQUAL	3
#define NOT	4
#define LEQ	5
#define GEQ	6
#define NEQ	7
#define ASSIGN	8

#define ERROR	9
#define CONTINUE	10
#define SIZE_OF_CHARACTER_SET	11

uint8_t first[SIZE_OF_CHARACTER_SET];
uint8_t second[SIZE_OF_CHARACTER_SET];

int s = 0, f = 0;

memset(first, -1, SIZE_OF_CHARACTER_SET); //Initialize to error token

memset(second, -1, SIZE_OF_CHARACTER_SET); //Note that there's an implicit.

					// conversion of -1 to 255 here
					// (signed int to unsigned char)
first['>'] = GT
first['<'] = LT;
first['!'] = NOT;
first['='] = ASSIGN;
second['='] = EQUAL;

c = getchar();

if ( (f = first[c]) == ERROR ) //discard bad character
{return ERROR;}

if ( (s = second[c]) == ERROR ) //1-character lexeme
{
	ungetchar();

	return f;
}

else
{
	if ( s == EQUAL )
	{
		switch(f)
		{
			case ASSIGN: return EQ;
			case LT: return LEQ;
			case GT: return GEQ;
			case NOT: return NEQ;
		}

		return ERROR; //discardboth characters
	}

