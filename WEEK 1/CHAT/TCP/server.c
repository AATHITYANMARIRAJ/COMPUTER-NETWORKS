#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    int addrlen = sizeof(server_addr);

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified IP address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started. Waiting for incoming connection...\n");

    // Accept incoming connection
    if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected. You can start chatting!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, BUFFER_SIZE);

        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    close(new_socket);
    close(server_socket);

    return 0;
}

