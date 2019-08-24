
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
int main(void)
{
	int (*func_p)(void) = main;

	printf("%p %p\n",func_p,&main);	

	static int a;
	return 0;
}
