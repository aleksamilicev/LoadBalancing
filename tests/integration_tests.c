#include <stdio.h>
#include <string.h>
#include "common/network.h"

#define LB_PORT 5059
#define WR_PORT 5060

void test_lb_to_worker() {
    SOCKET lb_client_fd = create_client("127.0.0.1", LB_PORT);
    send_message(lb_client_fd, "Test message from client to Load Balancer.");

    char buffer[1024];
    receive_message(lb_client_fd, buffer, sizeof(buffer));
    printf("Load Balancer response: %s\n", buffer);

    closesocket(lb_client_fd);
}

void test_worker_communication() {
    SOCKET worker_client_fd = create_client("127.0.0.1", WR_PORT);
    send_message(worker_client_fd, "Test message from Load Balancer to Worker.");

    char buffer[1024];
    receive_message(worker_client_fd, buffer, sizeof(buffer));
    printf("Worker response: %s\n", buffer);

    closesocket(worker_client_fd);
}

int main() {
    printf("Running integration tests...\n");

    printf("Testing Load Balancer communication:\n");
    test_lb_to_worker();

    printf("Testing Worker communication:\n");
    test_worker_communication();

    cleanup_winsock();
    return 0;
}
