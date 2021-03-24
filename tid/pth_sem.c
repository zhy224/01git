#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

sem_t mysem;

int buf[10];
int idx;
int i;
void * produucer(void *arg)
{
    for(i=0;i<10;i++)
    {
        idx++;
        if(i==9)
        {
            buf[idx]=0xab;
        }
        else
        {
            buf[idx]=rand();
        }
        printf("product:%d\n",buf[idx]);
        sem_post(&mysem);
        sleep(1);
    }
    return NULL;
}
void * consumer(void *arg)
{
    while (1)
    {
       sem_wait(&mysem);
       printf("consum:%d\n",buf[idx]);
       if(buf[idx]==0xab) break;
       idx--;
       sleep(1); /* code */
    }
    return NULL;
}
int main()
{
    pthread_t pid1,pid2;
    sem_init(&mysem,0,0);
    pthread_create(&pid1,NULL,produucer,NULL);
    pthread_create(&pid2,NULL,consumer,NULL);
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    sem_destroy(&mysem);
    return 0;
}