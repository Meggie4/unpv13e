#include	"unp.h"

int
main(int argc, char **argv)
{
	//socket是TCP套接字的花哨名字
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//通过htons函数将端口号13转换为特定格式
	servaddr.sin_port   = htons(13);	/* daytime server */
	//通过inet_pton将ip地址转换为特定格式
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	//连接
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	//读取时间
	//TCP是没有记录边界的字节流协议
	//因此，时间字节流可能会分多个TCP分节进行输出，当read函数返回０时，表示对端已经关闭了连接
	//当read函数返回负值时，表示发生了错误
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		//通过fputs函数将结果打印出来
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}
