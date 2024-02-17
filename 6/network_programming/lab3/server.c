#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

FILE *sharedFile;
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int clientSocket;
    int threadNumber;
} ThreadArgs;

void *handleClient(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int clientSocket = threadArgs->clientSocket;
    int threadNumber = threadArgs->threadNumber;

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        pthread_mutex_lock(&fileMutex);
        fprintf(sharedFile, "Thread %d: %.*s", threadNumber, (int)bytesRead, buffer);
        fflush(sharedFile); 
        pthread_mutex_unlock(&fileMutex);

        printf("Thread %d received: %.*s", threadNumber, (int)bytesRead, buffer);
    }

    close(clientSocket);
    free(threadArgs);
    return NULL;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    pthread_t threads[MAX_CLIENTS];
    int port = 0;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(0);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    getsockname(serverSocket, (struct sockaddr *)&serverAddr, &clientLen);
    port = ntohs(serverAddr.sin_port);
    printf("Server started on port %d\n", port);

    sharedFile = fopen("data.txt", "w");
    if (sharedFile == NULL) {
        perror("Error opening shared file");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    while (1) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen)) == -1) {
            perror("Error accepting connection");
            continue;
        }

        ThreadArgs *threadArgs = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        threadArgs->clientSocket = clientSocket;
        threadArgs->threadNumber = i;

        if (pthread_create(&threads[i++], NULL, handleClient, (void *)threadArgs) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        pthread_detach(threads[i - 1]);

        if (i >= MAX_CLIENTS) {
            i = 0;
            while (i < MAX_CLIENTS) {
                pthread_join(threads[i++], NULL);
            }
            i = 0;
        }
    }

    fclose(sharedFile);
    close(serverSocket);

    return 0;
}
