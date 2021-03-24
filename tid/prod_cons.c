#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct msg
{
    struct msg *next;
    int num;
};

struct msg *head;
pthread_cond_t has_product=PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void *producer(void *p)
{
    struct msg *mp;
    int i;
    for(i=0;i<5;++i)
    {
        mp=malloc(sizeof(struct msg));/*如果分配成功：则返回指向被分配内存空间的指针,不然返回指针NULL;
        同时，当内存不再使用的时候，应使用free()函数将内存块释放掉。 */
        mp->num=rand()%1000+1;/*用rand()%x可以得到0到x之间的随机数*/
        printf("Produce %d\n",mp->num);
        pthread_mutex_lock(&lock);
        mp->next=head;
        head=mp;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&has_product);
        sleep(rand()%5);
    }
}
void *consumer(void *p)
{
    struct msg *mp;
    int i;
    for(i=0;i<5;++i)
    {
        pthread_mutex_lock(&lock);
        while (head==NULL)
        pthread_cond_wait(&has_product,&lock);
        mp=head;
        head=mp->next;
        pthread_mutex_unlock(&lock);
        printf("Consume %d\n",mp->num);
        free(mp);
        sleep(rand()%5);
    }
}
int main(int argc,char *argv[])
{
    pthread_t pt,ct;
    srand(time(NULL));/*srand()的种子主要有两种，srand(1)和srand((unsigned)time(NULL))，
    前者生成的随机数每次都是相等的，后者则是根据现在在走的时间取的随机数，是真随机数。*/
    pthread_create(&pt,NULL,producer,NULL);
    pthread_create(&ct,NULL,consumer,NULL);
    pthread_join(pt,NULL);
    pthread_join(ct,NULL);
    return 0;
}