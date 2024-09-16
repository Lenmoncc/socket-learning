#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 10000


int main() {
    // 1. 创建套接字连接符
    int sockfd;
    
    // 2. 初始化socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    
    // 3. 连接服务器
    struct sockaddr_in clt_addr;
    clt_addr.sin_family = AF_INET;
    clt_addr.sin_port = htons(PORT);
    clt_addr.sin_addr.s_addr = inet_addr("192.168.50.132");

    int ret = connect(sockfd, (struct sockaddr *)&clt_addr, sizeof(clt_addr));
    if (ret < 0) {
        perror("connect");
        exit(1);
    }
    
    // 4. 与服务器通信  接/发 recv/send
    char client[] = "client sended!";
    char buff[1024];
    
    while (1) {
        int st = send(sockfd, client, strlen(client), 0);
        if (st < 0) {
            perror("fail to send");
            break;
        }

        int rcvfd = recv(sockfd, buff, sizeof(buff), 0);
        if (rcvfd < 0) {
            perror("receive failed!");
            break;
        } else {
            buff[rcvfd] = '\0'; // 确保字符串结束
            printf("%s\n", buff);
        }
        
        sleep(1);
    }
    
    // 5. 关闭文件描述符
    close(sockfd);
    return 0;
}
