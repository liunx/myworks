#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

/*
 * register_plugin -- register the plugin struct
 * into public server struct
 */
int register_plugin(server *srv, plugin *plg)
{
	// first, check the version match
	if (srv->version != plg->version) {
		fprintf(stderr, "Version mismatched!\n");
		return -1;
	}
	// then, put the plg into a slot
	// we should allocate a room for the new comer
	srv->

}
