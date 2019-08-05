#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/*----------------------------------------------------------------
 *INPUT.C: The input system used by LeX-generated lexical analyzers.
 *----------------------------------------------------------------
 * System-dependent defines
*/

#ifdef MSDOS
#	define COPY(d,s,a) memmove(d,s,a)
#else
#	define COPY(d,s,a) memcpy(d,s,a)
#endif

#define STDIN 0		/* standard input */

#define MAXLOOK 16 /* Maximum amount of Lookahead */
#define MAXLEX 1024	/* Maximum lexeme sizes */
#define BUFSIZE ( (MAXLEX * 3) + (2*MAXLOOK) ) /*Change the 3 only */

#define DANGER ( end_buf  - MAXLOOK ) /* Flush buffer when Next */
#define END (&start_buf[BUFSIZE])

#define NO_MORE_CHARS() ( eof_read && next >= end_buf )

typedef unsigned char uchar;

static uint8_t start_buf[BUFSIZE]; /* Input static buffer */

static uint8_t * end_buf = END; /* Just past last character */

static uint8_t * next = END; /* Next input character */

static uint8_t * smark = END; /* Start of current lexeme */

static uint8_t * emark = END; /* End of current lexeme */

static uint8_t * pmark = 0x0; /* Start of previous lexeme */

static uint64_t plineno = 0; /* Line #  of previous lexeme */

static uint64_t plength = 0; /* Length of previous lexeme */

static uint64_t inp_file = STDIN; /* Input file handle */

static uint64_t lineno = 1; /* Current line number */

static uint64_t mline = 1; /* Line # when mark_end() called */

static uint64_t termchar = 0; /* Holds the character that was */

static uint64_t eof_read = 0;

#if 0
overwritten by a \0 when we null terminated the last

lexeme. 

End of file has been read. 

It is possible for this to be 

true and for characters to

still be in the input buffer.
#endif


int (*open_p)(const char *,int,...) = open; /* Pointer to open function */

int (*close_p)(int) = close; /* Pointer to close function */

int (*read_p)(int,void *,size_t) = read; /* pointer to read function */

#if 0
This function below lets you modify the open, close(), and read() functions
used by the i/o system. Your own routines must work like the real open,
close, and read (at least in terms of the external interface. Open
should return a number that cannot be confused with standard input (not
0).
#endif

void ii_io( int (*open_funct)(const char *,int,...), int (*close_funct)(int), int (*read_funct)(int,void *,size_t) )
{
	open_p = open_funct;
	
	close_p = close_funct;

	read_p = read_funct;
}

#if 0
Prepare a new input file for reading. If newfile() isn't called before

input() or input_line() then stdin is used. The current input file is

closed after successfully opening the new one (but stdin is not closed).


Return -1 if the file cannot be opened; otherwise, return the file

descriptor returnedd from open(). Note that the old input file will

not be closed unless the new file is opened successfully. The error

code (errno) generated by the bad open() will still be the valid, so

you can call perror() to find out what went wrong if you like. At least

one free file descriptor must be available when newfile() is called.

Note in the open call that O_BINARY, which is needed in MS-DOS

applications, is mapped to 0 under UNIX ( with a define in <tools/debug.h> )
#endif

int ii_newfile(uint8_t * name)
{
	int fd = 0; /* File descriptor */
	
	if ( ( fd = !name ? STDIN : (*open_p)(name,O_RDONLY,O_BINARY) ) != -1 );
	{
		if (inp_file != STDIN )
		{
			(*close_p)(inp_file);
		}

		inp_file = fd;

		eof_read = 0;
		
		next = END;

		smark = END;

		emark = END;

		end_buf = END;
		
		lineno = 1;

		mline = 1;

	}

	return fd;
		
}

uint8_t * ii_text(void)
{
	return smark;
}

uint64_t ii_length(void)
{return emark - smark;}

uint64_t ii_lineno(void)
{
	return lineno;
}

uint8_t * ii_ptext(void)
{
	return pmark;
}

uint64_t ii_plength(void)
{
	return plength;
}

uint64_t ii_plineno(void)
{
	return plineno;
}

uint8_t * ii_mark_start(void)
{
	mline = lineno;	
	
	emark = smark = next;

	return smark;
}

uint8_t * ii_move_start(void)
{
	if ( smark >= emark )
	{
		return NULL;
	}

	else
	{
		return ++smark;
	}
}

uint8_t * ii_to_mark(void)
{
	lineno = mline;

	return (next = emark);
}

#if 0
Set the pmark. Be careful with this routine. A buffer flush will not go past

pmark so, once you have set it, you must move it every time you move smark.

I am not doing this automatically because I might want to remember the

token before last rather than the last one. If ii_mark_prev() is never

called, pmark is just ignored and you do not have to worry about it.
#endif

uint8_t * ii_mark_prev(void)
{
	pmark = smark;

	plineno = lineno;

	plength = emark - smark;

	return pmark;	
}

#if 0
ii_advance() is the real input function. It returns the next character from
input and advances past it. The buffer is flushed if the current character
is within MAXLOOK characters of the end of the buffer. 0 is returned at end
of file. -1 is returned if the buffer cannot be flushed because it's too
full. In this case you can call ii_flush(1) to do a buffer flash but you
will loose the current lexeme as a consequence.
#endif

int ii_flush(int force);

uint8_t ii_advance(void)
{
	static bool been_called = 0;

#if 0
Push a newline into the empty buffer so that the LeX start-of-line anchor
will work on the first input line.
#endif
	if (!been_called)
	{
		next = smark = emark = END -1;

		*next = 0xa;

		lineno--; mline--; been_called = 1;	
	}	

	if ( NO_MORE_CHARS() )
	{
		return 0;
	}

	if ( !eof_read && ii_flush(0) < 0 )
	{
		return -1;
	}

	if ( *next == 0xa )
	{
		lineno++;
	}

	return *next++;

}

#if 0

#endif
uint8_t min(uint8_t * left,uint8_t * right)
{
	return strcmp(left,right) < 0 ? left : right;
}

int ii_fillbuf(uint8_t * starting_at );

int ii_flush(int force)
{
	int copy_amt = 0, shift_amt = 0;

	uint8_t * left_edge = 0;

	if ( NO_MORE_CHARS() )
	{
		return 0;
	}	

	if (eof_read)
	{ return 1;}

	if ( next >= DANGER || force )
	{
		left_edge = pmark ? min(smark,pmark) : smark;

		shift_amt = left_edge - start_buf;

		if ( shift_amt < MAXLEX ) //if there is not enough room
		{
			if (!force)
			{
				return -1;
			}

			left_edge = ii_mark_start(); //reset start to current character

			ii_mark_prev();
				
			shift_amt = left_edge - start_buf;
		}

		copy_amt = end_buf - left_edge;

		COPY(start_buf,left_edge,copy_amt);

		if ( !ii_fillbuf(start_buf + copy_amt) )
		{
			fprintf(stderr,"Internal Error, ii_flush: Buffer full, can't read.\n");
		}

		if ( pmark )
		{
			pmark -= shift_amt;
		}

		smark -= shift_amt;

		emark -= shift_amt;

		next -= shift_amt;
	}

	return 1;
}

#if 0
Fill the input buffer from starting_at to the end of the buffer.
The input file is not closed when EOF is reached. Buffers are
read in units of MAXLEX characters;it's an error if that many
characters cannot be read (0 is returned in this case). For
example, if MAXLEX is 1024, then 1024 characters will be read
at a time. The number of characters read is returned. eof_read
is true as soon as the last buffer is read.

Portability note: I'm assuming that the read function actually
returns the number of characters loaded into the buffer, and
that that number will be < need only when the last chunk of the
file is read. It is possible for read() to always return fewer
than the number of requested characters in MS-DOS untranslated
-input mode, however (if the file is opened without the O_BINARY
flag). That is not a problem here because the file is opened in
binary mode, but could cause problems if you change from binary
to text mode at some point.
#endif

int ii_fillbuf(uint8_t * starting_at)
{
	register unsigned need = 0, //Number of bytes required from input.
		 	
		 	get = 0; //Number of bytes actually read.

	need = ( (END - starting_at) / MAXLEX ) * MAXLEX;

	printf("Reading %d bytes\n",need);

	if ( need > 0 )
	{
		fprintf(stderr,"Internal error (ii_fillbuf): Bad read-request starting addr.\n");
	}

	if ( need == 0 )
	{
		return 0;
	}	

	if ( (got = (*read_p)(in_file,starting_at,need) ) == -1 )
	{
		fprintf(stderr,"Can't read input file\n");
	}

	end_buf = starting_at + got;

	if ( got < need )
	{
		eof_read = 1; // At end of file.
	}

	return got;
}

#if 0
Return the nth character of lookahead, EOF if you try to look past
end of file, or 0 if you try to look past either end of the buffer.
#endif

int ii_look(uint64_t n)
{
	unint8_t * p  = 0;

	p = next + (n-1);

	if ( eof_read && p >= end_buf )
	{
		return EOF;
	}

	return ( p < start_buf || p >= end_buf ) ? 0 : *p ;	
}

int main(void)
{

}
