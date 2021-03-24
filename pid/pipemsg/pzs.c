#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
//#define FIFO1 "fifo1"
//#define FIFO2 "fifo2"
int main(){
    int pid,i,rfd,wfd,len=0,fd_in;
    pid=getpid();
    char str[32];
    int flag,stdinflag;
    fd_set write_fd,read_fd;
    struct timeval net_timer;
    mkfifo("fifo1",0666);
	mkfifo("fifo2",0666);
    wfd=open("fifo1",O_WRONLY);/*调用成功时返回一个文件描述符fd;调用失败时返回-1，并修改errno*/
	rfd=open("fifo2",O_RDONLY);
    if(rfd<=0||wfd<=0) return 0;
        printf("this is zs\n");
    while (1)
    {
        FD_ZERO(&read_fd);//将指定的文件描述符集清空
        FD_SET(rfd,&read_fd);//在文件描述符集合中增加一个新的文件描述符
        FD_SET(fileno(stdin),&read_fd);
        net_timer.tv_sec=5;
        net_timer.tv_usec=0;
        memset(str,0,sizeof(str));
        if(i=select(rfd+1,&read_fd,NULL,NULL,&net_timer)<=0)
        {/*负值：select错误;正值：某些文件可读写或出错;0：等待超时，没有可读写或错误的文件*/
            continue;
        }
        if(FD_ISSET(rfd,&read_fd))//测试指定的文件描述符是否在该集合中
        {
            printf("-----------------------------\n");
            read(rfd,str,sizeof(str));
            printf("ls:%s\n",str);
        }
        if(FD_ISSET(fileno(stdin),&read_fd))
        {
            printf("-----------------------------\n");
            fgets(str,sizeof(str),stdin);
            len=write(wfd,str,strlen(str));
        }
        if(!strncmp(str, "exit", 4))
        {
            break;
        }
    }
    close(rfd);
    close(wfd);
}