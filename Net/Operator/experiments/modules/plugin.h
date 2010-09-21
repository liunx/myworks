#ifndef _PLUGIN_H_
#define _PLUGIN_H_

/*
 * plugin -- the core plugin struct
 */
typedef struct {
	size_t version; // plugin version
	char *name;	// plugin name
	void *(*init)(void);	// the plugin main init 
	void *(*handler)(void);	// the handler function	
} plugin;

#endif
