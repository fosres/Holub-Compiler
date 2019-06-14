#include <stdio.h>
#include <stdbool.h>
typedef enum
{
v = 0b0,
b = 0b1,
uc = 0b1000,
sc = 0b1001,
usi = 0b0010,
ui = 0b0100,
uli = 0b0110,
ulli = 0b1110,
ssi = 0b0011,
si = 0b0101,
sl = 0b0111,
sll = 0b1111,
f = 0b10000,
d = 0b10001,
ld = 0b10111,
ptr = 0b11111,

} typeid;

typedef struct var
{
	typeid type_id;

	union
	{
		bool b;
		
		unsigned char uc;

		signed char sc;
		
		unsigned short int usi;

		signed short int ssi;
		
		unsigned int ui;

		signed int si;

		unsigned long int uli;

		signed long int sl;

		unsigned long long int ull;

		signed long long int sll;

		float f;

		double d;

		long double ld;

		void * ptr;

	} value;
		

} var;

uint8_t storage_specs = 0;
int main(void)
{
	printf("%llu\n",sizeof(size_t));
	return 0;
}
