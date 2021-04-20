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

#define BACKLOG 10
#define MAX 50

void* pthread_run(void* arg);
typedef struct sockmy_addr* sockmy_addrp;
int sockfd,clientfd,ret,sin_size,i,j,recvbt;
int client_p[MAX];

int main(int argc,char* argv[])
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in my_addr,client_addr;
    my_addr.sin_family=AF_INET;
	my_addr.sin_port = htons(atoi(argv[2]));
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ret = bind(sockfd,(sockmy_addrp)&my_addr,sizeof(my_addr));
    if(ret < 0)
	{
		perror("bind");
		exit(1);
	}
    if(listen(sockfd,BACKLOG)==-1)
    {
        perror("listen");
        exit(1);
    }
    sin_size=sizeof(struct sockaddr_in);
    while (1)
    {
        if((clientfd=accept(sockfd,(sockmy_addrp)&client_addr,&sin_size))==-1)
        {
            perror("listen error");
            exit(1);
        }
        for (i = 0;i < 50;i++)
        {
            if (client_p[i] == 0)
			{
                client_p[i] = clientfd;
                printf("get a new client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                pthread_t ptid;
                pthread_create(&ptid,NULL,pthread_run,&client_p[i]);
                break;
            }
			if (50 == i)
			{
				char* str = "server full,please quit\n";
				send(clientfd,str,strlen(str),0);
				close(clientfd);
			}
        }
    }  
}
void* pthread_run(void* arg)
{
	int index=*(int*)arg;
	while(1)
	{			
		char buf1[1024];
        char buf2[1204];
        recvbt=recv(clientfd,buf1,strlen(buf1),0);
        buf1[recvbt]='\0';
        printf("from %d:%s\n",index,buf1);
        
        buf2[strlen(buf2)-1] = '\0';
        if(strncmp("quit",buf2,4) == 0)
        {
            close(clientfd);
            pthread_exit((void*)i);
            break;
        }
        send(clientfd, buf2, strlen(buf2), 0);
		for (j = 0;j < 50;j++)
		{
			if (client_p[j] != 0)
			{
				send(client_p[i],buf2,strlen(buf2),0);
			}
		}
        if(recv(index,buf1,sizeof(buf1),0) < 0)
		{	
			int i;
			for(i=0;i<50;i++)
			{
				if(client_p[i] == index)
				{
				client_p[i] = 0;
				break;
				}	
			}
			pthread_exit((void*)i);
		}
		
	}		
}