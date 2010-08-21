/*
 * Get word from a line
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getword.h"

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

