#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct sockaddr* sockaddrp;
void* pthread_run(void* arg);
char name[30];
int sockfd;

void* pthread_run(void* arg)
{
    while(1)
    {
		char buf[255]={};
		recv(sockfd,buf,sizeof(buf),0);
        printf("%s\n",buf);
    }
}

void sigint(int signum)
{
	return ;
}

int main(int argc,char* argv[])
{
	signal(SIGINT,sigint);
	//建立socket对象
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}
	//准备通信地址
	struct sockaddr_in addr = {AF_INET};
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = inet_addr(argv[2]);
	//等待连接
	connect(sockfd,(sockaddrp)&addr,sizeof(addr));
	system("clear");
    printf("请输入您的昵称:");
    scanf("%s",name);
    pthread_t ptid;
	pthread_create(&ptid,NULL,pthread_run,NULL);
    char buf[255] = {};
    sprintf(buf,"%s 进入了悟空聊天室\n",name);
    send(sockfd,buf,strlen(buf),0);
    while(1)
		{
        char msg1[255] = {};
        scanf("%s",msg1);
        char msg2[255] = {};
        sprintf(msg2,"%s:%s",name,msg1);
        send(sockfd,msg2,strlen(msg2),0);
        if (strcmp(msg1,"q") == 0)
		{
            memset(msg1,0,sizeof(msg1));
            sprintf(buf,"%s退出了悟空聊天室\n",name);
            send(sockfd,buf,strlen(buf),0);
            break;
        }
    }
    close(sockfd);
    return 0;
}