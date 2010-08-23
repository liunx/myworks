#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return a char array
// Non-reentry function, because it return a pointer 
// to a data block, it means that it do some memory
// allocation, and be not freed, so we need care about
// it and free it, at last.
char *ch_return(void)
{
	char hello[] = "Hello";
	size_t len = strlen(hello);
	char *str = (char *)malloc(len);
	strcpy(str, hello);

	return str;

}

char **ch2_return(void)
{
	char **str;
	str = (char **)malloc(sizeof(char **) * 64);
	int i;

	for (i = 0; i < 10; i++) {
		str[i] = "Nice, liunx";
	}

	return str;

}

int main()
{
	int i;
	char *str;
	char **str2;
	str = ch_return();
	str2 = ch2_return();

	for (i = 0; i < 10; i++) {
		printf("The value is %s\n", str2[i]);
	}

	free(str2);
	free(str);

	return 0;

}
