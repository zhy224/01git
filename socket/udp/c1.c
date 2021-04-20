#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <error.h>
#define SERVPORT 10086
//#define SERADDE 192.168.1.173
#define MAXDATASIZE 30
int main(int argc,char *argv[])
{
    int sockfd,n;
    char buf[MAXDATASIZE];
    struct hostent *host;
    struct sockaddr_in serv_addr;
    if(argc<2)
    {
        fprintf(stderr,"please enter the server's hostname\n");
        exit(1);
    }
    if((host=gethostbyname(argv[1]))==NULL)
    {
        herror("gethostbyname error");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
    {
        perror("socket create error");
        exit(1);
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVPORT);
    //inet_pton(AF_INET,SERVPORT,&serv_addr.sin_addr);
    serv_addr.sin_addr=*((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero),8);
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1)
    {
        perror("socket 出错");
        exit(1);
    }
    while(1)
    {
        scanf("%s",buf);
        n=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        if(n==-1)
        {
            printf("send error");
        }
        if(!strncmp(buf, "exit", 4))
		{
			break;
		}
    }
    printf("收到:%s",buf);
    close(sockfd);
    exit(0);
}