// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "msg.c"


#define BUFFER_SIZE 1024

void process_handler(char* input) {
    int length = strlen(input);
    for (int i = 0; i < length; i++) {
        input[i] = toupper(input[i]);
    }

    char server_sign[100] = "\n=====\nModified by server.\n";
    strcat(input, server_sign);
}

int main(int argc, char *argv[]) {
    setbuf(stdout, 0);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定地址
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    show_msg(INFO_TYPE, "Server is listening on port %d...\n", atoi(argv[1]));

    while (1) {
        // 接收数据
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t received_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (received_size == -1) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        // 处理数据并发送
        // process_and_send(sockfd, client_addr, buffer, received_size);
        show_msg(DEBUG_TYPE, "Client %s:%d sended a file.\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        process_handler(buffer);
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    }

    // 关闭socket
    close(sockfd);

    return 0;
}
