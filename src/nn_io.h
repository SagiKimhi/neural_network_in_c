#ifndef NN_IO_H_
#define NN_IO_H_

/* ---------
 * Includes:
 * --------- */
#include "nn_common_includes.h"

/* ----------------------
 * Function Declarations:
 * ---------------------- */

/* Function: nn_io_fopen
 *
 * Attempts to open the specified ~filename~.~file_extension~
 * under the specified ~mode~ argument.
 * Upon success, the file pointer is returned.
 * Upon failure, an error is printed and NULL is returned. */
extern FILE *
nn_io_fopen(const char *filename, const char *file_extension, const char *mode);

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
extern int
nn_io_get_line(char *buf, size_t bufsize, FILE *stream);

/* Function: nn_io_skip_whitespace
 *
 * skips all trailing spaces in stream up until reaching a
 * newline or a non whitespace character.
 * Returns the latest character encountered, or EOF upon failure or if EOF 
 * was encountered.
 * Non whitespace characters are pushed back into stream with ungetc. */
extern int
nn_io_skip_whitespace(FILE *stream);

#endif /* NN_IO_H_ */

#ifdef NN_IO_IMPLEMENTATION

FILE *
nn_io_fopen(const char *filename, const char *file_extension, const char *mode)
{
    FILE *fp;
    char new_filename[FILENAME_MAX];
    int filename_len, extension_len;

    NN_ASSERT(filename && file_extension && mode);

    filename_len = strlen(filename);
    extension_len = strlen(file_extension);

    NN_ASSERT(filename_len + extension_len <= FILENAME_MAX);

    sprintf(new_filename, "%s%s", filename, file_extension);

    if (!(fp = fopen(new_filename, mode))) {
        fprintf(
            stderr,
            "Error: unable to open file '%s' with with mode %s.\n",
            new_filename,
            mode);
        perror(NULL);
    }

    return fp;
}

int
nn_io_get_line(char *buf, size_t bufsize, FILE *stream)
{
    int i, c;

    /* Skip spaces at the begining of the line */
    if ((c = nn_io_skip_whitespace(stream)) == EOF)
        return EOF;

    if (c == '\n')
        return 0;

    for (i = 0, c = fgetc(stream); c != EOF && c != '\n' && i < (bufsize - 1);
         i++, c = fgetc(stream)) {
        buf[i] = c;

        if (isspace(c) && nn_io_skip_whitespace(stream) == '\n')
            break;
    }

    if (i == (bufsize - 1) && !isspace(c) && c != EOF) {
        ungetc(c, stream);
    }

    buf[i] = '\0';
    return i;
}

int
nn_io_skip_whitespace(FILE *stream)
{
    int c;

    if (!stream || feof(stream))
        return EOF;

    while (isspace(c = fgetc(stream))) {
        if (c == '\n')
            return c;
    }

    if (c == EOF)
        return EOF;

    return ungetc(c, stream);
}

#endif /* NN_IO_IMPLEMENTATION */
