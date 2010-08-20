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
#include <unistd.h>

#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>
#include <apr_poll.h>
#include <apr_hash.h>

/* default listen port number */
#define DEF_LISTEN_PORT		60002

/* default socket backlog number. SOMAXCONN is a system default value */
#define DEF_SOCKET_BACKLOG	SOMAXCONN

#define DEF_POLLSET_NUM		10000

/* default socket timeout */
#define DEF_POLL_TIMEOUT	(APR_USEC_PER_SEC * 30)

/* default socket timeout */
#define DEF_SOCK_TIMEOUT	(APR_USEC_PER_SEC * 30)

/* default buffer size */
#define BUFSIZE			4096

/* useful macro */
#define CRLF_STR		"\r\n"

typedef struct _serv_ctx_t serv_ctx_t;

/**
 * network event callback function type
 */
typedef int (*socket_callback_t)(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, 
				 apr_socket_t *sock);

/**
 * network server context 
 */
struct _serv_ctx_t {
    enum {
        SERV_RECV_REQUEST,
        SERV_SEND_RESPONSE,
    } status;

    socket_callback_t cb_func;
    apr_pool_t *mp;

    /* recv ctx */
    struct {
        const char *filepath;
        int is_firstline;
    } recv;

    /* send ctx */
    struct {
        char *send_buf;
        apr_off_t offset;
    } send;
};

static apr_socket_t* create_listen_sock(apr_pool_t *mp, const char *host, int port);
static int do_accept(apr_pollset_t *pollset, apr_socket_t *lsock, apr_pool_t *mp);

static apr_status_t do_connect(apr_pollset_t *pollset, apr_socket_t **sock, apr_pool_t *mp, 
			       const char *host, int port);

static int recv_req_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock);
static int send_resp_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock);


/**
 * @remark Error checks omitted
 */
int main(int argc, const char *argv[])
{
    apr_status_t rv;
    apr_pool_t *mp;
    // The hash table
    apr_hash_t *ht;

    apr_socket_t *lsock;/* listening socket */
    // add a second server 
    apr_socket_t *lsock2;

    /* Let's add the connect socket */
    apr_socket_t *csock;

    apr_pollset_t *pollset;
    apr_int32_t num;
    const apr_pollfd_t *ret_pfd;

    apr_initialize();
    apr_pool_create(&mp, NULL);
    // Create the hash table
    ht = apr_hash_make(mp);
    if (ht == NULL)
	    printf("Failed to create the hash table.\n");


    lsock = create_listen_sock(mp, "localhost", 60002);
    assert(lsock);
    // Then add the pointer to the hash table
    apr_hash_set(ht, lsock, APR_HASH_KEY_STRING, "S");
    lsock2 = create_listen_sock(mp, "localhost", 60001);
    assert(lsock2);
    apr_hash_set(ht, lsock2, APR_HASH_KEY_STRING, "S");
    //char *val = apr_hash_get(ht, lsock, APR_HASH_KEY_STRING);
    //printf("val for lsock is %s\n", val);

    /*
     * Here, we create the epoll util
     */
    apr_pollset_create(&pollset, DEF_POLLSET_NUM, mp, 0);
    {
        apr_pollfd_t pfd = { mp, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, NULL };
	/*
	 * Add the listener socket
	 */
        pfd.desc.s = lsock;
        apr_pollset_add(pollset, &pfd);
	// the second server
        pfd.desc.s = lsock2;
        apr_pollset_add(pollset, &pfd);
	// the connect socket
	//pfd.desc.s = csock;
        //apr_pollset_add(pollset, &pfd);
	// for connection socket
    	// do_connect will add the socket to the pollset
    	rv = do_connect(pollset, &csock, mp, "localhost", 60000);
    	if (rv != APR_SUCCESS) {
	    printf("Failed to connect to the remote server.\n");
	    exit(1);
    	}
    	// add the sock in the hash table with a value of "C"
    	apr_hash_set(ht, csock, APR_HASH_KEY_STRING, "C");

    }

    // we use this pointer to be return value
    char *val = NULL;
    apr_socket_t *hsock = NULL;
    //char buf[1024];
    //apr_size_t len;
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

static apr_socket_t* create_listen_sock(apr_pool_t *mp, const char *host, int port)
{
    apr_status_t rv;
    apr_socket_t *s;
    apr_sockaddr_t *sa;
    
    rv = apr_sockaddr_info_get(&sa, host, APR_INET, port, 0, mp);
    if (rv != APR_SUCCESS) {
	goto error;
    }
    
    rv = apr_socket_create(&s, sa->family, SOCK_STREAM, APR_PROTO_TCP, mp);
    if (rv != APR_SUCCESS) {
	goto error;
    }

    /* non-blocking socket */
    apr_socket_opt_set(s, APR_SO_NONBLOCK, 1);
    apr_socket_timeout_set(s, 0);
    apr_socket_opt_set(s, APR_SO_REUSEADDR, 1);/* this is useful for a 
						* server(socket listening) process */

    rv = apr_socket_bind(s, sa);
    if (rv != APR_SUCCESS) {
	goto error;
    }
    rv = apr_socket_listen(s, DEF_SOCKET_BACKLOG);
    if (rv != APR_SUCCESS) {
	goto error;
    }

    return s;

 error:
    return NULL;
}

static int do_accept(apr_pollset_t *pollset, apr_socket_t *lsock, apr_pool_t *mp)
{
    apr_socket_t *ns;/* accepted socket */
    apr_status_t rv;
            
    rv = apr_socket_accept(&ns, lsock, mp);
    if (rv == APR_SUCCESS) {
        serv_ctx_t *serv_ctx = apr_palloc(mp, sizeof(serv_ctx_t));
        apr_pollfd_t pfd = { mp, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, serv_ctx };
        pfd.desc.s = ns;
        /* at first, we expect requests, so we poll APR_POLLIN event */
        serv_ctx->status = SERV_RECV_REQUEST;
        serv_ctx->cb_func = recv_req_cb;
        serv_ctx->recv.is_firstline = TRUE;
        serv_ctx->mp = mp;

        /* non-blocking socket. We can't expect that @ns inherits non-blocking mode from @lsock */
        apr_socket_opt_set(ns, APR_SO_NONBLOCK, 1);
        apr_socket_timeout_set(ns, 0);
        
        apr_pollset_add(pollset, &pfd);
    }
    return TRUE;
}

/**
 * Connect to the remote host
 */
static apr_status_t do_connect(apr_pollset_t *pollset, apr_socket_t **sock, apr_pool_t *mp, 
			       const char *host, int port)
{
    apr_sockaddr_t *sa;
    apr_socket_t *s;
    apr_status_t rv;
    
    rv = apr_sockaddr_info_get(&sa, host, APR_INET, port, 0, mp);
    if (rv != APR_SUCCESS) {
	return rv;
    }
    
    rv = apr_socket_create(&s, sa->family, SOCK_STREAM, APR_PROTO_TCP, mp);
    if (rv != APR_SUCCESS) {
	return rv;
    }

    /* it is a good idea to specify socket options explicitly.
     * in this case, we make a blocking socket with timeout. */
    apr_socket_opt_set(s, APR_SO_NONBLOCK, 1);
    apr_socket_timeout_set(s, DEF_SOCK_TIMEOUT);

    rv = apr_socket_connect(s, sa);
    if (rv != APR_SUCCESS) {
		printf("Connecting timeout!\n");
        	return rv;
    }

    /* see the tutorial about the reason why we have to specify options again */
    apr_socket_opt_set(s, APR_SO_NONBLOCK, 1);
    apr_socket_timeout_set(s, 0); // It'll never block 

        serv_ctx_t *serv_ctx = apr_palloc(mp, sizeof(serv_ctx_t));
        apr_pollfd_t pfd = { mp, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, serv_ctx };
        pfd.desc.s = s;

       	/* at first, we expect requests, so we poll APR_POLLIN event */
       	serv_ctx->status = SERV_RECV_REQUEST;
        serv_ctx->cb_func = recv_req_cb;
        serv_ctx->recv.is_firstline = TRUE;
        serv_ctx->mp = mp;
        apr_pollset_add(pollset, &pfd);

    *sock = s;
    return APR_SUCCESS;
}


static int recv_req_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, 
		       apr_socket_t *sock)
{
    while (1) {
	char buf[BUFSIZE];
	apr_size_t len = sizeof(buf) - 1;/* -1 for a null-terminated */

	apr_status_t rv = apr_socket_recv(sock, buf, &len);
	if (rv == APR_EOF) {
		// We do not need to remove the fd from pollset
		// when we close(fd), it will auto remove it from
		// pollset
            	//apr_pollset_remove(pollset, &pfd);
		printf("Disconnected from remote client...\n");
            	apr_socket_close(sock);
	    	break;
        }

	// if len == 0, means we complete the data reciving
	if (len == 0) {
		printf("Complete the recieving...\n");
		break;
	}
        buf[len] = '\0';/* apr_socket_recv() doesn't return a null-terminated string */

	// Just send what we got
	apr_socket_send(sock, buf, &len);
	// We do not need this break, because if the recive not 
	// complete, we must loop back to recive again until finish
	// the reciving

	//break;
	printf("Get a data...\n");

        
    }
	//apr_socket_send(sock, buf, &len);

    return TRUE;
}


/**
 * Send a response to the client.
 */
static int send_resp_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock)
{

    return TRUE;
}

