#include <stdio.h>

int func(int a,int b)
{
	return a;
}

int fac(int (*func_p)(int a,int b));

int main(void)
{
	long long int a = 3;
	
	a	++;

	printf("%llu\n",a);	
	
	return 0;
}
