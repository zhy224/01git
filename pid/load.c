#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	printf("i am load:%d\n",getpid());
	pid=fork();
	if(pid==0)
	{
		execl("/home/zy/prog/pid/proc","proc",NULL);
	}else
	{
		printf("i am load.i finished\n");
		wait(NULL);
	}
	return 0;
}
