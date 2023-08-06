#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t server_len = sizeof(server_addr);

    // Create client socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the UDP echo server. You can start sending messages!\n");

    while (1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_len);

        memset(buffer, 0, sizeof(buffer));
        int msg_len = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[msg_len] = '\0';

        printf("Echo: %s", buffer);
    }

    close(client_socket);
    return 0;
}

