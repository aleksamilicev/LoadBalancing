#define _WIN32_WINNT 0x0600

#include "network.h"
#include <winsock2.h>
#include <ws2tcpip.h> // Za InetPton
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Dodaj Winsock biblioteku

// Inicijalizacija Winsock-a
void initialize_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

// Kreira TCP server socket i vraća file descriptor
SOCKET create_server(int port) {
    initialize_winsock();

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

// Kreira TCP klijent socket i vraća file descriptor
SOCKET create_client(const char* ip, int port) {
    initialize_winsock();

    SOCKET client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // // Koristi InetPton umesto inet_pton
    // if (InetPton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
    //     fprintf(stderr, "Invalid IP address.\n");
    //     closesocket(client_fd);
    //     exit(EXIT_FAILURE);
    // }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(client_fd);
        exit(EXIT_FAILURE);
    }

    return client_fd;
}

// Šalje poruku preko socket-a
void send_message(SOCKET socket, const char* message) {
    send(socket, message, strlen(message), 0);
}

// Prima poruku preko socket-a
void receive_message(SOCKET socket, char* buffer, size_t buffer_size) {
    memset(buffer, 0, buffer_size);
    recv(socket, buffer, buffer_size, 0);
}

// Čisti Winsock resurse
void cleanup_winsock() {
    WSACleanup();
}
