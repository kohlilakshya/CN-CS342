#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

void getInput(char buffer[]) {
    char inp;
    printf("\nWrite expression to send in the format:\noperand1 operator operand2\nValid operators are + - * / ^.\n");
    bzero(buffer, 1500); 
    int i = 0;

    // Maximum input size is 1000
    while (1) {
        scanf("%c", &inp); // Read message character by character    
        if (inp == '\n') 
            break;
        if (i == 1000) {
            printf("Only the first 1000 characters of the message are being sent.\n");
            break;
        }
        buffer[i++] = inp; 
    }
}

void closeConnection(char buffer[], int clientSocket, struct sockaddr_in *server) {
    strcpy(buffer, "connection_close"); // Append encoded value of "connection_close" after msg type
    printf("Connection closed.\n"); 
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)server, sizeof(*server)); // Write to socket
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server; 
    char inp; // To read user input character by character 
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET is for IPv4, SOCK_DGRAM indicates that UDP socket is created
    char buffer[1500]; // Buffer to store received and modified sending value

    // Socket couldn't be created
    if (clientSocket == -1) {
        printf("Socket creation failed.\n");
        return 0;
    } 

    server.sin_family = AF_INET; // Address family - IPv4

    // Server IP address, inputted by user
    if (inet_aton(argv[1], &server.sin_addr) == 0) { 
        printf("Server IP address error.\n");
        return 0;
    } 

    int serverPort = atoi(argv[2]); // Server port, inputted by user
    server.sin_port = htons(serverPort); 
    socklen_t length = sizeof(struct sockaddr_in); 

    printf("UDP client connected to %s on port %d.\n", argv[1], serverPort);

    while (1) {
        printf("Evaluate expression?\nPress 'y' for YES or any other key for NO and press Enter: \n");
        scanf(" %c", &inp); // Added a space to ignore any leftover newline character

        // If inp is not 'y', send close connection request to server
        if (inp != 'y') {
            printf("\nBye!\n");
            closeConnection(buffer, clientSocket, &server);
            break; 
        }

        getchar(); // Consume the newline character left by previous scanf
        getInput(buffer);
        socklen_t len = sizeof(server);
        // 6 parameters
        sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &server, sizeof(server)); // Write to socket
        bzero(buffer, 1500);
        int received = recvfrom(clientSocket, buffer, 50, MSG_WAITALL, (struct sockaddr *) &server, &len); // Receive acknowledgement from server
        
        printf("Value received from server with IP %s and port %d:\n", argv[1], serverPort); 
        printf("ANS: %s\n", buffer);
    }

    close(clientSocket); // Close the socket

    return 0;
}
