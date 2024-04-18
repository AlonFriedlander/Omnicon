//#include <iostream>
//#include <WS2tcpip.h>
//#include <chrono>
//#include <thread>
//
//#pragma comment (lib, "ws2_32.lib")
//
//using namespace std;
//
//SOCKET in;
//sockaddr_in multicastAddr1;
//sockaddr_in multicastAddr2;
//
//// Function to send multicast messages
//void sendMulticastMessages() {
//    // Set up the multicast addresses
//    multicastAddr1.sin_family = AF_INET;
//    multicastAddr1.sin_port = htons(54000); // Port used for multicast 1
//    inet_pton(AF_INET, "239.0.0.1", &multicastAddr1.sin_addr);
//
//    multicastAddr2.sin_family = AF_INET;
//    multicastAddr2.sin_port = htons(54000); // Port used for multicast 2
//    inet_pton(AF_INET, "239.0.0.2", &multicastAddr2.sin_addr);
//
//    // Main loop to send multicast messages
//    while (true) {
//        // Send message to multicast group 1
//        string group1Message = "group1";
//        sendto(in, group1Message.c_str(), group1Message.size() + 1, 0, (sockaddr*)&multicastAddr1, sizeof(multicastAddr1));
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        // Send message to multicast group 2
//        string group2Message = "group2";
//        sendto(in, group2Message.c_str(), group2Message.size() + 1, 0, (sockaddr*)&multicastAddr2, sizeof(multicastAddr2));
//
//        // Sleep for 1 second
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//}
//
//int main() {
//    // Initialize Winsock
//    WSADATA data;
//    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
//        cout << "Error initializing Winsock." << endl;
//        return 1;
//    }
//
//    // Create a UDP socket
//    in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (in == INVALID_SOCKET) {
//        cout << "Error creating socket." << endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // Bind the socket to the server address
//    sockaddr_in serverAddr;
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = INADDR_ANY;
//    serverAddr.sin_port = htons(54000); // Port used for multicast
//    if (bind(in, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//        cout << "Error binding socket." << endl;
//        closesocket(in);
//        WSACleanup();
//        return 1;
//    }
//
//    // Start thread for sending multicast messages
//    thread sender(sendMulticastMessages);
//    sender.detach();
//
//    // Buffer to store received data
//    char buffer[1024];
//
//    // Main server loop
//    while (true) {
//        // Receive data from clients
//        sockaddr_in clientAddr;
//        int clientAddrSize = sizeof(clientAddr);
//        int bytesReceived = recvfrom(in, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrSize);
//        if (bytesReceived == SOCKET_ERROR) {
//            cout << "Error receiving data." << endl;
//            continue;
//        }
//
//        // Process client registration
//        string message(buffer, bytesReceived);
//        if (message == "yellow") {
//            // Add client to multicast group 239.0.0.1
//            if (setsockopt(in, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicastAddr1, sizeof(multicastAddr1)) == SOCKET_ERROR) {
//                cout << "Error joining multicast group 1." << endl;
//            }
//        }
//        else if (message == "blue") {
//            // Add client to multicast group 239.0.0.2
//            if (setsockopt(in, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicastAddr2, sizeof(multicastAddr2)) == SOCKET_ERROR) {
//                cout << "Error joining multicast group 2." << endl;
//            }
//        }
//    }
//
//    // Close the socket and clean up Winsock
//    closesocket(in);
//    WSACleanup();
//
//    return 0;
//}

















#include <iostream>
#include <string>
#include <Winsock2.h> 
#include <Ws2tcpip.h> 

using namespace std;

#pragma comment(lib,"WS2_32")

int Port = 8910;
std::string IP = "234.5.6.7";
int optval = 0;

int main()
{
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
	// An IPV4 udp blocking socket, I will write a tutorial on overlapped socket in the future.
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

	//Set target address 
	sockaddr_in TargetAddr;
	memset(&TargetAddr, 0, sizeof(TargetAddr));
	TargetAddr.sin_family = AF_INET;
	if (inet_pton(AF_INET, (PCSTR)(IP.c_str()), &TargetAddr.sin_addr.s_addr) < 0) {
		std::cout << "Multicast failed set join group\n";
		closesocket(Socket);
		WSACleanup();
		return -1;
	}
	TargetAddr.sin_port = htons(Port);

	std::string SendMessage;
	//Send
	while (1) {
		std::cin >> SendMessage;
		iResult = sendto(
			Socket,
			SendMessage.c_str(),
			SendMessage.length(),
			0,
			(struct sockaddr*)&TargetAddr,
			sizeof(TargetAddr)
		);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Sendto failed with error: <" << WSAGetLastError() << ">\n";
			closesocket(Socket);
			WSACleanup();
		}
		if (SendMessage.compare("bye") == 0) {
			break;
		}
	}

	closesocket(Socket);
	WSACleanup();

	return 1;
}















