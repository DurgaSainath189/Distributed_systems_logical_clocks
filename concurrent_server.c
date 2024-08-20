#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#define PORT 7070
#define BUFFER_SIZE 1024
int main() {
    int serverSocket, newSocket, valRead;
    struct sockaddr_in serverAddress, clientAddress;
    int opt = 1;
    int addressLength = sizeof(serverAddress);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created.\n");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully binded.\n");
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening...\n");
    if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLength)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Server accepted the client...\n");
    while (1) {
        valRead = read(newSocket, buffer, BUFFER_SIZE);
        printf("Client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0)
            break;
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(newSocket, message, strlen(message), 0);
        if (strcmp(message, "exit\n") == 0)
            break;
    }
    close(newSocket);
    close(serverSocket);
    return 0;
}

