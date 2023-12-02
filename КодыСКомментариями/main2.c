#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning (disable:4996)
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define PORT 6379
#define BUFFER_SIZE 104857600

int main() {
    WSADATA wsa_data; // Структура для хранения информации о Windows Sockets
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data); // Инициализация библиотеки Winsock
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета для TCP
    struct sockaddr_in server_address; // Структура для хранения адреса сервера
    server_address.sin_family = AF_INET; // Установка типа адреса (IPv4)
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Установка IP-адреса сервера (localhost)
    server_address.sin_port = htons(PORT); // Установка порта сервера
    result = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)); // Подключение к серверу
    if (result == SOCKET_ERROR) {
        printf("Failed to connect to the server.\n"); // Сообщение об ошибке подключения
        system("pause");
        return 1;
    }
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    while (1) {
        printf("Enter data to send to the server: "); // Приглашение ввести данные для отправки
        fgets(buffer, BUFFER_SIZE, stdin); // Чтение данных из стандартного ввода
        if (strcmp(buffer, "exit\n") == 0) { // Проверка на команду выхода
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0); // Отправка данных на сервер
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0); // Прием ответа от сервера
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Добавление символа конца строки
            printf("Response from server: %s", buffer); // Вывод ответа от сервера
        }
    }
    free(buffer);
    closesocket(client_socket);
    WSACleanup();
    system("pause");
    return 0;
}
