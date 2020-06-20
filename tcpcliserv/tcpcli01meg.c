#include "unp.h"

int main(int args, char** argv) {
    //创建套接字
    int connfd;
    struct sockaddr_in server_addr;
    char buff[MAXLINE], recv[MAXLINE];
    if(args < 2) {
        fprintf(stderr, "to enter ip address of the server!\n");
        exit(0);
    }
    //指定服务器地址
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    //连接服务器
    connfd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(connfd, (SA*)&server_addr, sizeof(server_addr));
    //从终端读取字符串
    printf("to enter message to retract.....\n");
    while(Fgets(buff, MAXLINE, stdin) != NULL) {
        Writen(connfd, buff, strlen(buff));
        if(Readline(connfd, recv, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");
        Fputs(recv, stdout);
        printf("to enter message to retract.....\n");
    }
    //通过套接字发送到服务器
}