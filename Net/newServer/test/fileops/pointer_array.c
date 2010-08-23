#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char **pptr = NULL;
	char *ptr = NULL;
	int i;
	for (i = 0; i < 10; i++) {
		ptr = (char *)malloc(sizeof(char *));
		pptr[i] = ptr;
	}

	for (i = 0; i < 10; i++) {
		printf("%s\n", pptr[i]);
	}

	return 0;
}
