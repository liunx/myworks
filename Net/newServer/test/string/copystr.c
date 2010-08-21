/*
 * Copy a string into array
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *line = "hello, this # @, dsfasdf #----------";
	size_t len = strlen(line);
	char *buf = (char *)malloc(len + 1);
	memset(buf, 0, len + 1);

	strncpy(buf, line, len);

	// In fact, we allocate a array
	buf[2] = 100;

	printf("The buf is %s\n", buf);
	free(buf);

	return 0;
}
