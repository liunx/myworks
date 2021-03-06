#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using namespace std;

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 60001
#define CONN_PORT 60002
#define INFTIM 1000

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }   
}

int main()
{
    int i, maxi, listenfd, connfd, sockfd,epfd,nfds;
    int serverfd;
    int retval;
    int flags = 1;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen = sizeof(socklen_t);
    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    struct epoll_event ev,events[20];
    //生成用于处理accept的epoll专用的文件描述符
    epfd=epoll_create(256);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    // The client struct
    struct hostent *Ser_hosten;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("The listenfd == %d\n", listenfd);
    if (listenfd < 0)
	    exit(-1);
    retval = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int));
    if (retval < 0) {
	    printf("Failed to setsockopt !\n");
	    exit(1);
    }

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("The serverfd == %d\n", serverfd);
    if (serverfd < 0) {
	    printf("创建连接socket失败\n");
	    exit(-1);
    }
    retval = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int));
    if (retval < 0) {
	    printf("Failed to setsockopt !\n");
	    exit(1);
    }

    //把socket设置为非阻塞方式
    setnonblocking(listenfd);
    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    //ev.events=EPOLLIN|EPOLLET;
    ev.events=EPOLLIN;
    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    ev.data.fd = serverfd;
    ev.events=EPOLLIN;

    epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&ev);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr));//htons(SERV_PORT);
    serveraddr.sin_port=htons(SERV_PORT);
    retval = bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval < 0) {
	    printf("端口已被占用\n");
	    exit(-1);
    }
    // Now, let's add the client socket 
    Ser_hosten = gethostbyname("localhost");
    if (Ser_hosten == NULL) {
	    perror("The hostname is not exist.\n");
	    exit(1);
    }

    serveraddr.sin_family = AF_INET;
    bcopy((char *)Ser_hosten->h_addr, (char *)&serveraddr.sin_addr.s_addr,
		    Ser_hosten->h_length);
    serveraddr.sin_port=htons(CONN_PORT);

    retval = connect(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval < 0) {
	    printf("不能连接...\n");
	    exit(-1);
    }
    listen(listenfd, LISTENQ);
    for ( ; ; ) {
        //等待epoll事件的发生
        nfds=epoll_wait(epfd,events,20,-1); // it just return the amount of events produced,
	// the negtive 1 is better for performace
	// so if nfds == 0, means that thereis no events to handle
        //处理所发生的所有事件     
        for(i=0;i<nfds;++i)
        {
		printf("The fd == %d\n", events[i].data.fd);
            if(events[i].data.fd==listenfd)
            {
                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
		printf("Accept the external event.\n");
                if(connfd<0){
                    perror("connfd<0");
                    //exit(1);
                }
                //setnonblocking(connfd);
                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << endl;
                //设置用于读操作的文件描述符
                ev.data.fd=connfd;
                //设置用于注测的读操作事件
                //ev.events=EPOLLIN|EPOLLET;
                ev.events=EPOLLIN;
                //注册ev
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            } else if (events[i].data.fd == serverfd) {
		    printf("Get data from remote server...\n");
                if ( (sockfd = events[i].data.fd) < 0) 
                    continue;
                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
			serverfd = -1;
                    } else
                        std::cout<<"readline error"<<std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
		    serverfd = -1;
                }

	    }
            else if(events[i].events&EPOLLIN)
            {
		    printf("Get the epoll in event!\n");
                cout << "EPOLLIN" << endl;
                if ( (sockfd = events[i].data.fd) < 0) 
                    continue;
                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else
                        std::cout<<"readline error"<<std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';
                cout << "read " << line << endl;

		// let's resend the data to another clients
		// It shows that we can resend to another cients
		// without turn on poll out
		write(serverfd, line, sizeof(line));
                //设置用于写操作的文件描述符
                ev.data.fd=sockfd;
                //设置用于注测的写操作事件
                ev.events=EPOLLOUT;
                //修改sockfd上要处理的事件为EPOLLOUT
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);

		// Then let's modify it back
		// After the poll in condition process finished, 
		// then epoll process the poll out event

		//ev.events = EPOLLIN;
		//epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
		//write(sockfd, "Hello, Liunx", 13);
            }
            else if(events[i].events&EPOLLOUT)
            {   
		    printf("Get the epoll out event!\n");
                sockfd = events[i].data.fd;
                //write(sockfd, line, n);
                //设置用于读操作的文件描述符
                ev.data.fd=sockfd;
                //设置用于注测的读操作事件
                ev.events=EPOLLIN;
                //修改sockfd上要处理的事件为EPOLIN
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
            }
        }
    }
    return 0;
}

