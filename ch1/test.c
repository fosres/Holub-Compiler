#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
int main(void)
{
	void * a = (int *)calloc(1,sizeof(int));

	*(int *)a = 3;

	printf("%d\n",*(int *)a);

	double b = (double)(UINT64_MAX);

	printf("%.12f\n",b);

	uint64_t c = UINT64_MAX;

	printf("%llu\n",c);

	return 0;
}
