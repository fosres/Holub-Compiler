#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "dfa.h"
#include "globals.h"

#define NCOLS	16
#define TYPE	"YY_TYPE"
#define SCLASS	"YYPRIVATE"

#if 0
SQUASH.C --This module contians the routines to compres a table
horizontally and vertically by removing redudant columns and rows,
and then print the compressed table. I have not been as careful
about making this routine general purpose because it is only
useful to LeX. The pairs compression routines in pairs.c are used
to compress the occs-and llama-generated tables so they are a
little more complicated.
#endif

static size_t Col_map[MAX_CHARS];

static size_t Row_map[DFA_MAX];

static size_t col_equiv(int*,int*,int);

static void col_cpy(int*,int*,int,int,int);

static void reduce(ROW*,size_t*,size_t*);

static void print_col_map(FILE*	);

static void print_row_map(FILE*,int*,int);

void pmap(FILE*,int*,int);

void cnext(FILE*,uint8_t*);

size_t squash(FILE*,ROW*,size_t,size_t,uint8_t*);

/*----------------------------------------------*/
#define ROW_EQUIV(r1,r2,ncols) (memcpy(r1,r2,ncols*sizeof(int))==0)
#define ROW_CPY(r1,r2,ncols) (memcpy(r1,r2,ncols*sizeof(int))

/*-------------------------------------------------*/

#if 0
Compress (and output) dtran using equivalent-column elimination.
Return the number of bytes required for the compressed tables
including the map but not the accepting array.
#endif
size_t squash(FILE*fp,ROW*dtran,size_t nrows,size_t ncols,uint8_t*name)
{
	size_t oncols = ncols;

	size_t onrows = nrows;

	reduce(dtran,&nrows,&ncols);

	print_col_map(fp);

	print_row_map(fp,onrows);

fprintf(fp,"%s %s %s[ %d ][ %d ]=\n",SCLASS,TYPE,name,nrows,ncols);

print_array(fp,(int*)dtran,nrows,ncols);

return ( ( nrows * ncols * sizeof(TTYPE))
	+(	onrows * sizeof(TTYPE))
	+(	oncols * sizeof(TTYPE))
       );

}

/*-----------------------------------------------------*/

static size_t col_equiv(size_t*col1,size_t*col2)
{
	/* Return 1 if the two columns are equivalent, else return 0 */
	
	while(--nrows >= 0 && *col1 == *col2)
	{
		col1+=MAX_CHARS;

		col2+=MAX_CHARS;
	}

	return (!(nrows >= 0 );	
}

/*------------------------------------------------------*/

#if 0
Copy a column from one array to another. Both arrays are

nrows dep, the source array is n_src_cols wide and the

destination array is n_dest_cols wide.
#endif

static void col_cpy(size_t*dest,size_t*src,size_t nrows,size_t n_src_cols,size_t n_dest_cols)
{
	while(--nrows>=0)
	{
		*dest=*src;

		dest+=n_dest_cols;

		src+=n_src_cols;
	}	
}

/*------------------------------------------------------------*/

#if 0
Reduce dtran horizontally and vertically, filling the two map

arrays with the character mapping to the final, reduced transition

table. Return the number of columns in the reduced dtran table.

Col_map is the x (character) axis, Row_map is the y (next state) axis.
#endif

static void reduce(ROW*dtran,size_t*p_nrows,size_t*p_ncols)
{
	size_t ncols=*p_ncols; /*number of columns in original machines */

	size_t nrows=*p_nrows; /*number of rows in original machine */

	size_t r_ncols = 0; // number of columns in reduced machine

	size_t r_nrows = 0; // number of rows in reduced machine

	SET*save=0; // rows or columns that will remain in table

	size_t*current=0; // first of several identical columns

	size_t*compressed=0; // pointer to compressed array

	size_t*p=0;

	size_t i=0,j=0;

	memset(Col_map,(~0),sizeof(Col_map));

	save = newset();

#if 0
Skip past any states in the Col_map that have already been processed. If the entire Col_map has been processed, break.
#endif
	for(r_ncols=0;;r_ncols++)
	{
		for(i=r_ncols;Col_map[i]!=(~0) && i < ncols; i++ )
			;

		if ( i>= ncols )
		{
			break;
		}
#if 0
Add the current column to the save set. It eventually ends up in the reduced

array as column "r_ncols" so modify the Col_map entry accordingly. Now, scan

through the array looking for duplicates of the current column (pointed to

by current). If you find a duplicate, make the associated Col_map entry also

point to "r_ncols".
#endif

	ADD(save,i);

	Col_map[i] = r_ncols;

	current = &dtran[0][i];

	p	= current + 1;

	for (j=i;++j<ncols;p++)
	{
		if(Col_map[j]==(~0) && col_equiv(current,p,nrows))
		{
			Col_map[j]=r_ncols;
		}
	}

#if 0
Compress the array horizontally by removing all of the columns that

are not in the save set. We are doing this by moving all the columns

that are in the save set to the proper position in a newly allocated

array. You cannot do it in place because there is no gurantee that the

equivalent rows are next to each other.
#endif

	}
	


