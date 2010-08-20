#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 100

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE + 1];
	time_t ticks;
	unsigned short port;
	int flag = 1, len = sizeof(int);

	port = 60000;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len)
			== -1) {
		perror("setsockopt");
		exit(1);
	}
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))
			== -1) {
		peror("bind");
		exit(1);
	} else 
		printf("bind call OK!\n");
	if (listen(listenfd, 5) == -1) {
		perror("listen");
		exit(1);
	}

	for (;;) {
		if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL))
				== -1) {
			perror("accept");
			exit(1);
		}
		if (fork() == 0) {
			close(listenfd);
			ticks = time(NULL);
			snprintf(buff, 100, "%.24s\r\n", ctime(&ticks));
			write(connfd, buff, strlen(buff));
			close(connfd);
			sleep(1);
			execlp("f1-9d", NULL);
			perror("execlp");
			exit(1);
		}
		close(connfd);
		exit(0);
	}
}
