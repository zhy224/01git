#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAXDATASIZE 256
#define SERVPORT 10011
#define STDIN 0
int main()
{
    int sockfd;
    int recvfrombytes;
    char buf[MAXDATASIZE];
    char *str;
    char name[MAXDATASIZE];
    char sendto_str[MAXDATASIZE];
    struct sockaddr_in serv_addr;
    fd_set  rfd_set, wfd_set, efd_set;
    struct timeval timeout;
    int ret;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVPORT);
    //inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr=inet_addr("192.168.1.173");
    fcntl(sockfd,F_SETFD,O_NONBLOCK);
    printf("enter your name first:");
    scanf("%s",name);
    name[strlen(name)] = '\0';
    printf("%s: ",name);
    fflush(stdout);
    sendto(sockfd, name, strlen(name), 0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    while(1)
    {
        FD_ZERO(&rfd_set);
        FD_ZERO(&wfd_set);
        FD_ZERO(&efd_set);
        FD_SET(STDIN, &rfd_set);
        FD_SET(sockfd, &rfd_set);
        FD_SET(sockfd, &wfd_set);
        FD_SET(sockfd, &efd_set);
        timeout.tv_sec = 10;
        ret = select(sockfd+1, &rfd_set, &wfd_set, &efd_set, &timeout);
        if(ret == 0)
        {
            continue;
        }
        if(ret < 0)
        {
            perror("select  error:");
            exit(1);
        }
        if(FD_ISSET(STDIN, &rfd_set))
        {
            fgets(sendto_str, 256, stdin);
            sendto_str[strlen(sendto_str)-1] = '\0';
            if(strncmp("quit",sendto_str,4) == 0)
            {
                close(sockfd);
                exit(0);
            }
            sendto(sockfd, sendto_str, strlen(sendto_str), 0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        }
        if(FD_ISSET(sockfd, &rfd_set))
        {
            recvfrombytes = recvfrom(sockfd, buf, strlen(buf), 0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
            if(recvfrombytes == 0)
            {
                close(sockfd);
                exit(0);
            }
            buf[recvfrombytes] = '\0';
            printf("server: %s\n",buf);
            printf("%s: \n",name);
            fflush(stdout);
        }
        if(FD_ISSET(sockfd,&efd_set))
        {
            close(sockfd);
            exit(0);
        }
    }
}