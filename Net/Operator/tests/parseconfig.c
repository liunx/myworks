/*
 * parseconfig
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_file_info.h>
#include <apr_strings.h>

/* default buffer size */
#define BUFSIZE		4096


int main(int argc, const char *argv[])
{
	apr_status_t rv;
	apr_pool_t *mp;
	char buf[] = "Hello, liunx    this is !";
	char *str = NULL;

	// The -must- two steps
	apr_initialize();
	apr_pool_create(&mp, NULL);
	str = apr_collapse_spaces(buf, buf);
	printf("The string is %s\n", buf);

	apr_pool_destroy(mp);
	apr_terminate();

	return 0;
}


