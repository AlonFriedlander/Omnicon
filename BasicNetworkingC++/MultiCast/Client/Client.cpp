//#include <iostream>
//#include <WS2tcpip.h>
//
//#pragma comment (lib, "ws2_32.lib")
//
//using namespace std;
//
//int main() {
//    // Initialize Winsock
//    WSADATA data;
//    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
//        cerr << "Error initializing Winsock" << endl;
//        return 1;
//    }
//
//    // Create a socket for sending registration message
//    SOCKET out = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (out == INVALID_SOCKET) {
//        cerr << "Error creating socket" << endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // Set up the server address structure
//    sockaddr_in serverAddr;
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(54000); // Port used by the server
//    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Server IP address
//
//    // Send registration message to the server
//    const char* registrationMessage = "blue"; // Change to "yellow" for group1
//    if (sendto(out, registrationMessage, strlen(registrationMessage), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//        cerr << "Error sending registration message" << endl;
//        closesocket(out);
//        WSACleanup();
//        return 1;
//    }
//
//    // Close the socket used for registration
//    closesocket(out);
//
//    // Create a socket for receiving multicast messages
//    SOCKET in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (in == INVALID_SOCKET) {
//        cerr << "Error creating socket" << endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // Set up the multicast group address structure
//    sockaddr_in multicastAddr;
//    multicastAddr.sin_family = AF_INET;
//    multicastAddr.sin_port = htons(54000); // Port used for multicast
//    inet_pton(AF_INET, "239.0.0.1", &multicastAddr.sin_addr);
//
//    // Bind the socket to the multicast group address
//    if (bind(in, (sockaddr*)&multicastAddr, sizeof(multicastAddr)) == SOCKET_ERROR) {
//        cerr << "Error binding socket" << endl;
//        closesocket(in);
//        WSACleanup();
//        return 1;
//    }
//
//    // Enter a loop to continuously receive messages
//    while (true) {
//        char buffer[1024];
//        sockaddr_in senderAddr;
//        int senderAddrSize = sizeof(senderAddr);
//        int bytesReceived = recvfrom(in, buffer, sizeof(buffer), 0, (sockaddr*)&senderAddr, &senderAddrSize);
//        if (bytesReceived == SOCKET_ERROR) {
//            cerr << "Error receiving data" << endl;
//            continue;
//        }
//
//        // Print the received message
//        buffer[bytesReceived] = '\0'; // Null-terminate the received data
//        cout << "Received multicast message: " << buffer << endl;
//    }
//
//    // Close the socket and cleanup Winsock
//    closesocket(in);
//    WSACleanup();
//
//    return 0;
//}






















#include <iostream>
#include <string>
#include <Winsock2.h> 
#include <Ws2tcpip.h> 

#pragma comment(lib,"WS2_32")

int Port = 8910;
#define UDP_MAX_SIZE 65535
std::string IP = "234.5.6.7";
int optval = 0;

int main()
{
	sockaddr_in AllowAddr;
	ip_mreq JoinReq;

	//WSAStartup
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cout << "WSAStartup function failed with error: <" << iResult << ">\n";
		WSACleanup();
		return -1;
	}
	// Create socket
	SOCKET Socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Can not create socket: <" << WSAGetLastError() << ">\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}

	// Allow reuse of port
	optval = 1;
	if ((setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval))) < 0)
	{
		std::cout << "Socket set SO_REUSEADDR fail\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}

	// Allow any address
	memset(&AllowAddr, 0, sizeof(AllowAddr));
	AllowAddr.sin_family = AF_INET;
	AllowAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	AllowAddr.sin_port = htons(Port);

	// Bind socket
	if (bind(Socket, (struct sockaddr*)&AllowAddr, sizeof(AllowAddr)) < 0) {
		std::cout << "Multicast failed to bind socket\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}

	// Membership setting
	if (inet_pton(AF_INET, (PCSTR)(IP.c_str()), &JoinReq.imr_multiaddr.s_addr) < 0) {
		std::cout << "IP invalid\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}
	// This can be used to restrict to only receive form particular sender
	JoinReq.imr_interface.s_addr = htonl(INADDR_ANY);

	// Join membership
	if ((setsockopt(Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&JoinReq, sizeof(JoinReq))) < 0)
	{
		std::cout << "Multicast join membership fail. Error code: <" << WSAGetLastError() << ">\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}
	char ReceivedIP[46] = { 0 };
	char ReceivedMessage[UDP_MAX_SIZE + 1] = { 0 };

	//Receive
	while (1) {
		sockaddr_in ClientAddr;
		int addrlen = sizeof(ClientAddr);
		int nbytes = recvfrom(
			Socket,
			ReceivedMessage,
			UDP_MAX_SIZE,
			0,
			(struct sockaddr*)&ClientAddr,
			&addrlen
		);
		if (nbytes < 0) {
			std::cout << "Receive fail. Error code: <" << WSAGetLastError() << ">\n";
			closesocket(Socket);
			WSACleanup();
			return -1;
		}
		inet_ntop(AF_INET, &ClientAddr.sin_addr, (PSTR)ReceivedIP, 46);
		std::cout << "Received from: " << ReceivedIP << ", " << ntohs(ClientAddr.sin_port) << "\n";
		std::cout << ReceivedMessage << "\n";
		if (strcmp("bye", ReceivedMessage) == 0) {
			break;
		}
		memset(ReceivedMessage, 0, UDP_MAX_SIZE);
	}
	closesocket(Socket);
	WSACleanup();

	return 1;
}
