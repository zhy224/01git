#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPEFILE "./fifo1"

int main()
{
	int fd;
	char * msg = "hello, friend!\n";
	printf("befor fifo open\n");
	if ((fd = open(PIPEFILE, O_WRONLY)) < 0 ){//以写的方式打开FIFO
			perror("fifo open failed!\n");
			exit(-1);
	}
	else {
			printf("fifo open sucessfully!\n");
	}
	printf("after fifo open\n");
	write(fd, msg, 10);//向FIFO写入数据
	close(fd);

	return 0;
}
