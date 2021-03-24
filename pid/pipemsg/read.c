#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main()
{
	int ret,fd;
	char buf[100] = {0};
	unlink("fifo.tmp");
	ret = mkfifo("fifo.tmp",0666);     //创建有名管道
	if (-1 == ret)      //创建失败
	{
		perror("fifo");
		exit(1);
	}
	fd = open("fifo.tmp", O_RDONLY);        //以只读方式打开
	if(-1 == fd)        //打开失败
	{
		perror("read");
		exit(1);
	}
	while(1)
	{
		ret = read(fd, buf, sizeof(buf));       //读出缓冲区中的内容
		if(ret == -1)
		{
			perror("read");
			exit(1);
		}
		if(!strncmp(buf, "bye", 3))     //以bye结束
		{
			break;      //跳出循环
		}
		printf("read from fifo.tmp: %s\n",buf);
		memset(buf, 0, sizeof(buf));    //清空缓冲区
		unlink("fifo.tmp"); //关掉管道文件，方便程序下次运行
	}
	return 0;
}
