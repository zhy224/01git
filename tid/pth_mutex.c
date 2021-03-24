#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
pthread_mutex_t mylock;
int g_var;
void * pthread1(void *arg)
{
    int i;
    printf("enter th1\n");
    pthread_mutex_lock(&mylock);
    for(i=0;i<10;i++)
    {
        g_var++;
        printf("th1:%d\n",g_var);
        sleep(1);
    }
    pthread_mutex_unlock(&mylock);
    return NULL;
}
void * pthread2(void *arg)
{
    int i;
    printf("enter th2\n");
    pthread_mutex_lock(&mylock);
    for(i=0;i<10;i++)
    {
        g_var--;
        printf("th2:%d\n",g_var);
        sleep(1);
    }
    pthread_mutex_unlock(&mylock);
    return NULL;
}
int main()
{
    pthread_t tid1,tid2;
    pthread_mutex_init(&mylock,NULL);
    pthread_create(&tid1,NULL,pthread1,NULL);
    pthread_create(&tid2,NULL,pthread2,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&mylock);
}