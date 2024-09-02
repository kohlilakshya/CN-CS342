#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void getInput(char buffer[])
{
    char inp;
    printf("Enter message to send (max 1000 characters):\n");
    bzero(buffer, 1500);
    int i = 0;

    // maximum input size is 1000
    while (1)
    {
        scanf("%c", &inp); // read message character by character
        if (inp == '\n')
            break;
        if (i == 1000)
        {
            printf("Message truncated: Only first 1000 characters are sent.\n");
            break;
        }
        buffer[i++] = inp;
    }
}

void recvAndAck(int c_fd, struct sockaddr_in *client)
{
    char *ip = inet_ntoa(client->sin_addr); // IP address of client
    int port = ntohs(client->sin_port);      // Port of client (convert from network byte order)
    printf("\nNew client connection established: %s:%d\n", ip, port);

    char buffer[1500]; // buffer to store message

    // Wait for message from client until a close connection request is received
    while (1)
    {
        bzero(buffer, 1500);
        int st = read(c_fd, buffer, 1500); // read message from socket

        if (st <= 0) {
            printf("\nClient %s:%d disconnected or error occurred.\n", ip, port);
            break;
        }

        printf("\nMessage received from client %s:%d:\n%s\n", ip, port, buffer); // print received message
        bzero(buffer, 1500);
        getInput(buffer);
        write(c_fd, buffer, strlen(buffer)); // send acknowledgment
    }

    close(c_fd); // close connection
    printf("\nClient connection closed: %s:%d\n", ip, port);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Server_Port_Number>\n", argv[0]);
        return 1;
    }
    struct sockaddr_in server, client;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4, SOCK_STREAM indicates TCP socket

    // Could not create socket
    if (serverSocket == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }
    server.sin_family = AF_INET;            // Address family - IPv4
    server.sin_addr.s_addr = INADDR_ANY;    // Address of host, binds socket to all available local interfaces
    server.sin_port = htons(atoi(argv[1])); // Argument 1 is server port number entered by user
    memset(&server.sin_zero, 0, sizeof(server.sin_zero));
    socklen_t length = sizeof(struct sockaddr_in);

    // If port is already in use, binding fails
    if (bind(serverSocket, (struct sockaddr *)&server, length) < 0)
    {
        perror("Binding failed");
        exit(1);
    }

    // Listen to the current socket
    if (listen(serverSocket, 20) == -1)
    {
        perror("Listening failed");
        exit(1);
    }
    printf("Server is running on port %d.\n", ntohs(server.sin_port));
    fflush(stdout);
    while (1)
    {
        int clientSocket = accept(serverSocket, (struct sockaddr *)&client, &length); // Client tries to connect

        // Client connection was not made
        if (clientSocket < 0)
        {
            perror("Server-client connection could not be established");
            continue;
        }
        int status = fork(); // Fork to create a child process to handle this client, so that multiple clients can be handled concurrently
        switch (status)
        {
        // Error while creating child process
        case -1:
            perror("Fork failed");
            break;
        // Child process
        case 0:
            close(serverSocket);               // Server socket is handled by parent process
            recvAndAck(clientSocket, &client); // This function handles a client
            break;
        // Parent process
        default:
            close(clientSocket); // Client socket is now handled by child process
        }
    }
    return 0;
}
