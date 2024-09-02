#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

#define BUFFER_SIZE 1500
#define MAX_MESSAGE_LENGTH 1000

// Function to get input from the user
void getInput(char buffer[]) {
    char inp;
    printf("Write message to send (max %d characters):\n", MAX_MESSAGE_LENGTH);
    bzero(buffer, BUFFER_SIZE);
    int i = 0;

    // Read message character by character
    while (i < MAX_MESSAGE_LENGTH) {
        scanf("%c", &inp);
        if (inp == '\n')
            break;
        buffer[i++] = inp;
    }

    if (i == MAX_MESSAGE_LENGTH) {
        printf("Message truncated to %d characters.\n", MAX_MESSAGE_LENGTH);
    }
}

// Function to close the connection with a specific message
void closeConnection(char buffer[], int clientSocket) {
    snprintf(buffer, BUFFER_SIZE, "connection_close");
    printf("Sending connection close message...\n");
    if (write(clientSocket, buffer, strlen(buffer)) == -1) {
        perror("Failed to send connection close message");
    }
}

int isSocketWritable(int socket) {
    fd_set write_fds;
    struct timeval timeout;
    int retval;

    FD_ZERO(&write_fds);
    FD_SET(socket, &write_fds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    retval = select(socket + 1, NULL, &write_fds, NULL, &timeout);
    if (retval == -1) {
        perror("select() error");
        return 0;
    }
    return retval > 0 && FD_ISSET(socket, &write_fds);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server_IP_Address> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server;
    char inp;                                           // To read user input character by character
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket
    char buffer[BUFFER_SIZE];                          // Buffer to store received and modified sending value

    // Check if the socket creation was successful
    if (clientSocket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET; // Address family - IPv4

    // Set server IP address from user input
    if (inet_aton(argv[1], &server.sin_addr) == 0) {
        fprintf(stderr, "Invalid server IP address: %s\n", argv[1]);
        close(clientSocket);
        return 1;
    }

    int serverPort = atoi(argv[2]); // Server port from user input
    server.sin_port = htons(serverPort);
    socklen_t length = sizeof(struct sockaddr_in);

    // Attempt to connect to the server
    if (connect(clientSocket, (struct sockaddr *)&server, length) == -1) {
        perror("Connection to server failed");
        close(clientSocket);
        return 1;
    }

    while (1) {
        printf("Do you want to send a message?\nPress 'y' for YES or any other key for NO: ");
        scanf(" %c", &inp); // Note the space before %c to ignore any leftover newline character

        // If user chooses not to send a message, close the connection
        if (inp != 'y') {
            closeConnection(buffer, clientSocket);
            break;
        }

        getchar(); // Consume newline character left in the input buffer
        getInput(buffer);

        // Check if the socket is writable before sending
        if (!isSocketWritable(clientSocket)) {
            printf("Socket is not writable. The server may be down.\n");
            break;
        }

        if (write(clientSocket, buffer, strlen(buffer)) == -1) {
            perror("Failed to send message");
            break;
        }

        bzero(buffer, BUFFER_SIZE);
        int received = read(clientSocket, buffer, BUFFER_SIZE); // Receive acknowledgment from server

        if (received == -1) {
            perror("Failed to receive message");
            break;
        } else if (received == 0) {
            // Connection closed by server
            printf("Connection closed by server.\n");
            break;
        }

        // Print the received message from server
        printf("Message received from server (IP: %s, Port: %d):\n", argv[1], serverPort);
        printf("%s\n", buffer);
    }

    close(clientSocket); // Close the socket
    return 0;
}
