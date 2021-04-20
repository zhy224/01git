#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

#define STDIN 0

int main()
{
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec=5;
    tv.tv_usec=0;
    char buf[32];
    FD_ZERO(&readfds);
    FD_SET(STDIN,&readfds);
    printf("what's your name?\n");
    select(STDIN+1,&readfds,NULL,NULL,&tv);
    if(FD_ISSET(STDIN,&readfds))
    {
        scanf("%s",buf);
        printf("your name is %s\n",buf);
    }
    else
    {
        printf("input timeout\n");
    }
}