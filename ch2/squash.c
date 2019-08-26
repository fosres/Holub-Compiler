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

/*----------------------------------------------
#define ROW_EQUIV(r1,r2,ncols) (memcmp(r1,r2,ncols*sizeof(int))==0)
#define ROW_CPY(r1,r2,ncols


