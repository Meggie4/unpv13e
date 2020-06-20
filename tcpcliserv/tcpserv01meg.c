#include "unp.h"

int main(int argc, char** argv) {
    int listenfd, connfd;
    pid_t child_pid;
    struct sockaddr_in server_addr, cli_addr;
    socklen_t cli_len;
    char buff[MAXLINE];
    //创建socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);
    //绑定ip和端口号
    Bind(listenfd, (SA*)&server_addr, sizeof(server_addr));
    //监听socket，最大待连接客户数目
    Listen(listenfd, LISTENQ);
    //并行处理socket连接
    for(;;) {
        cli_len = sizeof(cli_addr);
        connfd = Accept(listenfd, (SA*)&cli_addr, &cli_len);
        if((child_pid = Fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            Close(connfd);
            exit(0);
        }
        close(connfd);
    }
    //读入网络输入通过socket回射
    //继续监听
}