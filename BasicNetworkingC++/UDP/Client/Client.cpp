#include <iostream>
#include <string>
#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;

void main()
{
    /////////////////////////////////////////////////////////////////
    // INITIALIZE WINSOCK
    /////////////////////////////////////////////////////////////////

    // Structure to store the WinSock version. This is filled in
    // on the call to WSAStartup()
    WSADATA data;

    // To start WinSock, the required version must be passed to
    // WSAStartup(). This server is going to use WinSock version
    // 2 so I create a word that will store 2 and 2 in hex i.e.
    // 0x0202
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        cout << "Can't start Winsock! " << wsOk;
        return;
    }

    /////////////////////////////////////////////////////////////////
    // CONNECT TO THE SERVER
    /////////////////////////////////////////////////////////////////

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    // Loop to continuously send messages
    while (true)
    {
        // Prompt the user for input
        cout << "Enter a message ('quit' to exit): ";
        string message;
        getline(cin, message);

        // Check if the user wants to quit
        if (message == "quit")
        {
            break;
        }

        // Write out to that socket
        int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(server));

        if (sendOk == SOCKET_ERROR)
        {
            cout << "Error sending message! " << WSAGetLastError() << endl;
        }

        // Wait for response from the server
        char buf[1024];
        ZeroMemory(buf, 1024);
        int bytesReceived = recvfrom(out, buf, 1024, 0, nullptr, nullptr);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "Error receiving message! " << WSAGetLastError() << endl;
            continue;
        }

        // Print the response from the server
        cout << "Server response: " << buf << endl;
    }

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();
}
