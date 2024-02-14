#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_BACKLOG 5

void sigchld_handler(int signo) {
    
    int status;
    pid_t terminated_child_pid;

    while ((terminated_child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Процесс %d завершился с кодом возврата: %d\n", terminated_child_pid, WEXITSTATUS(status));
    }
}

void handle_client(int client_socket, int client_number) {
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        printf("Процесс %d: %.*s", client_number, (int)bytes_received, buffer);
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0; 

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка привязки сокета к адресу");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (getsockname(server_socket, (struct sockaddr*)&server_addr, &client_addr_len) == -1) {
        perror("Ошибка получения номера порта");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен на порту: %d\n", ntohs(server_addr.sin_port));

    if (listen(server_socket, MAX_BACKLOG) == -1) {
        perror("Ошибка прослушивания входящих соединений");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    int client_number = 1;

    if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
                perror("Ошибка установки обработчика сигнала");
                exit(EXIT_FAILURE);
            }

    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
            perror("Ошибка приема соединения");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Ошибка создания процесса-потомка");
            close(client_socket);
            continue;
        } else if (child_pid == 0) {
            close(server_socket);
            handle_client(client_socket, client_number);
            exit(EXIT_SUCCESS);
        } else {
            close(client_socket);
            client_number++;
        }
    }

    close(server_socket);

    return 0;
}
