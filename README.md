
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

### Assignment 02: Network Applications and Socket Programming

- **Description**: This assignment involves implementing various network applications using socket programming in C/C++/Java. The tasks include creating a DNS lookup tool, an HTTP web cache with LRU eviction policy, and a chat server with client-client communication capabilities.

- **Objectives**:
  - **Basic DNS Lookup**: Develop a DNS lookup tool that translates domain names into IP addresses using UDP sockets, without relying on external libraries.
  - **HTTP Web Cache**: Implement a web cache that stores recently accessed web pages and follows a least-recently-used (LRU) eviction policy. Use HTTP GET requests and socket programming for this task.
  - **Chat Server with Client-Client Communication**: Create a chat server that supports both broadcasting and private messaging between clients over TCP.

- **Files**:
   Contains the source code for all tasks, including implementations for DNS lookup, HTTP web cache, and chat server/client applications.

**Instructions**:

- After cloning the repository, navigate to the `assignment02` directory:
    ```bash
    cd assignment02
    ```

  - **Question 1: Basic DNS Lookup**
    1. **Compile the code**:
        ```bash
        gcc -o dnsLookup q1.c
        ```
    2. **Run the program**:
        ```bash
        ./dnsLookup
        ```
    3. **Follow the prompts** to input a domain name. The program will establish a UDP connection to the DNS server at 8.8.8.8, send a DNS query, and display the resolved IP address(es).


  - **Question 2: HTTP Web Cache**
    1. **Compile the code**:
        ```bash
        g++ -o q2 q2.cpp
        ```
    2. **Run the program**:
        ```bash
        ./q2
        ```
    3. **Follow the prompts** to enter a URL. The program will fetch the web page using HTTP GET requests, store it in the cache, and display the cache contents, which are managed using an LRU eviction policy.


  - **Question 3: Chat Server with Client-Client Communication**
    1. **Compile the server and client code**:
        ```bash
        g++ -std=c++11 -o client clientQ3.cpp
        g++ -std=c++11 -o server serverQ3.cpp
        ```
    2. **Run the server**:
        ```bash
        ./server <PORT>
        ```
    3. **Run the client**: Run the following in multiple terminal as different users.
        ```bash
        ./client <SERVER_IP> <PORT>
        ```
    4. **Chat:**

        - Type your message into the client terminal and press Enter to send a message.
        - Type `/exit` to exit the chat and close the client connection.
        - To broadcast a message to all clients, type `A` followed by your message.
        - To send a direct message to a specific client, type the client's socket number followed by your message.
    
    5. **Use the client** to connect to the server. You can send messages to all connected clients (broadcast) or to specific clients (private chat). Use `/exit` to terminate the client connection, and the server will notify all clients before shutting down.




## Assignment 03: Routing Protocols, MAC Layer Simulation, and Flow Control

### Description
This assignment involves implementing various network protocols and simulations using C/C++. The tasks include a simplified OSPF routing protocol, a MAC layer simulation using CSMA/CA, and the Stop-and-Wait flow control protocol.

### Objectives

- **Q1: Simplified OSPF Routing Protocol**
  - **Description**: Implement a simplified version of the OSPF (Open Shortest Path First) routing protocol. Build a routing table for a network of routers and simulate packet routing between them.

  - **Instructions**:
    1. **Compile the code**:
        ```bash
        g++ -std=c++11 -o ospf q1.cpp 
        ```
    2. **Run the program**:
        ```bash
        ./ospf
        ```

- **Q2: MAC Layer Simulation with CSMA/CA**
  - **Description**: Simulate a wireless network’s MAC layer using the CSMA/CA (Carrier Sense Multiple Access with Collision Avoidance) protocol.

  - **Instructions**:
    1. **Compile the code**:
        ```bash
        g++ -std=c++11 -o mac q2.cpp
        ```
    2. **Run the program**:
        ```bash
        ./mac
        ```
    3. **Observe**:
        - Review the statistics of successful transmissions, collisions, and backoffs.

- **Q3: Stop-and-Wait Flow Control Protocol**
  - **Description**: Implement the Stop-and-Wait flow control protocol to simulate reliable data transmission.

  - **Files**:
    - `senderQ3.cpp`: Contains the implementation of the sender using Stop-and-Wait protocol.
    - `receiverQ3.cpp`: Contains the implementation of the receiver using Stop-and-Wait protocol.

  - **Instructions**:
    1. **Compile the code**:
        ```bash
        g++ -o sender senderQ3.cpp
        g++ -o receiver receiverQ3.cpp
        ```
    2. **Run the receiver**:
        ```bash
        ./receiver
        ```
    3. **Run the sender**:
        ```bash
        ./sender
        ```
    4. **Observe**:
        - The program will demonstrate the flow control between sender and receiver using the Stop-and-Wait protocol.

## Assignment 4: Network Protocols & Simulations

### Task 1: WFQ Load Balancer

**Objective**: Implement a load balancer using the WFQ algorithm.

**Compile and Execute**:
```bash
g++ -std=c++11 -o q1 q1.cpp
./q1
```

### Task 2: Airport Security Lines

**Objective**: Simulate and optimize airport security screening.

**Compile**:
```bash
g++ -std=c++11 -o q2 q2.cpp
```

**Run**:
```bash
./q2
```
