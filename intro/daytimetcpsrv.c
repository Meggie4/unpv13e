#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	//指定ip地址为INADDR_ANY表示服务器进程可以在任意网络接口上接受客户端的连接
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	//将端口绑定到相应的socket套接字上
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	//监听端口号，LISTENQ表示内核允许在这个监听描述符上排队的最大客户连接数目
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		//等待某个客户通过TCP三次握手连接成功，并返回已连接描述符
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
		//按照给定格式填充时间
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		//将时间通过socket发送到客户端
        Write(connfd, buff, strlen(buff));

		//关闭与客户端的连接
		Close(connfd);
	}
}
