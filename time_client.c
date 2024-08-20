#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#define PORT 7070
#define BUFFER_SIZE 1024
void getCurrentTime(char *timeStr) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, 50, "%Y-%m-%d %H:%M:%S", timeinfo);
}
int main() {
    int clientSocket, valRead;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE + 50] = {0}; 
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created.\n");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");
    while (1) {
        char currentTime[50];
        getCurrentTime(currentTime);
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE + 50);
        printf("Client: ");
        fgets(message + 20, BUFFER_SIZE, stdin);
        strcat(message, currentTime);
        send(clientSocket, message, strlen(message), 0);
        if (strcmp(message + 20, "exit\n") == 0)
            break;
        valRead = read(clientSocket, buffer, BUFFER_SIZE);
        printf("Server (%s): %s\n", currentTime, buffer);
        if (strcmp(buffer, "exit") == 0)
            break;
    }
    close(clientSocket);
    return 0;
}

