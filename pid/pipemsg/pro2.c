#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#define KEYPATH "."
#define PROJECT_ID 'a'
struct msg
{
    long mtype;
    char name[10];
    char mtext[100];
};
void fun_ctrl_c();

int main(void)
{
    int msgpid,len;
    key_t key;
    pid_t pid1;
    char buf[100],sender[20];
    struct msg msgs,msgr;
    (void)signal(SIGINT,fun_ctrl_c);
    if((key=ftok(KEYPATH,PROJECT_ID))==-1){
        perror("Error in creating the key:");
        exit(1);
    }

    if((msgpid=msgget(key,IPC_CREAT|0666))==-1){
        perror("Error in creating the message queue");
        exit(1);
    }
    printf("Please enter your Group Number:");
    scanf("%ld",&msgs.mtype);
    getchar();
    printf("Please enter your name:");
    scanf("%s",&msgs.name);
    getchar();
    printf("\nYou entered HYL chat room!\n");
    sleep(1);
    system("clear");
    printf("You can enter what you want to say~\n");
    pid1=fork();
    if(pid1<0){
        perror("Error:");
        exit(1);
    }
    else if (pid1 == 0)
    {
        while (1)
        {
            len=msgrcv(msgpid,&msgr,sizeof(msgr),msgs.mtype,0);
            if(len<0){
                perror("Read msg failed:");
                exit(1);
            }
            if (len == 0) continue;
            if((strcmp(msgr.name,msgs.name))==0){
                msgsnd(msgpid,&msgr,100,0);
                continue;
            }
            printf("From %s: %s",msgr.name,msgr.mtext);
            
        }
        
        
    }
    else
    {
        while (1)
        {
            if((fgets(msgs.mtext,100,stdin))==NULL){
                printf("You cannot sent the empty msg!\n");
                continue;
            }
            if((msgsnd(msgpid,&msgs,100,0))<0){
                perror("msg send failed:");
            }
            else
            {
                if(strcmp(msgs.mtext,"886...\n")==0) fun_ctrl_c();
                printf("%s: %s",msgs.name,msgs.mtext);
            }
            
        }
        
    }
}
void fun_ctrl_c()
{
    char a;
    printf("Are you sure to quit?(y?)\n");
    scanf("%c",&a);
    getchar();
    if(a=='y'){
        printf("Thank you for using HYL chat room!\n");
        sleep(1);
        system("clear");
        exit(0);
    }
    else
    {
        printf("Back to HYL chat room.\n");
        return;
    }
    
}

