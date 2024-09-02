#include "base_64_decoder.c"
#include "base_64_encoder.c"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void getInput(char buffer[]) {
    char inp;
    printf("Write message to send\n");
    bzero(buffer, 1500);
    int i = 0;

    // Maximum input size is 1000 characters
    while (1) {
        scanf("%c", &inp); // Read message character by character    
        if (inp == '\n')
            break;
        if (i == 1000) {
            printf("Only the first 1000 characters of the message are being sent\n");
            break;
        }
        buffer[i++] = inp;
    }
}

void closeConnection(char buffer[], int clientSocket) {
    buffer[0] = '3'; // Set message type
    strcpy(buffer + 1, encode("connection_close")); // Append encoded value of "connection_close" after message type
    printf("CONNECTION CLOSED\n");
    write(clientSocket, buffer, strlen(buffer)); // Write to socket
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server_IP_Address> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server;
    char inp;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket

    // Socket creation failure
    if (clientSocket == -1) {
        printf("SOCKET CREATION FAILURE\n");
        return 0;
    }

    char buffer[1500]; // Buffer to store receiving and modified sending values

    server.sin_family = AF_INET; // Address family - IPv4

    // Server IP address inputted by the user
    if (inet_aton(argv[1], &server.sin_addr) == 0) {
        printf("SERVER IP ADDRESS ERROR\n");
        exit(0);
    }

    int serverPort = atoi(argv[2]); // Server port inputted by the user

    // Convert host byte order to network byte order
    server.sin_port = htons(serverPort);
    socklen_t length = sizeof(struct sockaddr_in);

    // Connection establishment failure
    if (connect(clientSocket, (struct sockaddr *)&server, length) == -1) {
        printf("COULDN'T CONNECT TO THE SERVER\n");
        exit(0);
    }

    while (1) {
        printf("Send message?\nPress 'y' for YES or any other key for NO and press enter: \n");
        scanf("%c", &inp);

        // 'n' implies send close connection request to server
        if (inp != 'y') {
            closeConnection(buffer, clientSocket);
            break;
        }

        getchar(); // Clear the newline character left by scanf
        getInput(buffer); // Get input message

        strcpy(buffer + 1, encode(buffer)); // Append encoded value of message after message type
        buffer[0] = '1'; // Set message type as 1
        write(clientSocket, buffer, strlen(buffer)); // Write to socket

        bzero(buffer, 1500);
        int received = read(clientSocket, buffer, 50); // Receive acknowledgment from server
        strcpy(buffer + 1, decode(buffer + 1)); // Decode message

        // Message type 2 indicates acknowledgment
        if (buffer[0] != '2') {
            printf("Acknowledgment could not be received.\nResend the message.\n");
            continue;
        }

        printf("Message received from server with IP %s and port %d\n", argv[1], serverPort);
        printf("%s\n", buffer + 1);
    }

    close(clientSocket); // Close the socket

    return 0;
}
