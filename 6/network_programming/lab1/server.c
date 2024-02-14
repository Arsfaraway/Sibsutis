#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_IP_LENGTH 16

int serverSocket;

void handleSignal(int signo) {
    if (signo == SIGINT) {
        printf("\nServer shutting down...\n");
        close(serverSocket);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    int port;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(0); 

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    char ip[MAX_IP_LENGTH];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), ip, INET_ADDRSTRLEN);
    
    printf("Server listening on %s\n", ip);

    getsockname(serverSocket, (struct sockaddr*)&serverAddr, &addrLen);
    port = ntohs(serverAddr.sin_port);
    
    printf("Server listening on port %d\n", port);

    signal(SIGINT, handleSignal);

    while (1) {
        int data;

        ssize_t bytesRead = recvfrom(serverSocket, &data, sizeof(data), 0,
                             (struct sockaddr*)&clientAddr, &addrLen);

        if (bytesRead < 0) {
            perror("Error receiving data");
            continue;
        }


        printf("Received from client %s:%d - %d\n",
       inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), data);

        int result = data;

        sendto(serverSocket, &result, sizeof(result), 0,
               (struct sockaddr*)&clientAddr, addrLen);

    }

    return 0;
}
