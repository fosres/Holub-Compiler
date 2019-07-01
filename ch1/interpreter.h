#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#ifndef __UINT8_T__
typedef unsigned char uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef unsigned short uint16_t;
#define __UINT16_T__
#endif

typedef enum
{
v = 0b0,
vp = 0b10,
b = 0b1,
bp = 0b11,
uc = 0b1010,
ucp = 0b11010, 
sc = 0b1011, 
scp = 0b11011,
usi = 0b0100,
usip = 0b10100,
ui = 0b0110,
uip = 0b10110,
uli = 0b1000,
ulip = 0b11000,
ulli = 0b1001000,
ullip = 0b11001000,
ssi = 0b0101,
ssip = 0b10101,
si = 0b0111,
sip = 0b10111,
sl = 0b1001,
slp = 0b11001,
sll = 0b1001001,
sllp = 0b11001001,
f = 0b1100,
fp = 0b11100,
d = 0b1101,
dp = 0b11101,
ld = 0b1110,
ldp = 0b11110

} typeid;

	typedef union
	{
		bool b;
		
		bool * bp;
		
		unsigned char uc;

		unsigned char * ucp;

		signed char sc;

		signed char * scp;
		
		unsigned short int usi;

		unsigned short int * usip;

		signed short int ssi;

		signed short int * ssip;
		
		unsigned int ui;

		unsigned int * uip;

		signed int si;

		signed int * sip;

		unsigned long int uli;

		unsigned long int * ulip;

		signed long int sl;

		signed long int * slp;

		unsigned long long int ull;

		unsigned long long int * ullp;

		signed long long int sll;

		signed long long int * sllp;

		float f;

		float * fp;

		double d;

		double * dp;

		long double ld;
		
		long double * ldp;
		
		void * vp;

	} value;


typedef struct var
{
	uint8_t name[31];
	
	struct var * in_stack;
	
	size_t offset;
	
	value val;	
	
	typeid type_id;
#if 0

inlcudes info on static, extern, and what index c it is in the *argv[] list

for translation units

7 6 | 5 4 3 2  | 1 | 0

Bits 7-2 for index of c in * argv[] list

Bit 1 for extern

Bit 0 for static

#endif

	uint8_t specs; 


} var;

extern var hash[6151];

extern var outcast[128];

extern var * outcast_p;

extern var stack[6151];

extern var * stack_p;

extern uint8_t * trans_units[20];

#endif
