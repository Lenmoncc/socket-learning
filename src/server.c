#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 10000
#define BUFFER_SIZE 1024

int main(void)
{
    // 1. 创建连接套接字
    int sockfd;
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
    ser_addr.sin_addr.s_addr = INADDR_ANY;

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

    // 4. 接收客户端的连接请求 阻塞等待
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int afd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
    if (afd < 0)
    {
        perror("accept");
        close(sockfd);
        return -1;
    }

    // 5. 通信，发/收操作 send/recv
    char buff[BUFFER_SIZE];
    char server[] = "send succeed!";
    
    while (1)
    {
        memset(buff, 0, sizeof(buff)); // 清空缓冲区

        int recvfd = recv(afd, buff, sizeof(buff) - 1, 0); // 保留一个字节用于'\0'
        if (recvfd < 0)
        {
            perror("receive failed");
            break; // 退出循环
        }
        else if (recvfd == 0)
        {
            printf("Client disconnected\n");
            break; // 客户端断开连接
        }
        else
        {
            buff[recvfd] = '\0'; // 确保字符串结束
            printf("client say: %s\n", buff);
        }

        if (send(afd, server, sizeof(server), 0) < 0)
        {
            perror("fail to send");
            break; // 退出循环
        }
    }

    // 6. 关闭文件描述符
    close(afd);
    close(sockfd);
    return 0;
}
