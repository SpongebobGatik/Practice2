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
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    result = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (result == SOCKET_ERROR) {
        printf("Failed to connect to the server.\n");
        system("pause");
        return 1;
    }
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    while (1) {
        printf("Enter data to send to the server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Response from server: %s", buffer);
        }
    }
    free(buffer);
    closesocket(client_socket);
    WSACleanup();
    system("pause");
    return 0;
}
