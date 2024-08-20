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
    int serverSocket, newSocket, valRead;
    struct sockaddr_in serverAddress, clientAddress;
    int opt = 1;
    int addressLength = sizeof(serverAddress);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created.\n");
    // Set socket options
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    // SO_KEEPALIVE
    int keepalive = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0)
    {
        perror("setsockopt SO_KEEPALIVE failed");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    // Bind
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully binded.\n");
    // Listen for connections
    if (listen(serverSocket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening...\n");
    // Accept incoming connection
    if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLength)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Server accepted the client...\n");
    // Start chat
    while (1)
    {
        // Read message from client
        valRead = read(newSocket, buffer, BUFFER_SIZE);
        printf("Client: %s\n", buffer);
        // Check for client exit command
        if (strcmp(buffer, "exit") == 0)
            break;
        // Clear the buffer and message
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
        // Get server message
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);
        // Send server message to client
        send(newSocket, message, strlen(message), 0);
        // Check for server exit command
        if (strcmp(message, "exit\n") == 0)
            break;
    }
    // Close sockets
    close(newSocket);
    close(serverSocket);
    return 0;
}

