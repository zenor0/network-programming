// udp_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 替换为服务器的IP地址

    while (1) {
        // 从用户输入读取数据
        printf("Enter data to send (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // 发送数据到服务器
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // 如果用户输入 'exit'，退出循环
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        // 接收服务器的响应
        ssize_t received_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);

        if (received_size == -1) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        // 打印服务器的响应
        buffer[received_size] = '\0';
        printf("Server response: %s\n", buffer);
    }

    // 关闭socket
    close(sockfd);

    return 0;
}
