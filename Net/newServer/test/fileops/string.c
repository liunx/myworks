/**
 * do some experiment about string functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_word(char *line)
{
	char *ptr;
	char buf[32]; // store the word
	ptr = line;
	int i = 0; // Get the total number of words
	int j = 0;
	while (*ptr != '\0') {
		//i++;
		ptr++;
		j = 0;
		bzero(buf, sizeof(buf));
		while(('A' <= *ptr && *ptr <= 'Z') || ('a' <= *ptr && *ptr <= 'z')) {
			buf[j] = *ptr;
			j++;
			ptr++;		
		}
		if (buf[0] != 0) {
			i++;
			printf("The word is %s\n", buf);
		}
	}
	printf("The total number of words is %d\n", i);
}

int main()
{
	char buf[] = "          liunx, this is ";
	char *ptr = buf;

	get_word(buf);
	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr == '#')
		printf("This line is a comment.\n");

	printf("The res is : %s\n", ptr);

	return 0;

}
