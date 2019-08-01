#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	int (*czar_third)(long)[10];

	int (*czar_third)[10](long);
	return 0;
}
