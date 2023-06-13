#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

int server_socket;

void exit_program() {
    printf("\nЗакрытие socket и завершение программы.\n");
    close(server_socket);
    exit(0);
}

int get_best_fan_index(int *scores, int fans_count) {
    int mx = -1, mx_id = -1;
    printf("Просмотр открыток начат ...\n");
    for (int i = 0; i < fans_count; ++i) {
        if (scores[i] > mx) {
            mx = scores[i];
            mx_id = i;
        }
    }
    printf("Просмотр открыток завершен\n");
    return mx_id;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: <%s> <port> <fan count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int fans_count = atoi(argv[2]);

    if (fans_count < 1 || fans_count > 10000) {
        printf("Ошибка: количество фанатов должно быть от 1 до 10000 включительно\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, exit_program);

    ssize_t read_res;
    struct sockaddr_in fans_sockets[fans_count];
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Создание серверного сокета
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("Ошибка при создании сокета\n");
        exit_program();
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Привязка серверного сокета к IP-адресу и порту
    if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Ошибка при соединении сокета\n");
        exit_program();
    }

    printf("Студентка ожидает письма\n");

    int fans_scores[fans_count];
    for (int fan = 0; fan < fans_count; fan++) {
        fans_scores[fan] = -1;
    }

    int letters_got = 0;

    while (letters_got < fans_count) {
        for (int fan = 0; fan < fans_count; fan++) {
            int result;
            struct sockaddr_in client_response;
            int client_addrlen = sizeof(client_response);
            while (recvfrom(server_socket, &result, sizeof(result), 0, (struct sockaddr *) &client_response, &client_addrlen) < 0) {
                sleep(2);
                printf("waiting...\n");
            }
            if (result > fans_scores[fan]) {
                if (fans_scores[fan] == -1) {
                    letters_got++;
                }
                fans_scores[fan] = result;
                printf("Получено письмо от фаната %d привлекательностью %d\n", fan + 1, result);

            } else {
                printf("Получино сообщение: %d\n", result);
            }
            memcpy(&fans_sockets[fan], &client_response, sizeof(client_response));
        }
    }
    int best_id = get_best_fan_index(fans_scores, fans_count);
    printf("Фанат %d — самый привлекательный!\n", best_id + 1);
    int good_answer = 1, bad_answer = 0;

    for (int i = 0; i < fans_count; ++i) {
        int answer = bad_answer;
        if (i == best_id) {
            answer = good_answer;
        }
        sendto(server_socket, &answer, sizeof(answer), 0, (struct sockaddr *) &fans_sockets[i],
               sizeof(struct sockaddr_in));
    }

    int exit_code = -1;
    for (int i = 0; i < fans_count; ++i) {
        // Отправка кода клиенту для завершения работы программы
        sendto(server_socket, &exit_code, sizeof(int), 0, (struct sockaddr *) &fans_sockets[i],
               sizeof(struct sockaddr_in));
    }
    // Завершение программы:
    printf("День завершён.\n");
    exit_program();
}