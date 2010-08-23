#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **return_list(char *str, char *split)
{
	char **pptr;
	pptr = (char **)malloc(sizeof(char **) * 32);
	// We use pointer array
	char *pstr, *savestr;
	char *token;
	int i; // for the array

	for (pstr = str, i = 0; ; i++, pstr = NULL) {
		token = strtok_r(pstr, split, &savestr);
		if (token == NULL)
			break;
		//ptr[i] = (char *)malloc(sizeof(char *));
		pptr[i] = token;
	}
	pptr[i] = NULL;

	return pptr;
}

int main()
{
	char str[] = "Name=Liunx=dsfas=adsf=dsf=asdf=asdf=asdf=sadf=asdf=as=df=asdf=as";
	char split[] = "=";
	char **ptr;
	int i;

	ptr = return_list(str, split);

	// The first string is a keyword
	// and the left is values
	for (i = 0; ptr[i] != NULL; i++) {
		printf("%s\n", ptr[i]);
	}

	free(ptr);

	return 0;

}
