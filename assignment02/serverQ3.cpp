#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <vector>
#include <map>
#include <thread>

using namespace std;

map<int, int> clientMap;

void handleClient(int clientSocket);
void closeServer(int serverSocket);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <PORT>" << endl;
        return 1;
    }

    int PORT = stoi(argv[1]);
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024] = {0};

    // Create socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "Error creating server socket" << endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind to the server
    if (::bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Bind failed" << endl;
        close(serverSocket);
        return 1;
    } else {
        cout << "Bound successfully" << endl;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) < 0) { // Max clients here is 10
        cerr << "Listening error" << endl;
        close(serverSocket);
        return 1;
    } else {
        cout << "Listening" << endl;
    }

    // Start server close thread
    thread(closeServer, serverSocket).detach();

    while (true) {
        struct sockaddr_in clientAddress;
        socklen_t addrlen = sizeof(clientAddress);
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addrlen)) < 0) {
            cerr << "Error accepting connection" << endl;
            close(serverSocket);
            return 1;
        } else {
            clientMap[clientSocket]++;
            cout << "Accepted connection from client: " << clientSocket << endl;
        }

        // Start client handler thread
        thread sendThread(handleClient, clientSocket);
        sendThread.detach();

    }

    close(serverSocket);
    return 0;
}

void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            clientMap[clientSocket]--;
            cerr << "Client " << clientSocket << " disconnected!" << endl;
            break;
        }
        buffer[bytesReceived] = '\0';

        string temp = buffer;
        string response = "User: ";
        response += to_string(clientSocket) + ": " + temp.substr(2);

        int flag = buffer[0] - '0';
        if (flag >= 0 && flag <= 9) {
            if (flag == clientSocket) {
                string res1 = "You: " + temp.substr(2);
                send(flag, res1.c_str(), res1.size(), 0);
            } else {
                send(flag, response.c_str(), response.size(), 0);
            }
        } else if (buffer[0] == 'A') {
            for (const auto& entry : clientMap) {
                if (entry.first == clientSocket) {
                    string res1 = "You: " + temp.substr(2);
                    send(entry.first, res1.c_str(), res1.size(), 0);
                } else if (entry.second > 0) {
                    send(entry.first, response.c_str(), response.size(), 0);
                }
            }
        } else {
            string errorMsg = "Invalid message type or client doesn't exist";
            send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
    close(clientSocket);
}

void closeServer(int serverSocket) {
    string inpMessage;
    while (true) {
        cin >> inpMessage;
        if (inpMessage == "/exit") {
            cout << "Exiting..." << endl;
            close(serverSocket);
            exit(0);
        }
    }
}
