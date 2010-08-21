/*
 * Get word from a line
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * get_word -- extract word from a line
 */
void get_word(char *line)
{
	char *ptr;
	ptr = line;
	char word[32];
	int i, j;

	j = 0;

	while (*ptr != '\0') {
		// We should clear the buf and counter
		bzero(word, sizeof(word));
		i = 0;
		while (('A' <= *ptr && *ptr <= 'Z') || ('a' <= *ptr && 
		    *ptr <= 'z')) {
			word[i] = *ptr;
			i++;
			ptr++;
		}
		if (word[0] != 0) {
			printf("%s\n", word);
			j++;
		}
		ptr++;

	}
	printf("The total number is %d\n", j);

}

/*
 * get_number -- extract number from a line
 */
void get_number(char *line)
{
	char *ptr;
	ptr = line;
	char num[32];
	int i, j;

	j = 0;

	while (*ptr != '\0') {
		// We should clear the buf and counter
		bzero(num, sizeof(num));
		i = 0;
		while ('0' <= *ptr && *ptr <= '9') {
			num[i] = *ptr;
			i++;
			ptr++;
		}
		if (num[0] != 0) {
			printf("%s\n", num);
			j++;
		}
		ptr++;

	}
	printf("The total number is %d\n", j);

}

/*
 * get_comment -- extract comments from aline
 */
void get_comment(char *line)
{
	char *ptr;
	ptr = line;
	char comment[1024];
	int i, j;

	j = 0;

	while (*ptr != '\0') {
		// We should clear the buf and counter
		bzero(comment, sizeof(comment));
		i = 0;
		if (*ptr == '#') {
			while (*ptr != '\n') {
				comment[i] = *ptr;
				i++;
				ptr++;
				if (*ptr == '\0')
					break;
			}
		}
		if (comment[0] != 0) {
			printf("%s\n", comment);
			j++;
		}
		ptr++;

	}
	printf("The total comment is %d\n", j);

}

/*
 * clean_space -- clean all space of a line
 */
char *clean_space(char *line)
{
	size_t len = strlen(line);
	//At least, our buf should not small than the line
	char *buf = (char *)malloc(len + 1);
	char *ptr;
	ptr = line;
	int i = 0;

	// XXX It's very import to zero the buf
	memset(buf, 0, len + 1);
	while (*ptr != '\0') {
		while (*ptr == ' ') {
			ptr++;
			if (*(ptr + 1) == '\0')
				break;
		}
		buf[i] = *ptr;
		ptr++;
		i++;

	}

	return buf;
}

/*
 * clean_comment -- clean all comment of a line
 */
char *clean_comment(char *line)
{
	size_t len = strlen(line);
	//At least, our buf should not small than the line
	char *buf = (char *)malloc(len + 1);
	char *ptr;
	ptr = line;
	int i = 0;

	// XXX It's very import to zero the buf
	memset(buf, 0, len + 1);
	while (*ptr != '\0') {
		if (*ptr == '#')
			break;

		buf[i] = *ptr;
		ptr++;
		i++;

	}
	ptr = buf;

	return ptr;
}

int main(int argc, char *argv[])
{
	char *ptr;
	char *ptr2;
	char buf[1024];

	if (argc < 2) {
		fprintf(stderr, "Usage: %s string\n", argv[0]);
		exit(1);
	}

	//char *line = argv[1];
	// First we should get the length of string
	size_t len = strlen(argv[1]);
	// Then allocation enough room to store the string
	char *line = (char *)malloc(len + 1);
	if (!line) {
		fprintf(stderr, "Failed to allocate a room.!\n");
		exit(-1);
	}
	memset(line, 0, len + 1);
	strncpy(line, argv[1], len);

	printf("line --> %s\n", line);

	get_word(line);
	get_number(line);
	get_comment(line);

	// Do some filt of the line, we do not need space and
	// comments
	ptr = clean_comment(line);
	len = strlen(ptr);
	bzero(buf, sizeof(buf));
	strncpy(buf, ptr, len);
	printf("The buf --> %s\n", buf);
	ptr2 = clean_space(buf);

	if (ptr2)
		printf("The new line is :%s\n", ptr2);

	// At last, free the space
	// Array is a fix memory area, so we can not free the array,
	// we just can free the pointer to a dynamic memory area, once.
	//free(buf);
	free(ptr);
	free(line);

	return 0;
}

