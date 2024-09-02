
# CS342: Computer Networks Lab

Welcome to my repository for the CS342: Computer Networks course! This repository showcases practical implementations of key networking concepts, including TCP and UDP client-server communication, DNS lookups, a basic HTTP web cache with LRU policy, client-client chat server functionality, and simulations of OSPF routing and MAC layer behavior using CSMA/CA.

## Repository Structure

The repository is organized into directories for each lab, with each directory containing relevant code, configuration files, and documentation for that specific lab.

## Assignments

### Assignment 01: Socket Programming

- **Description**: This assignment involves socket programming in C/C++. It includes implementing a TCP server-client communication system and handling multiple clients. The assignment also covers creating a simple chat server and client, as well as a network calculator using both TCP and UDP protocols.

- **Objectives**:
  - **Concurrent Base64 Encoding Communication**: Implement a TCP-based communication protocol where the server decodes Base64 encoded messages from multiple clients and sends acknowledgments.
  - **Chat Server and Client**: Develop a chat application that supports multiple clients, allows message exchange, and ensures graceful exit.
  - **Network Calculator**: Create a TCP and UDP calculator where the client sends arithmetic expressions to the server, which evaluates and returns the results.

- **Files**:
   Contains the source code for all questions, including TCP and UDP implementations for calculator, chat server/client, and Base64 encoding/decoding.


**Instructions**:

- After cloning the repository, change the directory to `assignment01`:
    ```bash
    cd assignment01
    ```
  - **Question1**:
    1. Compile the client and server code:
        ```bash
        gcc -o ClientTCP ClientTCP.c
        gcc -o ServerTCP ServerTCP.c
        ```
    2. Open a terminal and run the server with:
        ```bash
        ./ServerTCP <Server_Port_Number>
        ```
    3. Open a new terminal and run the client with:
        ```bash
        ./ClientTCP <Server_IP_Address> <Server_Port_Number>
        ```

  - **Question2**:
    1. Compile the client and server code:
        ```bash
        gcc -o client ClientQ2.c
        gcc -o server ServerQ2.c
        ```

    2. Open a terminal and run the server with:
        ```bash
        ./server <Server_Port_Number>
        ```

    3. Open a new terminal and run the client with:
        ```bash
        ./client <Server_IP_Address> <Server_Port_Number>
        ```

  - **Question3**:
    - **For TCP:**
      - Compile the client and server code for TCP:
          ```bash
          g++ CalcClientTCP.c -o CalcClientTCP
          g++ CalcServerTCP.c -o CalcServerTCP
          ```

      - Open a terminal and run the server with:
          ```bash
          ./CalcServerTCP <Server_Port_Number>
          ```

      - Open a new terminal and run the client with:
          ```bash
          ./CalcClientTCP <Server_IP_Address> <Server_Port_Number>
          ```
      - Once the client is connected, you can enter expressions in the format:
          ```
          operand1 operator operand2
          ```
          Valid operators are `+`, `-`, `*`, `/`, and `^`.

    - **For UDP:**
      - Compile the client and server code for UDP:
        ```bash
        g++ CalcClientUDP.c -o CalcClientUDP 
        g++ CalcServerUDP.c -o CalcServerUDP 
        ```

      - Open a terminal and run the server with:
          ```bash
          ./CalcServerUDP <Server_Port_Number>
          ```

      - Open a new terminal and run the client with:
          ```bash
          ./CalcClientUDP <Server_IP_Address> <Server_Port_Number>
          ```

      - Once the client is connected, you can enter expressions in the format:
          ```
          operand1 operator operand2
          ```
          Valid operators are `+`, `-`, `*`, `/`, and `^`.