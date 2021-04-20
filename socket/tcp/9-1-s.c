#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define SERVPORT 12306
#define BACKLOG 30
int main()
{
    int sockfd,client_fd;
    char buf[BACKLOG];
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    int sin_size;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket 创建失败");
        exit(1);
    }
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(SERVPORT);
    my_addr.sin_addr.s_addr=INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind 出错");
        exit(1);
    }
    if(listen(sockfd,BACKLOG)==-1)
    {
        perror("listen 出错");
        exit(1);
    }
    while (1)
    {
        sin_size=sizeof(struct sockaddr_in);
        if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size))==-1)
        {
            perror("accept error");
            continue;
        }
        printf("收到一个连接来自:%s\n",inet_ntoa(remote_addr.sin_addr));
        if(!fork())/*fork返回值为零则为子进程，按位取反为true*/
        {/*子进程*/
            if(send(client_fd," I am server \n",26,0)==-1)
            perror("send 出错");
            /*---------------------*/
            recv(client_fd,buf,BACKLOG,0);
            printf("收到:%s",buf);
            /*---------------------*/
            close(client_fd);
            exit(0);
        }
        close(client_fd);
    }
}
