#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <serverSocket_Port_Number>\n", argv[0]);
        return 1;
    }

    int PORT = atoi(argv[1]);
    int serverSocket, num = 0;
    struct sockaddr_in server, client;
    struct sockaddr_storage store; // Accommodates all supported protocol-specific address structures
    socklen_t addrSize;

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0); // Socket creation
    if (serverSocket == -1) {
        perror("Socket creation failure");
        return 1;
    }

    server.sin_family = AF_INET; // Address family
    server.sin_port = htons(PORT); // Port in network byte order
    server.sin_addr.s_addr = INADDR_ANY; // Internet address
    memset(server.sin_zero, '\0', sizeof(server.sin_zero)); // Clear the contiguous memory blocks

    // Binding
    if (::bind(serverSocket, (sockaddr *)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        return 1;
    } else {
        printf("Bound successfully\n");
    }

    int frame = 1;
    while (true) {
        sleep(1);
        char buffer[1024];
        socklen_t nlen = sizeof(client);
        ssize_t bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&client, &nlen);
        if (bytesReceived == -1) {
            perror("Error in receiving");
            close(serverSocket);
            return 1;
        }
        
        buffer[bytesReceived] = '\0'; // Null-terminate the buffer
        if (strncmp(buffer, "frame", 5) == 0) {
            printf("Received packet %d successfully", frame);
            if (frame < 10)
                printf("\t");
            else
                printf("\t");

            strcpy(buffer, "ACK");
            printf(" No error in packet. Sending ACK\n");
            num++;
        } else {
            printf("Received packet %d successfully", frame);
            if (frame < 10)
                printf("\t");
            else
                printf("\t");

            strcpy(buffer, "NAK");
            printf(" Error in packet. Sending NAK\n");
        }

        if (sendto(serverSocket, buffer, strlen(buffer), 0, (sockaddr *)&client, nlen) == -1) {
            perror("Error in sending");
            close(serverSocket);
            return 1;
        }

        if (num > 0) {
            frame++;
            num = 0;
        }
    }

    close(serverSocket); // Socket close
    return 0;
}
