#include <stdio.h>
#include <stdlib.h>
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

#define DANGER ( End_buf  - MAXLOOK ) /* Flush buffer when Next */
#define END (&start_buf[BUFSIZE])

#define NO_MORE_CHARS() ( Eof_read && Next >= End_buf )

typedef unsigned char uchar;

static uint8_t start_buf[BUFSIZE]; /* Input static buffer */

static uint8_t * end_buf = END; /* Just past last character */

static uint8_t * next = END; /* Next input character */

static uint8_t * smark = END; /* Start of current lexeme */

static uint8_t * emark = END; /* End of current lexeme */

static uint8_t * pmark = 0x0; /* Start of previous lexeme */

static uint64_t plineno = 0; /* Line #  of previous lexeme */

static uint64_t plength = 0; /* Length of previous lexeme */

static uint64_t in_file = STDIN; /* Input file handle */

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

extern int open(), close(), read();

int (*openp)(void) = open; /* Pointer to open function */

int (*closep)(void) = close; /* Pointer to close function */

int (*readp)(void) = read; /* pointer to read function */



int main(void)
{

}
