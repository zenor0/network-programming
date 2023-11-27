#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 12345

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) {
        // 接收客户端数据
        ssize_t received_bytes = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                          (struct sockaddr*)&client_addr, &client_addr_len);
        if (received_bytes < 0) {
            perror("recvfrom");
            break;
        }

        // 处理接收到的数据（这里简单地将数据回送给客户端）
        sendto(client_socket, buffer, received_bytes, 0,
               (struct sockaddr*)&client_addr, client_addr_len);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    // 创建UDP套接字
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 绑定地址
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        // 接受客户端连接
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        // 创建新线程处理客户端连接
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
            perror("pthread_create");
            close(client_socket);
        }
    }

    // 关闭服务器套接字
    close(server_socket);

    return 0;
}
