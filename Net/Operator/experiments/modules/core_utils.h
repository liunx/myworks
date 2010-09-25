#ifndef _CORE_UTILS_H_
#define _CORE_UTILS_H_
/*
 * The main structure for the main program, we can 
 * pass the pointer to sthe structure to mods.
 */
typedef struct {
	int mods_count;	// The number of mods
	void (*hello)(void);// the hello function
} core_t;


void core_init(core_t *core);
#endif
