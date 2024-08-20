#include <stdio.h>
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
        exit(EXIT_FAILURE); }
    printf("Socket successfully created.\n");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE); }
    // SO_KEEPALIVE
    int keepalive = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0) {
        perror("setsockopt SO_KEEPALIVE failed");
        exit(EXIT_FAILURE); }
    // SO_LINGER
    struct linger ling;
    ling.l_onoff = 1;
    ling.l_linger = 0; 
    if (setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0) {
        perror("setsockopt SO_LINGER failed");
        exit(EXIT_FAILURE); }
    // SO_SNDBUF
    int send_buf_size = 8192; 
    if (setsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, &send_buf_size, sizeof(send_buf_size)) < 0) {
        perror("setsockopt SO_SNDBUF failed");
        exit(EXIT_FAILURE);
    }
    // SO_RCVBUF
    int recv_buf_size = 8192;  
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, sizeof(recv_buf_size)) < 0) {
        perror("setsockopt SO_RCVBUF failed");
        exit(EXIT_FAILURE); }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE); }
    printf("Socket successfully binded.\n");
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE); }
    while (1) {
        valRead = read(newSocket, buffer, BUFFER_SIZE);
        printf("Client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0)  break;
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(newSocket, message, strlen(message), 0); }
    close(newSocket);
    close(serverSocket);
    return 0;
}

