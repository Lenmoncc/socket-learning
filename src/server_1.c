#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define PORT 10000
#define BUFFER_SIZE 1024

//回调函数，回收子进程资源
void callback(int num)
{
    while (1)
    {
        pid_t pid=waitpid(-1,NULL,WNOHANG);
        if(pid<=0)
        {
            printf("子进程资源回收完毕！\n");
       	break;
        }
        printf("child process die, pid=%d\n",pid);
    }
    
}


int childwork(int afd);
int main(void)
{
    // 1. 创建连接套接字
    int sockfd;//监听客户端连接请求
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("fail to socket");
        return -1;
    }

    // 2. 绑定端口
    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_addr.s_addr = inet_addr("192.168.50.132");

    if (bind(sockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
    {
        perror("fail to bind");
        close(sockfd);
        return -1;
    }

    // 3. 监听客户端发出的连接请求
    if (listen(sockfd, 255) < 0)
    {
        perror("fail to listen");
        close(sockfd);
        return -1;
    }

/*
           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/
    //**注册信号的捕捉**
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=callback;//句柄
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD,&act,NULL);
    
    //接受多个客户端连接请求，accept()循环调用
    while (1)
    {

    // 4. 接收客户端的连接请求 阻塞等待
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int afd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (afd < 0)
        {
            if(errno==EINTR)
            {
            //accept调用被信号中断，解除阻塞
            continue;//重新调用accept
            }
        perror("accept");
        close(sockfd);
        return -1;
        }

    //建立连接，创建子进程，让子进程与客户端通信
        pid_t pid=fork();
        if(pid==0)
        {
        close(sockfd);
        while (1)
        {
            int ret=childwork(afd);
            if(ret<=0)
            {
                break;
            }
        }
        close(afd);
        exit(0);      
        }
        else if(pid>0)
        {
            close(afd);
        }      
    }

    // 6. 关闭文件描述符
    //close(afd);//释放与客户端连接的资源
    //close(sockfd);
    return 0;
}


// 5. 通信，发/收操作 send/recv
int childwork(int afd)
{
    char buff[BUFFER_SIZE];
    char server[] = "send succeed!";

    memset(buff, 0, sizeof(buff)); // 清空缓冲区

    int recvfd = recv(afd, buff, sizeof(buff) - 1, 0); // 保留一个字节用于'\0'
    if (recvfd < 0)
    {
        perror("receive failed");
//        break; // 退出循环
    }
    else if (recvfd == 0)
    {
        printf("Client disconnected\n");
  //      break; // 客户端断开连接
    }
    else
    {
        buff[recvfd] = '\0'; // 确保字符串结束
        printf("client say: %s\n", buff);
    }

    if (send(afd, server, sizeof(server), 0) < 0)
    {
        perror("fail to send");
    //    break; // 退出循环
    }

        return recvfd;

}
