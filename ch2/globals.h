/*GLOBALS.H: Global variables shared between modules */
#ifdef ALLOC
#	define CLASS
#	define I(x) (x)
#else 	
#	define CLASS extern
#	define I(x)
#endif
#define MAXINP	2048

CLASS int Verbose I( = 0);
CLASS int No_lines I( = 0);
CLASS int Unix	I( = 0);
CLASS int Public I( = 0);
CLASS char *Template I(="lex.par");
CLASS int Actual_lineno I( = 1);
CLASS int Lineno I( = 1);

CLASS char Input_buf[MAXINP];
CLASS char*Input_file_name;
CLASS FILE*Ifile;
CLASS FILE*Ofile;
#undef CLASS
#undef I
