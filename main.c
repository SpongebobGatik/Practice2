#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "stack.h"
#include "set.h"
#include "queue.h"
#include "table.h"
#define MAX_SIZE 1000
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define PORT 6379
#define BACKLOG 10
#define BUFFER_SIZE 1024

DWORD WINAPI handle_client(LPVOID lpParam) {
    SOCKET client_socket = (SOCKET)lpParam;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes_received] = '\0';
    char** argv = NULL;
    int argc = 0;
    char* token = strtok(buffer, " ");
    while (token != NULL) {
        argv = realloc(argv, sizeof(char*) * ++argc);
        argv[argc - 1] = token;
        token = strtok(NULL, " ");
    }
    char* filename = NULL;
    char* query = NULL;
    char* item = NULL;
    char* basename = NULL;
    char* key = NULL;
    int temp;
    if (argc < 4 || argc > 7) {
        closesocket(client_socket);
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filename = argv[i + 1];
        }
        else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) {
            query = argv[i + 1];
            temp = i + 1;
            basename = argv[i + 2];
            if (i + 5 > argc) item = argv[i + 3];
            else {
                key = argv[i + 3];
                item = argv[i + 4];
            }
        }
    }
    int pos1 = 0;;
    int pos2 = 0;;
    int status = 0;;
    char* result = NULL;
    if (filename != NULL && query != NULL) {
        FILE* file = fopen(filename, "r");
        if (!file) {
            FILE* file = fopen(filename, "w");
        }
        if (strcmp(argv[temp], "SPUSH") == 0) {
            Stack* stack = loadFromFileStack(filename, basename, &pos1, &pos2, &status);
            if (stack == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    SPUSH(stack, item);
                    result = malloc(strlen(item) + 5);
                    sprintf(result, "-> %s\n", item);
                    if (status == 1) status = 0;
                    fclose(file);
                    saveToFileStack(stack, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "SPOP") == 0) {
            Stack* stack = loadFromFileStack(filename, basename, &pos1, &pos2, &status);
            if (stack == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    char* element = SPOP(stack);
                    result = malloc(strlen(element) + 5);
                    sprintf(result, "-> %s\n", element);
                    if (status == 2) status = 0;
                    fclose(file);
                    saveToFileStack(stack, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "SADD") == 0) {
            Set* set = loadFromFileSet(filename, basename, &pos1, &pos2, &status);
            if (set == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    SADD(set, item);
                    result = malloc(strlen(item) + 5);
                    sprintf(result, "-> %s\n", item);
                    if (status == 1) status = 0;
                    fclose(file);
                    saveToFileSet(set, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "SREM") == 0) {
            Set* set = loadFromFileSet(filename, basename, &pos1, &pos2, &status);
            if (set == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    SREM(set, item);
                    result = malloc(strlen(item) + 5);
                    sprintf(result, "-> %s\n", item);
                    if (status == 2) status = 0;
                    fclose(file);
                    saveToFileSet(set, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "SISMEMBER") == 0) {
            Set* set = loadFromFileSet(filename, basename, &pos1, &pos2, &status);
            if (set == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    result = malloc(40);
                    if (SISMEMBER(set, item)) sprintf(result, "-> True\n");
                    else sprintf(result, "-> False\n");
                    fclose(file);
                }
            }
        }
        if (strcmp(argv[temp], "QPUSH") == 0) {
            Queue* queue = loadFromFileQueue(filename, basename, &pos1, &pos2, &status);
            if (queue == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    QPUSH(queue, item);
                    result = malloc(strlen(item) + 5);
                    sprintf(result, "-> %s\n", item);
                    if (status == 1) status = 0;
                    fclose(file);
                    saveToFileQueue(queue, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "QPOP") == 0) {
            Queue* queue = loadFromFileQueue(filename, basename, &pos1, &pos2, &status);
            if (queue == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    char* element = QPOP(queue);
                    result = malloc(strlen(element) + 5);
                    sprintf(result, "-> %s\n", element);
                    if (status == 2) status = 0;
                    fclose(file);
                    saveToFileQueue(queue, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "HSET") == 0) {
            HashTable* hashtable = loadFromFileTable(filename, basename, &pos1, &pos2, &status);
            if (hashtable == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    HSET(hashtable, item, key);
                    result = malloc(strlen(key) + strlen(item) + 20);
                    sprintf(result, "-> %s %s\n", key, item);
                    if (status == 1) status = 0;
                    fclose(file);
                    saveToFileTable(hashtable, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "HDEL") == 0) {
            HashTable* hashtable = loadFromFileTable(filename, basename, &pos1, &pos2, &status);
            if (hashtable == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    HDEL(hashtable, item);
                    result = malloc(strlen(item) + 5);
                    printf("-> %s\n", item);
                    if (status == 2) status = 0;
                    fclose(file);
                    saveToFileTable(hashtable, filename, basename, &pos1, &pos2, &status);
                }
            }
        }
        if (strcmp(argv[temp], "HGET") == 0) {
            HashTable* hashtable = loadFromFileTable(filename, basename, &pos1, &pos2, &status);
            if (hashtable == NULL) {
                result = malloc(40);
                sprintf(result, "Error when opening a file!\n");
                fclose(file);
            }
            else {
                if (pos1 + pos2 == 0) {
                    result = malloc(40);
                    sprintf(result, "Such a database, alas, does not exist!\n");
                    fclose(file);
                }
                else {
                    result = malloc(40);
                    if (HGET(hashtable, item) != NULL) sprintf(result, "-> True\n");
                    else printf(result, "-> False\n");
                    fclose(file);
                }
            }
        }
    }
    else {
        result = malloc(40);
        sprintf(result, "The file name or operation to be performed is not specified.\n");
    }
    int bytes_sent = send(client_socket, result, strlen(result), 0);
    closesocket(client_socket);
    free(argv);
    free(result);
}

int main() {
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    result = bind(listen_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    result = listen(listen_socket, BACKLOG);
    printf("The server is running and waiting for connections on port %d\n", PORT);
        while (1) {
            SOCKET client_socket = accept(listen_socket, NULL, NULL);
            HANDLE thread = CreateThread(NULL, 0, handle_client, (LPVOID)client_socket, 0, NULL);
            CloseHandle(thread);
        }
    closesocket(listen_socket);
    WSACleanup();
    return 0;
}