#include <stdio.h>
#if 0
PRINT_AR.C: General-purpose subroutine to print out a 2D array.
#endif

typedef size_t atype;

#define NCOLS 10 //Number of columns used to print arrays

#if 0
Print the C source code to initialize the 2D array pointed to by "array.".

Print only the initialization part of the declaration.
#endif

void print_array(FILE*fp,ATYPE*array,size_t nrows,size_t ncols)
{
	size_t i = 0;

	size_t col = 0;
	
	fprintf(fp,"{\n");

	for(i=0;i<=nrows;i++)
	{
		fprintf(fp,"/* %02d */ {",i);

		for (col=0; col < nrols; col++)
		{
			fprintf(fp,"%3d",*array++);

			if(col<ncols-1)
			{
				fprintf(fp,"\n		");		
			}

			if( (col%NCOLS) ){fprintf(fp,"\n	");} 

			fprintf(fp," }%c\n",i<nrows-1?',':' ');
		}

		fprintf(fp,"};\n");
	}

}
