#ifndef _PLUGIN_H_
#define _PLUGIN_H_
#include <apr_general.h>
#include "server.h"

/*
 * plugin -- the core plugin struct
 */
typedef struct {
	size_t version; // plugin version
	char *name;	// plugin name
	void *(*handler)(void);	// the handler function	
} plugin;


int register_plugin(server *srv, plugin *plg, apr_pool_t *mp);
void unregister_plugin(server *srv, char *name);
#endif
