#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interpreter.h"

var hash[6151];

var outcast[128];

var * outcast_p = &outcast[0] - 1;

var stack[6151];

var * stack_p = &stack[0] - 1;

uint8_t * trans_units[20];

int init_data(char * argv[])
{
	char c = 0x0;

	size_t sz = 20*sizeof(uint8_t*);
	
	memset(trans_units,0x0,20*sizeof(uint8_t*));
	
	FILE * in = NULL;

	size_t file_size = 0;

	size_t i = 0;
	
	while ( *++argv != NULL )
	{
	
	if ( ( in = fopen(*argv,"r") ) == NULL )
	{
		fprintf(stderr,"File reading failed\n");	
		
		return -1;
	}

	fseek(in,0L,SEEK_END);

	file_size = ftell(in);

	rewind(in);
	
	trans_units[i] = (uint8_t *)calloc(file_size,sizeof(uint8_t));
	
	fread(trans_units[i],sizeof(uint8_t),file_size,in);
	
	if ( fclose(in) != 0 )
	{
		printf("File closing failed.\n");
	
		return -1;
	}

	i++;
}
	
	return 0;
}

void free_trans_units(void)
{
	size_t i = 0;
	
	while ( trans_units[i] != 0x0 )
	{

	free(trans_units[i]);

	i++;
	
	}	

}

int main(int argc, char * argv[])
{
	
	init_data(argv);
	
	printf("%llu\n",sizeof(typeid));

	int (*a)(int,char **) = main;


	unsigned long long int abacus;
		
	return 0;

}
