#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void fun_ctrl_c();
int main()
{
    int i;
    sigset_t set,pendset;
    struct sigaction action;
    (void) signal(SIGINT,fun_ctrl_c);
    if(sigemptyset(&set)<0)
    {perror("加入信号集合错误");}
    if(sigaddset($set))
}