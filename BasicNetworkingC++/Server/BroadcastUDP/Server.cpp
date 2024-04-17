#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h> // Include this header for inet_pton and inet_ntop

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib library

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return -1;
    }

    // Enable broadcasting
    int enableBroadcast = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&enableBroadcast, sizeof(enableBroadcast)) == SOCKET_ERROR) {
        std::cerr << "Error setting socket option\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.0.0.255", &serverAddr.sin_addr); // Broadcast address
    serverAddr.sin_port = htons(PORT);

    while (true) {
        // Receive data from client
        int clientAddrLen = sizeof(clientAddr);
        int recvlen = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (recvlen > 0) {
            // Print received message
            buffer[recvlen] = '\0';
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Received message from " << clientIP << ": " << buffer << std::endl;

            // Broadcast message to all clients
            sendto(serverSocket, buffer, recvlen, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        }
    }

    // Clean up
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
