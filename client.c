#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h> 
#define PORT 7070
#define BUFFER_SIZE 1024
int main()
{
    int clientSocket, valRead;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    // Create client socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created.\n");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
    {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    // Set SO_KEEPALIVE option
    int keepalive = 1;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0)
    {
        perror("setsockopt SO_KEEPALIVE failed");
        exit(EXIT_FAILURE);
    }
    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");
    // Start chat
    while (1)
    {
        // Clear the buffer and message
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
        // Get client message
        printf("Client: ");
        fgets(message, BUFFER_SIZE, stdin);
        // Send client message to the server
        send(clientSocket, message, strlen(message), 0);
        // Check for client exit command
        if (strcmp(message, "exit\n") == 0)
            break;
        // Read message from server
        valRead = read(clientSocket, buffer, BUFFER_SIZE);
        printf("Server: %s\n", buffer);
        // Check for server exit command
        if (strcmp(buffer, "exit") == 0)
            break;
    }
    // Close socket
    close(clientSocket);
    return 0;
}

