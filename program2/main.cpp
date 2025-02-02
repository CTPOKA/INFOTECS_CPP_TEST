#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "../library/library.h"

#define SOCKET_PATH "/tmp/simple_socket"

void receiveData(int client_fd) {
    char buffer[256];

    while (true) {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Соединение с сервером потеряно. Ожидание переподключения...\n";
            close(client_fd);
            break;
        }

        buffer[bytes_received] = '\0';

        if (lib::isDivisibleBy32(buffer)) {
            std::cout << "Полученные данные: " << buffer << std::endl;
        } else {
            std::cout << "Ошибка: Число не кратно 32" << std::endl;
        }
    }
}

int main() {
    int client_fd;
    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    while (true) {
        client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (client_fd == -1) {
            perror("Ошибка создания сокета");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
            std::cout << "Подключено к серверу!\n";
            receiveData(client_fd);
        }

        close(client_fd);
        std::cout << "Попытка повторного подключения через 2 секунды...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
