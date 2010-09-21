#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void *funlib;
void (*fun)(); 

int main()
{
	funlib = dlopen("./libhello.so", RTLD_LAZY);
	fun = dlsym(funlib, "hello");
	fun();

	return 0;
}
