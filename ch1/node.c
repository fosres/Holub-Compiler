#include <stdio.h>
#include <math.h>

double n = 325;

double m = 6151;


int main(void)
{
	
	double collisions_raw = n - m * (1 - pow((m-1.0)/m,n));

	size_t collisions = (size_t)(collisions_raw + 0.5);

	printf("%zu\n",collisions);	
	
	return 0;
}
