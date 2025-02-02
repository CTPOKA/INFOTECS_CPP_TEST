#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <condition_variable>
#include <algorithm>
#include <thread>
#include "../library/library.h"

#define SOCKET_PATH "/tmp/simple_socket"

std::mutex mtx;
std::condition_variable cv;
char buffer[256];
int write_index = 0;

void inputThread()
{
    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input.length() > 64 || !std::all_of(input.begin(), input.end(), ::isdigit))
        {
            std::cout << "Ошибка: строка должна содержать только цифры и быть длиной до 64 символов." << std::endl;
            continue;
        }

        std::unique_lock<std::mutex> lock(mtx);

        lib::sortDigitsAndReplaceEven(input);

        strcpy(buffer + write_index, input.c_str());
        write_index += input.size();
        
        cv.notify_one();
    }
}

bool isSocketActive(int sockfd)
{
    char buffer;
    ssize_t n = recv(sockfd, &buffer, 1, MSG_PEEK | MSG_DONTWAIT);
    return (n != 0);
}

void processingThread(int server_fd)
{
    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1) {
            continue;
        }

        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return write_index > 0; });

            if (!isSocketActive(client_fd)) {
                std::cout << "Клиент отключился. Ожидание нового клиента." << std::endl;
                close(client_fd);
                break;
            }

            std::string str = buffer;
            write_index = 0;
            lock.unlock();

            std::cout << "Полученные данные: " << str << std::endl;

            std::string message = std::to_string(lib::calcSumOfDigits(str));

            std::cout << "Отправка данных: " << message << std::endl;

            if (send(client_fd, message.c_str(), message.size(), 0) == -1) {
                close(client_fd);
                break;
            }
        }
    }
}

int main()
{
    unlink(SOCKET_PATH);

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Ошибка создания сокета");
        return 1;
    }

    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Ошибка привязки сокета");
        return 1;
    }

    if (listen(server_fd, 1) == -1)
    {
        perror("Ошибка прослушивания сокета");
        return 1;
    }

    std::cout << "Введите данные:" << std::endl;
    std::thread thread1(inputThread);
    std::thread thread2(processingThread, server_fd);

    thread1.join();
    thread2.join();

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
