#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/syscall.h>

void my_delay() //延时函数，通过多重循环达到延时目的
{
	int i,j;
	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 1000; j++);
	}	
}
void * thread_func(void * arg) //线程函数，功能是循环打印传入的字符串参数
{
	int i;
	
	for (i = 0; i < 10; i++) {
		printf("thread_fun PID:%lu TID:0x%lx tid:%ld\n", (unsigned long)getpid(), (unsigned long)pthread_self(),syscall(SYS_gettid));
		printf("%s\n", (char *)arg);
		my_delay();
	}
}
int main()
{
	pthread_t tid1, tid2;
	printf("main PID:%lu  TID:0x%lx\n", (unsigned long)getpid(), (unsigned long)pthread_self());
	pthread_create(&tid1, NULL, thread_func, (void *)"A"); //创建线程1
	pthread_create(&tid2, NULL, thread_func, (void *)"B"); //创建线程2
	pthread_join(tid1, NULL); //等待线程1结束
	pthread_join(tid2, NULL); //等待线程2结束

	return 0;

}
