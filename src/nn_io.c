#ifndef NN_IO_C_
#define NN_IO_C_

#include "nn_io.h"

FILE *nn_io_fopen(const char *filename, const char *file_extension, const char *mode)
{
	FILE *fp;
	char new_filename[FILENAME_MAX];
	int filename_len, extension_len;

	NN_ASSERT(filename && file_extension && mode);

	filename_len = strlen(filename);
	extension_len = strlen(file_extension);

    NN_ASSERT(filename_len + extension_len <= FILENAME_MAX);

	sprintf(new_filename, "%s%s", filename, file_extension);

	if (!(fp=fopen(new_filename, mode))) {
		fprintf(stderr, 
            "Error: unable to open file '%s' with with mode %s.\n", 
            new_filename, mode
        );
		perror(NULL);
	}

	return fp;
}

int nn_io_get_line(char *buf, size_t bufsize, FILE *stream)
{
    int i, c;

	/* Skip spaces at the begining of the line */
	if ( (c = nn_io_skip_whitespace(stream)) == EOF )
		return EOF;

	if (c == '\n')
		return 0;
	
	
	for (
        i = 0, c = fgetc(stream); 
        c != EOF && c != '\n' && i < (bufsize-1); 
        i++, c=fgetc(stream) 
    ) 
    {
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

int nn_io_skip_whitespace(FILE *stream)
{
    int c;

	if (!stream || feof(stream))
		return EOF;

	while (isspace(c=fgetc(stream))) {
		if (c=='\n')
			return c;
	}
	
	if (c==EOF)
		return EOF;

	return ungetc(c, stream);
}


#endif /* NN_IO_C_ */
