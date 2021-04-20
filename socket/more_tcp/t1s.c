#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct sockaddr* sockaddrp;
void* pthread_run(void* arg);
int clifd_p[50];
int sockfd;

void* pthread_run(void* arg)
{
	int index=*(int*)arg;
	while(1)
	{			
		char buf1[50] = {};	
		int j;
		for (j = 0;j < 50;j++)
		{
			if (clifd_p[j] != 0)
			{
				send(clifd_p[j],buf1,strlen(buf1),0);
			}
		}
		if(recv(index,buf1,sizeof(buf1),0) < 0)
		{	
			int i;
			for(i=0;i<50;i++)
			{
				if(clifd_p[i] == index)
				{
				clifd_p[i] = 0;
				break;
				}	
			}
			pthread_exit((void*)i);
		}
	}		
}

int main(int argc,char* argv[])
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr = {AF_INET};
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = inet_addr(argv[2]);
	int ret = bind(sockfd,(sockaddrp)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}
	listen(sockfd,1024);
	struct sockaddr_in src_addr={};
	socklen_t addr_len=sizeof(src_addr);	
    while(1)
    {
		struct sockaddr_in src_addr={};
		socklen_t addr_len=sizeof(src_addr);
        printf("欢迎进入悟空聊天室！");
        int clifd = accept(sockfd,(sockaddrp)&src_addr,&addr_len);
        if (clifd < 0)
		{
            printf("客户端繁忙，请稍后再试...\n");
            return -1;
        }
        int i;
        for (i = 0;i < 50;i++)
        {
            if (clifd_p[i] == 0)
			{
                clifd_p[i] = clifd;
                printf("帐号 = %d\n",clifd);
                pthread_t ptid;
                pthread_create(&ptid,0,pthread_run,&clifd);
                break;
            }
			if (50 == i)
			{
				char* str = "聊天室已满，请加入其他聊天室，或者等待有人离开!";
				send(clifd,str,strlen(str),0);
				close(clifd);
			}
        }
    }
}
