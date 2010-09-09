/**
 * apr tutorial sample code
 * http://dev.ariel-networks.com/apr/
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>
#include <apr_poll.h>
#include <apr_hash.h>
#include "connection.h"
#include "parseconfig.h"

/**
 * do_listen -- create a listen socket
 */
apr_status_t do_listen(apr_pollset_t *pollset, apr_hash_t *ht, 
		       apr_pool_t *mp, const char *param)
{
	apr_status_t rv;
	apr_socket_t *s;
	apr_sockaddr_t *sa;
	char **ptr;
	int port;
	
	ptr = str_split_char(param, ":");
	if (ptr == NULL) {
		fprintf(stderr, "Get Null param!\n");
		return APR_EINVAL;
	}
	// then convert param
	port = atoi(ptr[1]);
	if (port < 0)
		return APR_EINVAL;

	rv = apr_sockaddr_info_get(&sa, ptr[0], APR_INET, port, 0, mp);
	if (rv != APR_SUCCESS) {
		return rv;
	}
	
	rv = apr_socket_create(&s, sa->family, SOCK_STREAM, APR_PROTO_TCP, mp);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	/* non-blocking socket */
	apr_socket_opt_set(s, APR_SO_NONBLOCK, 1);
	apr_socket_timeout_set(s, 0);
	apr_socket_opt_set(s, APR_SO_REUSEADDR, 1);/* this is useful for a 
						* server(socket listening) process */

	rv = apr_socket_bind(s, sa);
	if (rv != APR_SUCCESS) {
		return rv;
	}
	rv = apr_socket_listen(s, DEF_SOCKET_BACKLOG);
	if (rv != APR_SUCCESS) {
		return rv;
	}
	// Then.., add it to hash table, listen and connect socket 
	// against a different val, so we can distinguish in the 
	// pollset
	apr_hash_set(ht, s, APR_HASH_KEY_STRING, "S"); // server

	// Then.., if everything is OK, add it to the pollset
	apr_pollfd_t pfd = {mp, APR_POLL_SOCKET, APR_POLLIN, 0, {NULL}, NULL};
	pfd.desc.s = s;
	apr_pollset_add(pollset, &pfd);
	free(ptr);

	return APR_SUCCESS;
	
}

int do_accept(apr_pollset_t *pollset, apr_socket_t *lsock, apr_pool_t *mp)
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

		/**
	 * non-blocking socket. We can't expect that @ns 
	 * inherits non-blocking mode from @lsock 
	 */
		apr_socket_opt_set(ns, APR_SO_NONBLOCK, 1);
		apr_socket_timeout_set(ns, 0);
		
		apr_pollset_add(pollset, &pfd);
	}
	return TRUE;
}

/**
 * Connect to the remote host
 */
apr_status_t do_connect(apr_pollset_t *pollset, apr_hash_t *ht, 
			apr_pool_t *mp, const char *param)
{
	apr_sockaddr_t *sa;
	apr_socket_t *s;
	apr_status_t rv;
	char **ptr;
	int port;

	ptr = str_split_char(param, ":");
	if (ptr == NULL) {
		fprintf(stderr, "Get Null param!\n");
		return APR_EINVAL;
	}
	// then convert param
	port = atoi(ptr[1]);
	if (port < 0)
		return APR_EINVAL;

	rv = apr_sockaddr_info_get(&sa, ptr[0], APR_INET, port, 0, mp);
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

	// Then, add it to hash table. -- "C"lient
	apr_hash_set(ht, s, APR_HASH_KEY_STRING, "C");

	serv_ctx_t *serv_ctx = apr_palloc(mp, sizeof(serv_ctx_t));
	apr_pollfd_t pfd = { mp, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, serv_ctx };
	pfd.desc.s = s;

	/* at first, we expect requests, so we poll APR_POLLIN event */
	serv_ctx->status = SERV_RECV_REQUEST;
	serv_ctx->cb_func = recv_req_cb;
	serv_ctx->recv.is_firstline = TRUE;
	serv_ctx->mp = mp;
	apr_pollset_add(pollset, &pfd);

	free(ptr);
	return APR_SUCCESS;
}


int recv_req_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, 
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
int send_resp_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock)
{

	return TRUE;
}

