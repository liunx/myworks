#include <stdio.h>
#include <stdlib.h>
#include "plugin.h" // it contains the apr libs


static void append_items_to_array(apr_array_header_t *arr, plugin *plg);

/*
 * register_plugin -- register the plugin struct
 * into public server struct
 */
int register_plugin(server *srv, plugin *plg, apr_pool_t *mp)
{
	// first, check the version match
	if (srv->version != plg->version) {
		fprintf(stderr, "Version mismatched!\n");
		return -1;
	}
	// then, put the plg into a slot
	// we should allocate a room for the new comer
	append_items_to_array(srv->slots, plg);

	return 0;

}
/*
 * unregister_plugin -- remove the plugin
 * free the memory of the plugin, and make the pointer to 
 * be NULL.
 */
void unregister_plugin(server *srv, char *name)
{
	int i;
	for (i = 0; i < srv->slots->nelts; i++) {
		plugin *plg = ((plugin **)srv->slots->elts)[i];
		if (!strcmp(plg->name, name)) {
			// remove the plugin
			free(plg);
			plugin **wildp = &((plugin **)srv->slots->elts)[i];
			// look care about wild pointer
			*wildp = NULL;	
		}
	}

}

// put the plug pointer to the slots
// I wont export the append_items_to_array
static void append_items_to_array(apr_array_header_t *arr, plugin *plg)
{
	*(const plugin **)apr_array_push(arr) = plg;
}
