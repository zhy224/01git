#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
int main()
{
	int pid=getpid();
	char buf[64];
	int i,rfd,wfd,len=0,fd_in,ret;
	char str[32];
	int flag,stdinflag;
	fd_set write_fd,read_fd;
	struct timeval net_timer;
	mkfifo(FIFO1,S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo(FIFO2,S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	ret=open(FIFO1,O_WRONLY|O_NONBLOCK);
	if(ret==-1){
		rfd = open(FIFO1,O_RDONLY|O_NONBLOCK);
		wfd = open(FIFO2,O_WRONLY);
	}
	else{
		wfd = open(FIFO1,O_WRONLY);
		rfd = open(FIFO2,O_RDONLY);
	}
	if(rfd<=0||wfd<=0) return 0;
		printf("this is client_%d\n--------------------------\n",pid);	
	while(1)
		{
			FD_ZERO(&read_fd);
			FD_SET(rfd,&read_fd);
			FD_SET(fileno(stdin),&read_fd);
			net_timer.tv_sec=5;
			net_timer.tv_usec=0;
			memset(str,0,sizeof(str));
			if(i=select(rfd+1, &read_fd,NULL, NULL, &net_timer) <= 0)
				continue;
			if(FD_ISSET(rfd,&read_fd))
			{
				read(rfd,str,sizeof(str));
				printf("\nreceive info:");
				printf("%s\n",str);
			}
			if(FD_ISSET(fileno(stdin),&read_fd))
			{
				//sprintf(buf,"%d",pid);
				printf("client_%d:",pid);
				fgets(str,sizeof(str),stdin);
				//strcat(buf,str);
				//len=write(wfd,buf,strlen(buf));
				len=write(wfd,str,strlen(str));
			}
			if(!strncmp(str,"exit",4))
			{
				break;
			}
		}
	close(rfd);
	close(wfd);
}
