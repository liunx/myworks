/*
 * A set of core functions that the server 
 * export to its mods, they are useful for mods
 * to finish it's works with main program's helpers
 */
#include <stdio.h>
#include <stdlib.h>
#include "core_utils.h"

void hello(void)
{
	printf("Hello, I'am the main program!\n");
}

void core_init(core_t *core)
{
	core->mods_count = 0;
	core->hello = hello;
}
