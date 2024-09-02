#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

void getInput(char buffer[]) {
    char inp;
    printf("Write message to send:\n");
    bzero(buffer, 1500); 
    int i = 0;
    
    // Maximum input size is 1000
    while (1) {
        scanf("%c", &inp); // Read message character by character    
        if (inp == '\n') 
            break;
        if (i == 1000) {
            printf("Only the first 1000 characters of the message will be sent.\n");
            break;
        }
        buffer[i++] = inp; 
    }
}

void closeConnection(char buffer[], int clientSocket) {
    strcpy(buffer, "connection_close"); // Append encoded value of "connection_close" after msg type
    printf("Connection closed.\n"); 
    write(clientSocket, buffer, strlen(buffer)); // Write to socket
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server_IP_Address> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server; 
    char inp; // To read user input character by character 
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET is for IPv4, SOCK_STREAM indicates that TCP socket is created
    char buffer[1500]; // Buffer to store receiving and modified sending value
    
    // Socket couldn't be created
    if (clientSocket == -1) {
        printf("Socket creation failed.\n");
        return 0;
    } 

    server.sin_family = AF_INET; // Address family - IPv4

    // Server IP address, inputted by user
    if (inet_aton(argv[1], &server.sin_addr) == 0) { 
        printf("Invalid server IP address.\n");
        return 0;
    } 

    int serverPort = atoi(argv[2]); // Server port, inputted by user
    server.sin_port = htons(serverPort); 
    socklen_t length = sizeof(struct sockaddr_in); 

    // Connection establishment failure
    if (connect(clientSocket, (struct sockaddr *) &server, length) == -1) { 
        printf("Couldn't connect to the server.\n");
        exit(0);
    } 

    printf("TCP client connected to %s on port %d.\n", argv[1], serverPort);

    while (1) {
        printf("Evaluate expression?\nPress 'y' for YES or any other key for NO and press Enter: \n");
        scanf("%c", &inp);

        // If input is not 'y', send close connection request to server
        if (inp != 'y') {
            closeConnection(buffer, clientSocket);
            break; 
        }

        getchar(); // To consume the newline character left by scanf
        getInput(buffer);

        write(clientSocket, buffer, strlen(buffer)); // Write to socket
        bzero(buffer, 1500);
        int received = read(clientSocket, buffer, 50); // Receive acknowledgement from server

        printf("Value received from server (IP: %s, Port: %d):\n", argv[1], serverPort); 
        printf("ANS: %s\n", buffer);
    }

    close(clientSocket); // Close the socket

    return 0;
}
