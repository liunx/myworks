#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *hello = "Hello, this---is a test";
	char *ret;
	ret = strstr(hello, "this");

	printf("The ret is : %s\n", ret);

	return 0;
}
