#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "../common/network.h"

#define PORT 5060

int main() {
    SOCKET server_fd = create_server(PORT);
    printf("Worker is running on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in lb_addr;
        int addr_len = sizeof(lb_addr);
        SOCKET lb_fd = accept(server_fd, (struct sockaddr*)&lb_addr, &addr_len);

        if (lb_fd == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        char buffer[1024];
        receive_message(lb_fd, buffer, sizeof(buffer));
        printf("Received from Load Balancer: %s\n", buffer);

        send_message(lb_fd, "Message received by Worker.");
        closesocket(lb_fd);
    }

    closesocket(server_fd);
    cleanup_winsock();
    return 0;
}
