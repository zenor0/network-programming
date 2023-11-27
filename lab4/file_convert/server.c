// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "msg.c"


#define BUFFER_SIZE 1024


// Function to process input data
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
    int server_port = atoi(argv[1]);
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);


    // 创建 socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }


    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind & Listen for incoming connections
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    show_msg(INFO_TYPE, "Server is listening on port %d...\n", server_port);
    while (1) {
        int client_socket = accept(sockfd, NULL, NULL);
        if (client_socket == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Get connection infos
        getpeername(client_socket, (struct sockaddr *)&client_addr, &client_len);
        show_msg(DEBUG_TYPE, "Client %s:%d try to send a file.\n", 
                    inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char buffer[BUFFER_SIZE];
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                // perror("Receive failed");
                close(client_socket);
                break;
            }

            process_handler(buffer);
            send(client_socket, buffer, strlen(buffer), 0);
        }
        close(client_socket);
    }

    close(sockfd);
    return 0;
}
