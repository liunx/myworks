#ifndef _SERVER_H_
#define _SERVER_H_
/*
 * The server struct
 */
typedef struct {
	size_t version;	// the server version
	void *plugin_slots;	// we use it to store plugins
} server;

#endif
