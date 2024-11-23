#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "../common/network.h"

#define PORT 5059

int main() {
    SOCKET server_fd = create_server(PORT);
    printf("Load Balancer is running on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        SOCKET client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

        if (client_fd == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        char buffer[1024];
        receive_message(client_fd, buffer, sizeof(buffer));
        printf("Received from client: %s\n", buffer);

        send_message(client_fd, "Message received by Load Balancer.");
        closesocket(client_fd);
    }

    closesocket(server_fd);
    cleanup_winsock();
    return 0;
}
