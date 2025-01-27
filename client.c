#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/sctp.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_L2TP);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    sctp_sendmsg(sock, "Hello from client", 18, NULL, 0, 0, 0, 0, 0, 0);

    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = sctp_recvmsg(sock, buffer, BUFFER_SIZE, NULL, 0, NULL, NULL);
    if (bytes_received > 0) {
        printf("Received: %s\n", buffer);
    }

    close(sock);
    return 0;
}
