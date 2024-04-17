#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h> // Include this header for inet_pton and inet_ntop

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib library

#define PORT 12345
#define SERVER_IP "127.0.0.1" // Replace with the actual IP address of the server
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }


    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(PORT); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    while (true) {
        // Get user input
        std::cout << "Enter message to send to server (type 'quit' to exit): ";
        std::cin.getline(buffer, BUFFER_SIZE);

        // Check if user wants to quit
        if (strcmp(buffer, "quit") == 0) {
            break;
        }

        // Send message to server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error sending message to server\n";
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }

        // Receive message from server
        int serverAddrLen = sizeof(serverAddr);
        int recvlen = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, nullptr, nullptr);
        if (recvlen > 0) {
            // Print received message
            buffer[recvlen] = '\0';
            std::cout << "Received message from server: " << buffer << std::endl;
        }
    }

    // Clean up
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
