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
uc = 0b1000,
ucp = 0b11000, sc = 0b1001, scp = 0b11001,
usi = 0b0010,
usip = 0b10010,
ui = 0b0100,
uip = 0b10100,
uli = 0b0110,
ulip = 0b10110,
ulli = 0b1110,
ullip = 0b11110,
ssi = 0b111110,
ssip = 0b1111101,
si = 0b0101,
sip = 0b10101,
sl = 0b0111,
slp = 0b10111,
sll = 0b1111,
sllp = 0b11111,
f = 0b10000,
fp = 0b110000,
d = 0b10001,
dp = 0b110001,
ld = 0b100011,
ldp = 0b110111

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
	
	struct var * in_stack;

	struct var * func_addr;

	uint8_t name[31];
	
	value val;	
	
	typeid type_id;

	uint8_t specs;


} var;

extern var hash[6151];

extern var outcast[128];

extern var * outcast_p;

extern var stack[6151];

extern var * stack_p;

extern uint8_t * trans_units[20];

#endif
