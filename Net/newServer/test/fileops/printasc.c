#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i;
	for (i = 0; i < 128; i++) {
		printf("%d--%c\n", i, i);
	}

	printf("A - a = %d\n", 'a' - 'A');
	return 0;
}
