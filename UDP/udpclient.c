#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_size = sizeof(server_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set server address and port:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);  // Same port as the server
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)

    // Get input from the user:
    printf("Enter message to server: ");
    fgets(client_message, sizeof(client_message), stdin);
    client_message[strcspn(client_message, "\n")] = 0;  // Remove newline character

    // Send the message to the server:
    if (sendto(socket_desc, client_message, strlen(client_message), 0, (struct sockaddr*)&server_addr, server_size) < 0) {
        printf("Unable to send message\n");
        return -1;
    }

    // Receive the server's response:
    if (recvfrom(socket_desc, server_message, sizeof(server_message), 0, NULL, NULL) < 0) {
        printf("Error while receiving server's message\n");
        return -1;
    }

    printf("Server's response: %s\n", server_message);

    // Close the socket:
    if (close(socket_desc) < 0) {
        printf("Error while closing socket\n");
        return -1;
    }

    return 0;
}