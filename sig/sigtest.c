#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

void fun_cz();
int main()
{
    pid_t pid;
    int ret;
    pid=fork();
    if(pid<0)
    {
        perror("创建子进程失败\n");
    }
    else if(pid==0){
        int counter;
        while (1)
        {
            counter++;
            //(void) signal (SIGTSTP,fun_cz);
            printf("running %ds\n",counter);
            sleep(1);
        }
    }
    else{
        sleep(10);
        signal(pid,SIGKILL);
    }
    return 0;
}
void fun_cz()
{
    printf("I don’t want hangup.\n");
}