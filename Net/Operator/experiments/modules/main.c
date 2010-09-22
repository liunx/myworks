/*
 * modules -- I want realise a struct to become a modules
 * program styles.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apr_general.h>
#include <apr_strings.h>
#include <apr_tables.h>
#include <apr_dso.h>
#include "server.h"
#include "plugin.h"


#define ARRAY_INIT_SZ		32

void init(apr_array_header_t *arr, apr_pool_t *mp)
{

}

/**
 * the Main
 */
int main(int argc, const char *argv[])
{
	apr_status_t rv;
	apr_pool_t *mp;
	// The global struct for plugins
	server *srv = NULL;
	apr_dso_handle_t *dso_h;
	typedef void (*plugin_init_t)(plugin *plg);
	plugin_init_t plugin_init;
	/*
	 * process the param
	 */
	if (argc < 2) {
		printf("sample: %s xxx.so \n", argv[0]);
		exit(1);
	}

	apr_initialize();
	apr_pool_create(&mp, NULL);

	/*
	 * Allocate memory for server struct
	 */
	srv = (server *)malloc(sizeof(server));
	if (srv == NULL) {
		fprintf(stderr, "memory allocation failed!\n");
		exit(1);
	}
	srv->version = 0x0010;
	// apr_array_header_t *slots
	srv->slots = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(void *));
	/*
	 * Now, init the server struct for plugins
	 */
	/* load share libary we want */
	if ((rv = apr_dso_load(&dso_h, argv[1], mp)) != APR_SUCCESS) {
		fprintf(stderr, "Failed load the share lib.\n");
		goto error;
	}
	/* seek the plugin init function */
	if ((rv = apr_dso_sym((apr_dso_handle_sym_t *)&plugin_init, dso_h,
	    "plugin_init")) != APR_SUCCESS) {
		fprintf(stderr, "Can not find the init function.\n");
		goto error;
	}
	/* Get the plugin */
	plugin *plg = (plugin *)malloc(sizeof(plugin));
	plugin_init(plg);
	/* Ok, We get the internals of the plugin */
	register_plugin(srv, plg, mp);

error:
	apr_dso_unload(dso_h);
	apr_pool_destroy(mp);
	apr_terminate();
	return 0;

}
