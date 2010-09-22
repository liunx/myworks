#include <stdio.h>
#include <stdlib.h>
#include "../plugin.h"

void handler(void)
{
	printf("Hello, I am the handler!\n");
}

void plugin_init(plugin *plg)
{
	plg->version = 0x0010;
	plg->name = NULL;
	plg->handler = NULL;

}

