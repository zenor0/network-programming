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
    memset(buffer, 0, buffer_size);
    ssize_t received_size = recv(sockfd, buffer, buffer_size, 0);
    if (received_size == -1) {
        perror("Error receiving data");
        return -1;
    }

    FILE* file = fopen(filename, "w");
    fwrite(buffer, 1, received_size, file);
    fclose(file);

    return received_size;
}

void make_request(int socket, const char* input_filename, const char *save_filename) {
    char buffer[BUFFER_SIZE];
    int file_size;
    if ((file_size = read_file(input_filename, buffer, BUFFER_SIZE)) == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    send(socket, buffer, file_size, 0);
    if (recv_save_file(socket, buffer, BUFFER_SIZE, save_filename) == -1) {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout,NULL);
    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Read input from user, send to server, and receive response
    make_request(client_socket, "A1", "B1");
    make_request(client_socket, "A2", "B2");

    show_msg(DEBUG_TYPE, "File B1 received:\n");
    system("cat B1");
    show_msg(DEBUG_TYPE, "File B2 received:\n");
    system("cat B2");

    close(client_socket);

    return 0;
}
