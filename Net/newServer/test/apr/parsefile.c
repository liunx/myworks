/**
 * apr tutorial sample code
 * http://dev.ariel-networks.com/apr/
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_strings.h>

/* default buffer size */
#define BUFSIZE			4096


static apr_status_t parse_file(const char *config_fname, apr_pool_t *mp);

/**
 * file copy sample code
 * @remark Error checks omitted
 */
int main(int argc, const char *argv[])
{
    apr_status_t rv;
    apr_pool_t *mp;
        
    if (argc < 2) {
        printf("usage %s parse_config_file\n", argv[0]);
        return 0;
    }

    apr_initialize();
    apr_pool_create(&mp, NULL);

    rv = parse_file(argv[1], mp);
    if (rv != APR_SUCCESS) {
        goto error;
    }

    apr_pool_destroy(mp);
    apr_terminate();
    return 0;

 error:
    {
        char errbuf[256];
        apr_strerror(rv, errbuf, sizeof(errbuf));
        printf("error: %d, %s\n", rv, errbuf);
    }
    apr_terminate();
    return -1;
}

static apr_status_t parse_file(const char *config_fname, apr_pool_t *mp)
{
    apr_status_t rv;
    apr_file_t *config_fp = NULL;
    char *saveptr;
    char *token;
    int i;
        
    /* open the file. in other words, create apr_file_t object */
    /* @remark after apr-1.1.0, use APR_FOPEN_READ for third argument */
    if ((rv = apr_file_open(&config_fp, config_fname, APR_READ|APR_BINARY, 
	APR_OS_DEFAULT, mp)) != APR_SUCCESS) {
        goto done;
    }

       	char buf[BUFSIZE];
	char *ptr = NULL;
	char *str2 = NULL;
	char *saveptr2 = NULL;
	char *subtoken;
        apr_size_t len = sizeof(buf);

    while (1) {
        rv = apr_file_read(config_fp, buf, &len);
        if (rv != APR_SUCCESS) {
            break;
        }

	}
    for (i = 1, ptr = buf; ; i++, ptr = NULL) {
	    token = apr_strtok(ptr, "\n", &saveptr);
	    if (token == NULL)
		    break;
	    printf("%d: %s\n", i, token);
	    
	    for (str2 = token; ; str2 = NULL) {
		    subtoken = apr_strtok(str2, "=", &saveptr2);
		    if (subtoken == NULL)
			    break;
		    printf("-->%s\n", subtoken);
	    }
    }

 done:
    if (config_fp) {
        apr_file_close(config_fp);
    }
    return rv;
}
