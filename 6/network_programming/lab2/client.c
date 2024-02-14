#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_RETRIES 5

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <IP адрес сервера> <порт>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    int client_socket;
    struct sockaddr_in server_addr;

    // Создаем сокет
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    int retries = 0;
    while (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        if (retries >= MAX_RETRIES) {
            perror("Ошибка подключения к серверу");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "Повторная попытка подключения через 1 сек...\n");
        sleep(1);
        retries++;
    }

    for (int i = 1; i <= 10; i++) {
        char message[MAX_BUFFER_SIZE];
        snprintf(message, sizeof(message), "Процесс %d: Отправлено от клиента: %d\n", getpid(), i);

        if (send(client_socket, message, strlen(message), 0) == -1) {
            perror("Ошибка отправки данных на сервер");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        sleep(i);
    }

    close(client_socket);

    return 0;
}
