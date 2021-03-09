#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t i,j;
	printf("i am parent pid:%d\n",getpid());
	i=fork();
	if(i!=0)
	{
		printf("i am parent:%d my parent:%d\n",getpid(),getppid());
		/*for(j=0;j<20;j++)
		{
			printf("*");
			fflush(stdout);
			sleep(1);
		}*/
		wait(NULL);
	}
	else
	{
		printf("i am child:%d my parent:%d\n",getpid(),getppid());
		for(j=0;j<20;j++)
		{
			printf(".");
			fflush(stdout);
			sleep(1);
		}
	}
	return 0;
}
