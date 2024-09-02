#include "base_64_decoder.c"
#include "base_64_encoder.c"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// Function to receive and acknowledge messages from the client
void recvAndAck(int clientFd, struct sockaddr_in *client) {
    char *ip = inet_ntoa(client->sin_addr); // IP address of client
    int port = ntohs(client->sin_port);      // Port of client
    printf("\nNEW CLIENT CONNECTION (%s : %d) ESTABLISHED\n", ip, port);

    char buffer[1500]; 

    // Wait for message from client until a close connection request is received
    while (1) {
        bzero(buffer, 1500); // Clear the buffer

        // Read message from socket
        int bytesRead = read(clientFd, buffer, 1500); 

        // Check if the message type is '3' (close connection)
        if (buffer[0] == '3') {
            break;
        }

        // Check if the message type is '1' (normal message)
        if (buffer[0] == '1') {
            printf("\nMessage received from client %s : %d\n\tEncoded Message: %s\n", ip, port, buffer + 1);
            printf("\tDecoded Message: %s\n", decode(buffer + 1));

            bzero(buffer, 1500);

            buffer[0] = '2'; // Set message type to '2' for acknowledgment
            strcpy(buffer + 1, encode("ACK")); // Append encoded value of "ACK"
            write(clientFd, buffer, strlen(buffer)); // Send acknowledgment
        } else {
            break;
        }
    }

    close(clientFd); // Close connection
    printf("\nCLIENT CONNECTION (%s : %d) CLOSED\n", ip, port);
    exit(0); // Exit the child process
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server, client;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket

    // Check socket creation
    if (serverSocket == -1) {
        printf("\nSOCKET CREATION FAILURE\n");
        exit(0);
    }

    server.sin_family = AF_INET; // Address family - IPv4
    server.sin_addr.s_addr = INADDR_ANY; // Bind to all available local interfaces
    server.sin_port = htons(atoi(argv[1])); // Set server port from command line argument
    memset(&server.sin_zero, 0, sizeof(server.sin_zero)); // Zero out the rest of the struct
    socklen_t length = sizeof(struct sockaddr_in);

    // Bind the socket to the address
    if (bind(serverSocket, (struct sockaddr *)&server, length) < 0) {
        printf("\nBINDING FAILED\n");
        exit(0);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 20) == -1) {
        printf("\nLISTEN FAILED\n");
        exit(0);
    }

    printf("SERVER WORKING\n");
    fflush(stdout);

    while (1) {
        // Accept incoming client connections
        int clientSocket = accept(serverSocket, (struct sockaddr *)&client, &length);

        // Check if connection was successful
        if (clientSocket < 0) {
            printf("\nSERVER-CLIENT CONNECTION COULD NOT BE ESTABLISHED\n");
            exit(0);
        }

        int status = fork(); // Create a child process to handle the client
        switch (status) {
            case -1:
                printf("\nCOULD NOT ESTABLISH CONNECTION\n");
                break;
            case 0: // Child process
                close(serverSocket); // Close the server socket in the child process
                recvAndAck(clientSocket, &client); // Handle client communication
                break;
            default: // Parent process
                close(clientSocket); // Close the client socket in the parent process
        }
    }

    return 0;
}
