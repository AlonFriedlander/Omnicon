#include "Subscriber.h"

std::string multicastSendingGroup = "234.5.6.7"; 
std::string multicastReceivingGroup = "234.5.6.8"; 
int multicastSendingPort = 8910; 
int multicastReceivingPort = 8911;

Subscriber::Subscriber(const std::string& name, int portNum)
    : subscriberName(name), portNumber(portNum) {
    
    // Initialize Winsock
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::runtime_error("Error initializing Winsock");
    }

    createSockets();

    std::thread receiveThread2(&Subscriber::receiveUnicastData, this);
    receiveThread2.detach();
}

Subscriber::~Subscriber() {
    // Clean up resources
    closesocket(sendSocketDescriptor);
    closesocket(recvSocketDescriptor);
    closesocket(unicastSocket);
    WSACleanup();
}

void Subscriber::createSockets() {
    //Create a UDP socket for sending multicast
    sendSocketDescriptor = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
    if (sendSocketDescriptor == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }

    // Allow multiple sockets to use the same PORT number
    int optval = 1;
    if ((setsockopt(sendSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval))) < 0)
    {
        closesocket(sendSocketDescriptor);
        WSACleanup();
        throw std::runtime_error("Error setting socket options");
    }

    multicastSendingAddr;
    memset(&multicastSendingAddr, 0, sizeof(multicastSendingAddr));
    multicastSendingAddr.sin_family = AF_INET;
    inet_pton(AF_INET, (PCSTR)(multicastSendingGroup.c_str()), &multicastSendingAddr.sin_addr.s_addr);
    multicastSendingAddr.sin_port = htons(multicastSendingPort);

    //................................................
    // Create UDP socket for recive multicast
    if ((recvSocketDescriptor = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0)) == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }

    // Allow multiple sockets to use the same PORT number
    if (setsockopt(recvSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval)) == SOCKET_ERROR) {
        closesocket(recvSocketDescriptor);
        WSACleanup();
        throw std::runtime_error("Error setting socket options");
    }

    // Set up the address structure for binding
    sockaddr_in multicastReceivingAddr;
    memset(&multicastReceivingAddr, 0, sizeof(multicastReceivingAddr));
    multicastReceivingAddr.sin_family = AF_INET;
    multicastReceivingAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    multicastReceivingAddr.sin_port = htons(multicastReceivingPort);

    // Bind the socket to the specified port
    if (bind(recvSocketDescriptor, reinterpret_cast<const sockaddr*>(&multicastReceivingAddr), sizeof(multicastReceivingAddr)) == SOCKET_ERROR) {
        closesocket(recvSocketDescriptor);
        WSACleanup();
        throw std::runtime_error("Error binding socket");
    }
    
    // Join the multicast group
    ip_mreq multicastRequest;
    inet_pton(AF_INET, (PCSTR)(multicastReceivingGroup.c_str()), &multicastRequest.imr_multiaddr.s_addr);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(recvSocketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&multicastRequest), sizeof(multicastRequest)) == SOCKET_ERROR) {
        closesocket(recvSocketDescriptor);
        WSACleanup();
        throw std::runtime_error("Error joining multicast group");
    }
    
    //................................................
    // Create a socket for receiving unicast UDP
    unicastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (unicastSocket == INVALID_SOCKET) {
        std::cerr << "Error creating unicast socket: " << WSAGetLastError() << std::endl;
        return;
    }

    // Create a server hint structure for the server
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
    serverHint.sin_family = AF_INET; // Address format is IPv4
    serverHint.sin_port = htons(portNumber); // Convert from little to big endian

    //Try and bind the socket to the IP and port
    if (bind(unicastSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
        return;
    }
}

void Subscriber::subscribe(ShapeType shapeType, const std::string& publisherAddress) {
    flag = true;

    // Start the thread to receive data
    std::thread receiveThread(&Subscriber::registerToPublisher, this);
    receiveThread.detach();
    
    // Create the message string in the format "SHAPE_TYPE:PORT_NUMBER"
    std::string data = shapeTypeToString(shapeType) + ":" + std::to_string(portNumber);

    // Loop until approved
    while (flag) {
        std::cout << "sending multicast" << std::endl;
        int iResult = sendto(sendSocketDescriptor, data.c_str(), data.size(), 0, reinterpret_cast<const sockaddr*>(&multicastSendingAddr), sizeof(multicastSendingAddr));
        
        if (iResult == SOCKET_ERROR) {
            std::cout << "Sendto failed with error: <" << WSAGetLastError() << ">\n";
            closesocket(sendSocketDescriptor);
            WSACleanup();
        }

        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
}


std::string Subscriber::shapeTypeToString(ShapeType shapeType) {
    switch (shapeType) {
    case ShapeType::CIRCLE:
        return "CIRCLE";
    case ShapeType::SQUARE:
        return "SQUARE";
    default:
        throw std::invalid_argument("Invalid shape type");
    }
}


void Subscriber::registerToPublisher() {
    char receiveData[1024];
    sockaddr_in senderAddress;
    int senderAddressSize = sizeof(senderAddress);

    // Loop until registration is successful
    while (flag) {
        int bytesReceived = recvfrom(recvSocketDescriptor, receiveData, sizeof(receiveData), 0, reinterpret_cast<sockaddr*>(&senderAddress), &senderAddressSize);

        if (bytesReceived != SOCKET_ERROR) {
            // Print the received data
            std::string receivedData(receiveData, bytesReceived);
            std::cout << "Received registration data: " << receivedData << std::endl;

            // Check if received "approved" message
            if (receivedData == "approved") {
                flag = false;
                break;
            }
        }
    }
}

void Subscriber::receiveUnicastData() {
    char receiveData[1024];
    sockaddr_in senderAddress;
    int senderAddressSize = sizeof(senderAddress);

    while (true) {
        int bytesReceived = recvfrom(unicastSocket, receiveData, sizeof(receiveData), 0, reinterpret_cast<sockaddr*>(&senderAddress), &senderAddressSize);
        
        if (bytesReceived != SOCKET_ERROR) {
            // Print the received shape string
            std::string shapeString(receiveData, bytesReceived);
            std::cout << "subscriber: " << subscriberName << " get : " << shapeString << std::endl;
        }
    }
    closesocket(unicastSocket);
}

