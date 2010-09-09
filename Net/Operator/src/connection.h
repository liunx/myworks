#ifndef CONNECTION_H
#define CONNECTION_H
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

apr_status_t do_listen(apr_pollset_t *pollset, apr_hash_t *ht, apr_pool_t *mp, 
		       const char *param);
int do_accept(apr_pollset_t *pollset, apr_socket_t *lsock, apr_pool_t *mp);

apr_status_t do_connect(apr_pollset_t *pollset, apr_hash_t *ht, apr_pool_t *mp, 
			const char *param);

int recv_req_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock);
int send_resp_cb(serv_ctx_t *serv_ctx, apr_pollset_t *pollset, apr_socket_t *sock);

#endif
