#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "192.168.1.69"
#define SERVER_PORT 48435
#define MAX_IP_LENGTH 16

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    int port;
    char ip[MAX_IP_LENGTH];

    printf("Enter the server IP address: ");
    scanf("%15s", ip);

    printf("Enter the server port: ");
    scanf("%d", &port);

    // Создаем UDP сокет
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    int data;
    printf("Enter a number to send to the server: ");
    scanf("%d", &data);

    for (int i = 0; i < 100; ++i) {
        // Отправляем данные серверу
        sendto(clientSocket, &data, sizeof(data), 0,
            (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        sleep(data);
        // Получаем и выводим преобразованные данные от сервера
        ssize_t bytesRead = recvfrom(clientSocket, &data, sizeof(data), 0, NULL, NULL);
        if (bytesRead < 0) {
            perror("Error receiving data");
        } else {
            //printf("Received from server: %d\n", data);
            printf("Received from server: %d\n", data);
        }
    }
    
    close(clientSocket);

    return 0;
}
