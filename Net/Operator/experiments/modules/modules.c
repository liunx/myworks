/*
 * modules -- I want realise a struct to become a modules
 * program styles.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apr_general.h>
#include <apr_strings.h>
#include <apr_hash.h>

typedef enum {
	HELLO_EVENT,
	GOODBYE_EVENT,
} event_t;

struct proto_function {
	char name[30];
	event_t event;
	void (*hello)();
	int (*test)();
};

void hello()
{
	printf("Hello, Liunx.\n");
}

int test()
{
	printf("Hello, test.\n");
	return 0;
}

struct proto_function pfun = {
	.name	= "HELLO_EVENT",
	.event	= HELLO_EVENT,
	.hello	= hello,
	.test	= test,
};

void register_modules(struct proto_function *pfun, apr_hash_t *ht,
	     	 	      apr_pool_t *mp)
{
	apr_hash_set(ht, apr_pstrdup(mp, pfun->name), APR_HASH_KEY_STRING, pfun);
}

// In fact, the main is a register function
int main(int argc, const char *argv[])
{
	apr_status_t rv;
	apr_pool_t *mp;
	apr_hash_t *ht;
	struct proto_function *ppfun;

	apr_initialize();
	apr_pool_create(&mp, NULL);
	ht = apr_hash_make(mp);

	// now register the module
	register_modules(&pfun, ht, mp);

	if (pfun.event == HELLO_EVENT) {
		ppfun = apr_hash_get(ht, "HELLO_EVENT", APR_HASH_KEY_STRING);
		if (ppfun != NULL) {
			ppfun->hello();
		}
	}


	apr_pool_destroy(mp);
	apr_terminate();
	return 0;

}
