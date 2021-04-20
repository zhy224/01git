#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>

#define _BACKLOG_ 10

void *fun(void* arg);

void *fun(void* arg)
{
    int client_sock = *(int*)arg;
    while(1)
    {  
        char buf[1024];
        memset(buf,'\0',sizeof(buf));
        read(client_sock,buf,sizeof(buf));
        printf("client %d:%s\n",client_sock,buf);
        printf("server:");
        memset(buf,'\0',sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1]='\0';
        if(strncasecmp(buf,"quit",4)==0)
        {
            printf("quit\n");
            break;
        }
        write(client_sock,buf,strlen(buf)+1);
        printf("wait...\n");
    }
    close(client_sock);
}
int main(int argc,char* argv[])
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        printf("socket()\n");
    }
    struct sockaddr_in server_socket;
    struct sockaddr_in socket;
    pthread_t thread_id;  
    bzero(&server_socket,sizeof(server_socket));
    server_socket.sin_family=AF_INET;
    // server_socket.sin_port=htons(_PORT_);
    server_socket.sin_port = htons(atoi(argv[2]));
    // server_socket.sin_addr.s_addr=htonl(INADDR_ANY);
    // server_socket.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_socket.sin_addr.s_addr = inet_addr(argv[1]);
    if(bind(sock,(struct sockaddr*)&server_socket,sizeof(struct sockaddr_in))<0)
    {
        printf("bind()\n");
        close(sock);
        return 1;
    }
    if(listen(sock,_BACKLOG_)<0)
    {
        printf("listen()\n");
        close(sock);
        return 2;
    }
    printf("success\n");
    for(;;)
    {
        socklen_t len=0;
        int client_sock=accept(sock,(struct sockaddr*)&socket,&len);
        if(client_sock<0)
        {
            printf("accept()\n");
            return 3;
        }
        printf("get a new client %s:%d\n", inet_ntoa(socket.sin_addr), ntohs(socket.sin_port));
        pthread_create(&thread_id, NULL, fun,&client_sock);  
        pthread_detach(thread_id); 
    }
    close(sock);
    return 0;
}
