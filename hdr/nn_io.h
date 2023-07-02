#ifndef NN_IO_H_
#define NN_IO_H_

/* ---------
 * Includes:
 * --------- */
#include "nn_common_includes.h"


/* --------
 * Defines:
 * -------- */


/* ---------
 * Typedefs:
 * --------- */


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

/* Function: nn_io_fopen
 *
 * Attempts to open the specified ~filename~.~file_extension~
 * under the specified ~mode~ argument.
 * Upon success, the file pointer is returned.
 * Upon failure, an error is printed and NULL is returned. */
extern FILE *nn_io_fopen(const char *filename, const char *file_extension, const char *mode);

/* Function: nn_io_get_line
 *
 * TLDR: 
 * like fgets, but discards trailing whitespace, and discards the newline character
 *
 * Get the next line of input from ~stream~ into ~buf~ while skipping
 * and discarding any trailing whitespace characters.
 * ~bufsize~ specifies the max number of characters to be read into ~buf~. 
 * The function stops when either bufsize limit is reached, newline character 
 * is reached (and skipped), or EOF is reached. 
 * The return value is the total number of characters read and/or skipped. */
extern int nn_io_get_line(char *buf, size_t bufsize, FILE *stream);

/* Function: nn_io_skip_whitespace
 *
 * skips all trailing spaces in stream up until reaching a
 * newline or a non whitespace character.
 * Returns the latest character encountered, or EOF upon failure or if EOF 
 * was encountered.
 * Non whitespace characters are pushed back into stream with ungetc. */
extern int nn_io_skip_whitespace(FILE *stream);

#endif /* NN_IO_H_ */

