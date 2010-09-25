#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"
#include "../core_utils.h"

static core_t *core;
// We use the handler to process the request data
// so it need to get a external data as param
// For example, it just process a specified string
void handler(void *data)
{
	char *name = (char *)data;
	if (strcmp(name, "Liunx") == 0)
		printf("Get the right data!\n");

	printf("Hello, I am the handler!\n");
	core->mods_count += 1;
	core->hello();
}

void plugin_init(plugin *plg)
{
	char *name = (char *)malloc(32);
	bzero(name, 32);
	strcpy(name, "Liunx");

	plg->version = 0x0010;
	plg->name = name;
	plg->handler = handler;
	// Get helpers from main program
	core = plg->core;

}

