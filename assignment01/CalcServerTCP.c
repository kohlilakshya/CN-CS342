#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <math.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <string.h>

#define MAX_STRING_SIZE 100
#define BUFFER_SIZE 1500

char* calculate(double operand1, char operator, double operand2) {
    static char result[MAX_STRING_SIZE]; // Using static to return a pointer to a local variable

    switch (operator) {
        case '+':
            snprintf(result, MAX_STRING_SIZE, "%f", operand1 + operand2);
            break;
        case '-':
            snprintf(result, MAX_STRING_SIZE, "%f", operand1 - operand2);
            break;
        case '*':
            snprintf(result, MAX_STRING_SIZE, "%f", operand1 * operand2);
            break;
        case '/':
            if (operand2 != 0.0) {
                snprintf(result, MAX_STRING_SIZE, "%f", operand1 / operand2);
            } else {
                snprintf(result, MAX_STRING_SIZE, "Error: Division by zero");
            }
            break;
        case '^':
            snprintf(result, MAX_STRING_SIZE, "%f", pow(operand1, operand2));
            break;
        default:
            snprintf(result, MAX_STRING_SIZE, "Error: Invalid operator");
            break;
    }

    return result;
}

void recvAndAck(int c_fd, struct sockaddr_in *client) {
    char *ip = inet_ntoa(client->sin_addr); // IP address of client
    int port = ntohs(client->sin_port); // Port of client
    printf("\nNew client connection (%s : %d) established\n", ip, port);

    char buffer[BUFFER_SIZE]; // Buffer to store sending value

    // Wait for message from client until close connection request is received
    while (1) {
        bzero(buffer, BUFFER_SIZE); 
        int st = read(c_fd, buffer, BUFFER_SIZE - 1); // Read message from socket

        if (st <= 0) {
            break; // Exit loop if no data or connection closed
        }

        // Print received message
        printf("Received from client: %s\n", buffer);

        double operand1, operand2;
        char operator;

        int numScanned = sscanf(buffer, "%lf %c %lf", &operand1, &operator, &operand2);

        if (numScanned != 3) {
            snprintf(buffer, BUFFER_SIZE, "Error: Invalid message format");
            printf("Error: Invalid message format\n");
        } else {
            char* resultString = calculate(operand1, operator, operand2);
            strncpy(buffer, resultString, BUFFER_SIZE - 1);
        }

        buffer[BUFFER_SIZE - 1] = '\0'; // Ensure null-termination

        printf("Sending to client: %s\n", buffer);
        write(c_fd, buffer, strlen(buffer)); // Send result to client
    }

    close(c_fd); // Close connection
    printf("\nClient connection (%s : %d) closed\n", ip, port);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server, client;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket
    
    // Could not create socket
    if (serverSocket == -1) { 
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET; // Address family - IPv4
    server.sin_addr.s_addr = INADDR_ANY; // Bind to all available local interfaces
    server.sin_port = htons(atoi(argv[1])); // Server port number
    memset(&server.sin_zero, 0, sizeof(server.sin_zero)); 
    socklen_t length = sizeof(struct sockaddr_in);

    // Binding to port
    if (bind(serverSocket, (struct sockaddr*) &server, length) < 0) { 
        perror("Binding failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 20) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is working\n");
    fflush(stdout);

    while (1) {
        int clientSocket = accept(serverSocket, (struct sockaddr *) &client, &length); // Accept client connection
        
        // Client connection was not made
        if (clientSocket < 0) { 
            perror("Server-client connection could not be established");
            exit(1);
        }

        int status = fork(); // Fork to create a child process for client handling

        switch (status) {
            // Error while creating child process
            case -1:
                perror("Fork failed");
                break;
            // Child process
            case 0:
                close(serverSocket); // Server socket is handled by parent process
                recvAndAck(clientSocket, &client); // Handle client communication
                break;
            // Parent process
            default:
                close(clientSocket); // Client socket is now handled by child process
        }
    }

    return 0;
}
