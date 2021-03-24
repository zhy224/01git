#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

struct msg
{
    struct msg *next;
    int num;
};

struct msg *head;
pthread_cond_t has_product=PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

pthread_t tid_p[1024],tid_c[1024],tid1,tid2;
int P=0,C=0;

void *producer(void *p)
{
    struct msg *mp;
    int i;
    for(i=0;i<5;++i)
    {
        mp=malloc(sizeof(struct msg));/*如果分配成功：则返回指向被分配内存空间的指针,不然返回指针NULL;
        同时，当内存不再使用的时候，应使用free()函数将内存块释放掉。 */
        mp->num=rand()%1000+1;/*用rand()%x可以得到0到x之间的随机数*/
        tid_p[P]=pthread_self();
        tid1=syscall(__NR_gettid);
        printf("Produce:%d,%lu,%d\n",mp->num,tid_p,tid1);
        pthread_mutex_lock(&lock);
        mp->next=head;
        head=mp;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&has_product);
        sleep(1);
    }
    P++;
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
        tid_c[C]=pthread_self();
        tid2=syscall(__NR_gettid);;
        printf("Consume:%d,%lu,%d\n",mp->num,tid_c,tid2);
        free(mp);
        sleep(1);
    }
    C++;
}
int main(){
    int pid,i,rfd,wfd,len=0,fd_in;
    pthread_t pt,ct;
    srand(time(NULL));/*srand()的种子主要有两种，srand(1)和srand((unsigned)time(NULL))，
    前者生成的随机数每次都是相等的，后者则是根据现在在走的时间取的随机数，是真随机数。*/
    char str[32];
    char c[10];
    int flag,stdinflag;
    fd_set write_fd,read_fd;
    struct timeval net_timer;
    mkfifo("fifo1",0666);
	mkfifo("fifo2",0666);
    rfd=open("fifo1",O_RDONLY);/*调用成功时返回一个文件描述符fd;调用失败时返回-1，并修改errno*/
	wfd=open("fifo2",O_WRONLY);
    if(rfd<=0||wfd<=0) return 0;
        printf("this is procon\n");
    while (1)
    {
        FD_ZERO(&read_fd);//将指定的文件描述符集清空
        FD_SET(rfd,&read_fd);//在文件描述符集合中增加一个新的文件描述符
        FD_SET(fileno(stdin),&read_fd);
        net_timer.tv_sec=5;
        net_timer.tv_usec=0;
        memset(str,0,sizeof(str));
        if(i=select(rfd+1,&read_fd,NULL,NULL,&net_timer)<=0)
        {/*负值：select错误;正值：某些文件可读写或出错;0：等待超时，没有可读写或错误的文件*/
            continue;
        }
        if(FD_ISSET(rfd,&read_fd))//测试指定的文件描述符是否在该集合中
        {
            printf("-----------------------------\n");
            read(rfd,str,sizeof(str));
            printf("admin:%s\n",str);
            if(strcmp(str,"create producer\n")==0)
			{
				pthread_create(&pt,NULL,producer,NULL);
                pthread_join(pt, NULL); //等待线程producer结束h
			}
            else if(strcmp(str,"create consumer\n")==0)
			{
				pthread_create(&ct,NULL,consumer,NULL);
				pthread_join(ct,NULL);	
			}
            else if(strstr(str,"delete")!=NULL)
			{
                printf("delete\n");
                int k=pthread_key_delete(tid1);
                char wstr3[1024]="OK";
				char wstr4[1024]="failed";
				if(k==0)
				{
					write(wfd,wstr3,strlen(wstr3));
				}
				else
				{
					write(wfd,wstr4,strlen(wstr4));
				}	
			}
            else if(strcmp(str,"list producer\n")==0)
			{
				char buf[128];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"%lu",tid_p);
                write(wfd,buf,strlen(buf));
			}
			else if(strcmp(str,"list consumer\n")==0)
			{
				char buf[128];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"%lu",tid_c);
                write(wfd,buf,strlen(buf));
			}
            else
            {
                char buf[32]="Unknown Command";
                write(wfd,buf,strlen(buf));
            }
            if(!strncmp(str, "exit", 4))
            {
                exit(1);
            }
        }
        /*if(FD_ISSET(fileno(stdin),&read_fd))
        {
            fgets(str,sizeof(str),stdin);
            printf("-----------------------------\n");
            len=write(wfd,str,strlen(str));
        }*/
    }
    close(rfd);
    close(wfd);
}