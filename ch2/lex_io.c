#include <stdio.h>
#include <stdarg.h>

#if 0
This file contains two output routines that replace the ones in yydebug.c

found in l.lib and used by occs for output. Link this file to a LeX-generated

lexical analyzer when a occs-generated parser is not present. Then use yycomment()

for messages to stdout, yyerror() for messages to stderr.
#endif

#if 0
Works like printf().*/
#endif

void yycomment(char* fmt,...)
{

	va_list args;

	va_start(args,fmt);

	vfprintf(stdout,fmt,args);

	va_end(args);
}

#if 0
Works like printf() but prints an error message along

with the current line number and lexeme.
#endif

void yyerror(char*fmt,...)
{
	va_list args;

	va_start (args,fmt);	

	fprintf(stderr,"ERROR on line %d, near <%s>\n",yylineno,yytext);

	vfprintf(stderr,fmt,args);

	va_end(args);
}
