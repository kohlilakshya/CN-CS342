#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

#define NUM_FRAMES 10

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP_ADDR> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    int client;
    char buffer[1024];
    struct sockaddr_in server;
    socklen_t addrSize;

    // Initialize random seed
    srand(time(NULL));

    client = socket(AF_INET, SOCK_DGRAM, 0); // UDP
    if (client == -1) {
        perror("Socket creation failure");
        return 1;
    }

    int PORT = atoi(argv[2]);
    server.sin_family = AF_INET; // Address family
    server.sin_port = htons(PORT); // Port in network byte order
    server.sin_addr.s_addr = inet_addr(argv[1]);
    memset(server.sin_zero, '\0', sizeof(server.sin_zero)); // Clear the contiguous memory blocks

    int frame = 1;
    while (frame <= NUM_FRAMES) {
        // 30% probability of corruption
        int num = rand() % 10;
        int count = 0;

        if (num <= 2)
            strcpy(buffer, "error");
        else {
            strcpy(buffer, "frame");
            count++;
        }

        if (sendto(client, buffer, strlen(buffer), 0, (struct sockaddr *) &server, sizeof(server)) == -1) {
            perror("Error in sending");
            close(client);
            return 1;
        } else {
            printf("Sending Frame %d", frame);
            if (frame < 10)
                printf("\t\t");
            else
                printf("\t");

            while (true) {
                socklen_t len = sizeof(server);
                int z = recvfrom(client, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &server, &len);
                if (z == -1) {
                    perror("Error in receiving data");
                    close(client);
                    return 1;
                } else {
                    if (strncmp(buffer, "ACK", 3) == 0) {
                        printf("Acknowledgment received for Frame %d\n", frame);
                        break;
                    } else {
                        printf("Acknowledgment lost or corrupted. Retransmitting Frame %d.\n", frame);
                        printf("Sending Frame %d", frame);
                        if (frame < 10)
                            printf("\t\t");
                        else
                            printf("\t");

                        // 30% probability of corruption
                        num = rand() % 10;
                        if (num <= 2)
                            strcpy(buffer, "error");
                        else {
                            strcpy(buffer, "frame");
                            count++;
                        }

                        if (sendto(client, buffer, strlen(buffer), 0, (struct sockaddr *) &server, sizeof(server)) == -1) {
                            perror("Error in sending");
                            close(client);
                            return 1;
                        }
                    }
                }
            }
        }

        if (count > 0) {
            frame++;
            count = 0;
        }
    }

    close(client);
    return 0;
}
