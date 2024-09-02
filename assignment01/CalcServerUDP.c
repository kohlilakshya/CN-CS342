#include <unistd.h> 
#include <stdio.h> 
#include <math.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

double calculate(double operand1, char operator, double operand2) {
    switch (operator) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0.0) {
                return operand1 / operand2;
            } else {
                return NAN; // Use NaN to indicate an error
            }
        case '^':
            return pow(operand1, operand2); 
        default:
            return NAN; // Use NaN to indicate an error
    }
}

void recvAndAck(int c_fd, struct sockaddr_in *client) {
    char *ip = inet_ntoa(client->sin_addr); // IP address of client
    int port = ntohs(client->sin_port); // Port of client
    printf("\nNew client connection established\n");

    char buffer[1500]; // Buffer to store sending value
    socklen_t len = sizeof(*client);

    // Wait for message from client until close connection request is received
    while (1) {
        memset(buffer, 0, sizeof(buffer)); 
        int st = recvfrom(c_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *) client, &len); // Read message from socket

        if (st < 0) {
            perror("recvfrom failed");
            close(c_fd);
            exit(EXIT_FAILURE);
        }

        printf("Received from client: %s\n", buffer);
        
        // Process the received message
        double operand1, operand2, result;
        char operator;

        int numScanned = sscanf(buffer, "%lf %c %lf", &operand1, &operator, &operand2);

        if (numScanned != 3) {
            snprintf(buffer, sizeof(buffer), "Error: Invalid input format");
            sendto(c_fd, buffer, strlen(buffer), 0, (struct sockaddr *) client, len); // Send error message to client
            continue; // Continue to the next iteration
        }

        result = calculate(operand1, operator, operand2);

        memset(buffer, 0, sizeof(buffer));

        if (isnan(result)) {
            snprintf(buffer, sizeof(buffer), "Error: Invalid operator or division by zero");
        } else {
            snprintf(buffer, sizeof(buffer), "%lf", result);
        }

        printf("Sending to client: %s\n", buffer);
        sendto(c_fd, buffer, strlen(buffer), 0, (struct sockaddr *) client, len); // Send result to client
    }

    close(c_fd); // Close connection
    printf("\nClient connection (%s : %d) closed\n", ip, port);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server, client;
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket

    if (serverSocket == -1) { 
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;  // Address family - IPv4
    server.sin_addr.s_addr = INADDR_ANY; // Bind to all available local interfaces
    server.sin_port = htons(atoi(argv[1]));  // Server port number entered by user
    memset(&server.sin_zero, 0, sizeof(server.sin_zero));

    if (bind(serverSocket, (struct sockaddr*) &server, sizeof(server)) < 0) { 
        perror("Binding failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server is working\n");
    fflush(stdout);

    recvAndAck(serverSocket, &client);        

    close(serverSocket); // Close the server socket
    return 0;   
}
