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

#define BUFSIZE	32
#define FILENAME	"test.conf"

char *read_file(const char *filename, size_t *length)
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
	printf("The length of file is %d\n", len);
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

int main()
{
	size_t *length;
	char *buf = read_file(FILENAME, length);
	printf("%s", buf);
	return 0;
	
}
