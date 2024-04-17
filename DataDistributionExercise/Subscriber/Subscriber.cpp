#include "Subscriber.h"


Subscriber::Subscriber(const std::string& name) : subscriberName(name) {
    // Initialize Winsock
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::runtime_error("Error initializing Winsock");
    }

    // Create a UDP socket
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketDescriptor == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }
}

Subscriber::~Subscriber() {
    // Clean up resources
    closesocket(socketDescriptor);
    WSACleanup();
}

void Subscriber::subscribe(ShapeType shapeType, const std::string& publisherAddress, int publisherPort) {
    // Serialize the shape type (assuming it's a simple conversion to string)
    std::string data = shapeTypeToString(shapeType);

    // Set up the address structure of the publisher
    SOCKADDR_IN publisherAddr;
    publisherAddr.sin_family = AF_INET;
    publisherAddr.sin_port = htons(publisherPort);
    inet_pton(AF_INET, publisherAddress.c_str(), &publisherAddr.sin_addr);
    
    std::cout << "are we arrive here" << std::endl;

    // Send the subscription request to the publisher
    sendto(socketDescriptor, data.c_str(), data.size(), 0, (sockaddr*)&publisherAddr, sizeof(publisherAddr));
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

void Subscriber::receiveData() {
    while (true) {
        // Logic for receiving data
        char receiveData[1024];
        sockaddr_in senderAddress;

        std::cout << "are we arrive to receive data?" << std::endl;


        int senderAddressSize = sizeof(senderAddress);
        int bytesReceived = recvfrom(socketDescriptor, receiveData, sizeof(receiveData), 0, reinterpret_cast<sockaddr*>(&senderAddress), &senderAddressSize);
        if (bytesReceived != SOCKET_ERROR) {
            // Print the received shape string
            std::string shapeString(receiveData, bytesReceived);
            std::cout << shapeString << std::endl;
        }
    }
}

//std::istream& operator>>(std::istream& is, Shape& shape) {
//    // Read the binary data from the input stream into the shape object
//    is.read(reinterpret_cast<char*>(&shape), sizeof(shape));
//
//    // Check if the read operation was successful
//    if (!is) {
//        // Handle error if read operation failed
//        // For example:
//        throw std::runtime_error("Failed to read shape data from input stream");
//    }
//
//    return is;
//}
//
//
//// Deserialize the received data into the shape object
//void Subscriber::deserializeShapeData(const char* data, int dataSize, Shape& shape) const {
//    // Deserialize logic here based on serialization format
//    std::istringstream inputStream(std::string(data, dataSize));
//    inputStream >> shape;
//}


int main() {
    Subscriber subscriber("ExampleSubscriber");
    subscriber.subscribe(ShapeType::CIRCLE, "127.0.0.1", 11111); // Example subscription
    subscriber.receiveData(); // Start receiving data

    std::this_thread::sleep_for(std::chrono::seconds(50)); // Sleep for 10 seconds
    return 0; // Return success
}