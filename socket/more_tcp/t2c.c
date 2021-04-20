#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc,char *argv[]){
	if(argc != 3){
		fprintf(stderr,"use: %s <ip> [port]\n",argv[0]);	
		return -1;
	}
	const char *ip = argv[1];
	unsigned short int port = atoi(argv[2]);

	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1){
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	socklen_t addrlen = sizeof(addr);

	int ret = connect(sfd,(const struct sockaddr*)(&addr),addrlen);
	if(ret == -1){
		perror("connect");
		return -1;
	}
	printf("连接服务器成功!\n");
	pid_t pid = fork();
	if(pid == -1){
		perror("fork");	
		return -1;
	}
	if(pid == 0){
		while(1){
			char buf[1024]={};
			gets(buf);
			if(send(sfd,buf,strlen(buf)+1,0)<=0){
				break;	
			}
		}
	}else{
		while(1){
			char buf[1024]={};
			if(recv(sfd,buf,1024,0)<=0){
				break;
			}
			time_t timep;
			time(&timep);
			printf("%s\n",ctime(&timep));
			printf("%s\n",buf);
		}
	}
	close(sfd);
	return 0;	
}
