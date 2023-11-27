// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "msg.c"

#define BUFFER_SIZE 1024

int read_file(const char* filename, char* buffer, size_t buffer_size) {
    memset(buffer, 0, buffer_size);
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    size_t size = fread(buffer, 1, buffer_size, file);
    fclose(file);

    return size;
}


int recv_save_file(int sockfd, char* buffer, size_t buffer_size, const char* filename) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t received_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (received_size == -1) {
        perror("Error receiving data");
        return -1;
    }
    FILE* file = fopen(filename, "w");
    fwrite(buffer, 1, strlen(buffer), file);
    fclose(file);

    return received_size;
}

int connect_to(char *server_ip, int server_port, const char* filename, const char* save_filename) {
    show_msg(INFO_TYPE, "Connecting to %s:%d, sending %s\n", server_ip, server_port, filename);
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        return -1;
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // 读取文件A1的内容并发送
    int file_size;

    if ((file_size = read_file(filename, buffer, BUFFER_SIZE)) == -1) {
        perror("Error opening file.txt");
        return -1;
    }
    sendto(sockfd, buffer, file_size, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 接收并保存文件B1的内容
    if (recv_save_file(sockfd, buffer, BUFFER_SIZE, save_filename) == -1) {
        return -1;
    }
    close(sockfd);

    return 0;
}

int main(int argc, char *argv[]) {
    setbuf(stdout, 0);
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s <server1_ip> <server1_port> <filename1> <save_filename1> [ <server2_ip> <server2_port> <filename2> <save_filename2>  ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i += 4) {
        show_msg(INFO_TYPE, "Progress: (%d/%d)\n", i / 4 + 1, argc / 4);
        if (connect_to(argv[i], atoi(argv[i + 1]), argv[i + 2], argv[i + 3]) == -1) {
            fprintf(stderr, "Error connecting to %s:%s\n", argv[i], argv[i + 1]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}


