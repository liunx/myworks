/*
 * Read file -- Read a file into memory
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "readfile.h"
#include "getword.h"

// read_file, read full of file contents and
// store into buf
char *read_file(const char *filename)
{
	int fd;
	struct stat file_info;
	char *buffer;
	size_t len;
	/*  Open the file */
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open the file!\n");
		exit(-1);
	}
	/* Get infomation about the file */
	fstat(fd, &file_info);
	len = file_info.st_size;
	//printf("The length of file is %d\n", len);
	/* Make sure the file is an ordinary file. */
	if (!S_ISREG(file_info.st_mode)) {
		close(fd);
		return NULL;
	}
	/* Allocate a buffer large enough to hold the file's contents. */
	buffer = (char *)malloc(len);
	/* Read the file into the buffer. */
	read(fd, buffer, len);
	close(fd);
	return buffer;
}

// parse buf into lines
void parse_file(char *buf)
{
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int i, j;
	int retval;
	// We need the i to keep loop
	for (str1 = buf; ; str1 = NULL) {
		token = strtok_r(str1, "\n", &saveptr1);
		if (token == NULL)
			break;
		// Now, we get a line, then let's process it with
		// funcions in getword
		str1 = clean_comment(token);
		str2 = clean_space(str1);
		printf("The newline is %s\n", str2);
		free(str2);
		free(str1);
	}

}

