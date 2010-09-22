#ifndef _SERVER_H_
#define _SERVER_H_
#include <apr_general.h>
#include <apr_tables.h>
/*
 * The server struct
 */
typedef struct {
	size_t version;	// the server version
	apr_array_header_t *slots; // a dynamic array for plugins
} server;

#endif
