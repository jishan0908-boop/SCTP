#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int listen_sock, conn_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    struct sctp_sndrcvinfo sndrcvinfo;
    int flags;

    // Create an SCTP socket
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listen_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address and port
    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(listen_sock);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(listen_sock, 5) < 0) {
        perror("Listen failed");
        close(listen_sock);
        return 1;
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);
    
    // Accept an incoming connection
    conn_sock = accept(listen_sock, NULL, NULL);
    if (conn_sock < 0) {
        perror("Accept failed");
        close(listen_sock);
        return 1;
    }

    memset(buffer, 0, BUFFER_SIZE);

    // Receive data using SCTP
    int bytes_received = sctp_recvmsg(conn_sock, buffer, BUFFER_SIZE, NULL, 0, &sndrcvinfo, &flags);
    if (bytes_received > 0) {
        printf("Received: %s\n", buffer);

        // Send a response using SCTP
        const char *response = "Hello from server";
        sctp_sendmsg(conn_sock, (void *)response, strlen(response), NULL, 0, 0, 0, 0, 0, 0);
    } else {
        perror("Failed to receive data");
    }

    // Close the sockets
    close(conn_sock);
    close(listen_sock);

    return 0;
}
