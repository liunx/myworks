#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>
#include <apr_poll.h>
#include <apr_hash.h>
#include "connection.h"
#include "parseconfig.h"

#define FILENAME	"test.conf"

static apr_hash_t *ht;
// The hash to store configure file keyword
static apr_hash_t *conf_ht;
static apr_pollset_t *pollset;


/*
 * Init the hash table, fill it with configure keywords
 * we use table to parse the configure file to know what 
 * we will do.
 */
static void init_hashtab(apr_hash_t *ht, apr_pool_t *mp)
{
	apr_hash_set(ht, apr_pstrdup(mp, "Server"), APR_HASH_KEY_STRING,
		     do_listen);
	apr_hash_set(ht, apr_pstrdup(mp, "Client"), APR_HASH_KEY_STRING,
		     do_connect);

}
/*
 * load_setting
 */
static void load_setting(apr_hash_t *conf_ht, apr_pool_t *mp, char *str)
{
	char **ptr = NULL;
	int retval;
	// get the hash val and the -val- store the respect function
	// pointer
	void *val = NULL;

	// We get the input string in a syle of  x = xxx,
	// we need split them out
	ptr = str_split_char(str, "=");
	if (ptr == NULL) {
		fprintf(stderr, "Failed to parse the line.\n");
		exit(1);
	}
	// Get the key 
	val = apr_hash_get(conf_ht, ptr[0], APR_HASH_KEY_STRING);
	if (val) {
		hash_callback_t cb_func = (hash_callback_t) val;
		cb_func(pollset, ht, mp, ptr[1]);
	}

	free(ptr);

	return;
}


/*
 * do_init -- setup the server from configure file
 * If something wrong, it will break
 */

static void do_init(apr_hash_t *conf_ht, apr_pool_t *mp, char *buf)
{
	char *str1, *str2, *token;
	char *saveptr1;
	// We need the i to keep loop
	for (str1 = buf; ; str1 = NULL) {
		token = strtok_r(str1, "\n", &saveptr1);
		if (token == NULL)
			break;
		// Now, we get a line, then let's process it with
		// funcions in getword
		str1 = clean_comment(token);
		str2 = clean_space(str1);
		if (str2[0] != '\0') {
                        //printf("The newline is %s\n", str2);
                        load_setting(conf_ht, mp, str2);
		}
		free(str2);
		free(str1);
	}

}


/**
 * @remark Error checks omitted
 */
int main(int argc, const char *argv[])
{
	apr_status_t rv;
	apr_pool_t *mp;
	// The hash table we use it to record the 
	// socket

	apr_socket_t *lsock;/* listening socket */
	// add a second server 
	apr_socket_t *lsock2;

	/* Let's add the connect socket */
	apr_socket_t *csock;

	apr_int32_t num;
	const apr_pollfd_t *ret_pfd;

	// Pointer to the file
	char *file;

	apr_initialize();
	apr_pool_create(&mp, NULL);
	// Create the hash table
	ht = apr_hash_make(mp);
	if (ht == NULL) {
		printf("Failed to create the hash table.\n");
		exit(1);
	}
	conf_ht = apr_hash_make(mp);
	if (conf_ht == NULL) {
		printf("Failed to create the hash table.\n");
		exit(1);
	}
	init_hashtab(conf_ht, mp);

	file = read_file(FILENAME);
	do_init(conf_ht, mp, file);

	// we use this pointer to be return value
	char *val = NULL;
	apr_socket_t *hsock = NULL;
	while (1) {
		rv = apr_pollset_poll(pollset, DEF_POLL_TIMEOUT, &num, &ret_pfd);
		if (rv == APR_SUCCESS) {
			int i;
			assert(num > 0);
			/* scan the active sockets */
			for (i = 0; i < num; i++) {
		    /*
		     * With hash table, we can manage many listen socket
		     */
		    hsock = ret_pfd[i].desc.s;
		    val = apr_hash_get(ht, hsock, APR_HASH_KEY_STRING);
		    // if NULL, the sock does not exist in the table
		    if (val) {
			    if (!apr_strnatcmp(val, "S")) {
	            		/* the listen socket is readable. that indicates 
		     		 * we accepted a new connection 
		     		 */
                    		do_accept(pollset, hsock, mp);
			    } else if (!apr_strnatcmp(val, "C")) {
				printf("Get response from remote server!\n");
                    		serv_ctx_t *serv_ctx = ret_pfd[i].client_data;
                    		socket_callback_t cb_func = serv_ctx->cb_func;
                    		cb_func(serv_ctx, pollset, ret_pfd[i].desc.s);

				//len = sizeof(buf) - 1;
				//apr_socket_recv(hsock, buf, &len);
				//printf("The data is : %s\n", buf);

			    }

		    } else {
                    serv_ctx_t *serv_ctx = ret_pfd[i].client_data;
                    socket_callback_t cb_func = serv_ctx->cb_func;

                    cb_func(serv_ctx, pollset, ret_pfd[i].desc.s);
                }
            }
        }
    }
    
    return 0;
}


