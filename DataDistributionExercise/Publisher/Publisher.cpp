#include "Publisher.h"

// Constructor
Publisher::Publisher(int port) : running(true) {
    // Initialize Winsock
    std::cout << "publisher ctor" << std::endl;
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::runtime_error("Error initializing Winsock");
    }

    // Create UDP socket
    if ((socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }

    // Initialize server address
    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind the socket to the specified port
    if (bind(socketDescriptor, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(socketDescriptor);
        WSACleanup();
        throw std::runtime_error("Error binding socket");
    }

    // Initialize map and list of subscribers
    initializeMapAndList();
}

// Destructor
Publisher::~Publisher() {
    stopPublishing();
    WSACleanup();
}

// Starts the publishing process
void Publisher::startPublishing() {
    std::thread listenerThread(&Publisher::subscriberRegistrar, this);
    std::thread eventManagerThread(&Publisher::eventManager, this);

    listenerThread.detach();
    eventManagerThread.detach();
}

// Stops the publishing process
void Publisher::stopPublishing() {
    running = false;
    closesocket(socketDescriptor);
}

// Internal: Initializes the map and list of subscribers
void Publisher::initializeMapAndList() {
    for (ShapeType shapeType : {ShapeType::SQUARE, ShapeType::CIRCLE}) {
        SubscriberShape subscriberShape(shapeTypeToString(shapeType), getFrequency(shapeType));
        subscribersList.push_back(subscriberShape);
        map.insert(std::make_pair(shapeTypeToString(shapeType), subscriberShape));
    }
}

// Internal: Gets the frequency for a given shape type
int Publisher::getFrequency(ShapeType shapeType) const {
    switch (shapeType) {
    case ShapeType::SQUARE: return 2; // 2 Hz
    case ShapeType::CIRCLE: return 3; // 3 Hz
    default: return 0;
    }
}

// Internal: Converts ShapeType enum value to string
std::string Publisher::shapeTypeToString(ShapeType shapeType) const {
    switch (shapeType) {
    case ShapeType::SQUARE: return "SQUARE";
    case ShapeType::CIRCLE: return "CIRCLE";
    default: return "";
    }
}

// Internal: Manages the scheduling and sending shapes to subscribers
void Publisher::eventManager() {
    int counter = 0;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100 milliseconds
        ++counter;
        sendScheduledTasks(counter);
    }
}

// Internal: Sends shapes to subscribers based on the counter value and frequency
void Publisher::sendScheduledTasks(int counter) {
    for (SubscriberShape& subscribers : subscribersList) {
        if (subscribers.checkUpdateTime(counter)) {
            
            std::cout << subscribers.specificTypeList.size()<< std::endl;

            sendToSubscriber(subscribers);
        }
    }
}

// Internal: Sends a shape to a subscriber
void Publisher::sendToSubscriber(SubscriberShape& subscriberShape) {
    std::string shapeString;
    Shape *shape = generateShape(subscriberShape.shapeType);
    if (subscriberShape.shapeType == "SQUARE") {
        shapeString = generateSquareString(shape);
    }
    else {
        shapeString = generateCircleString(shape);
    }

    //std::cout << subscriberShape.specificTypeList.size()<< std::endl;



    for (const SendingInfo& sendingInfo : subscriberShape.specificTypeList) {
        
        std::cout << "are we arrive to sendshapestring??" << std::endl;

        sendShapeString(shapeString, sendingInfo);
    }
    delete shape; // Remember to delete dynamically allocated shape
}

// Internal: Generates a random shape based on the given shape type
Shape* Publisher::generateShape(std::string& shapeType) {
    int size = rand() % 101; // Random size between 1 and 100
    std::vector<int> coordinates(3);
    for (int i = 0; i < 3; ++i) {
        coordinates[i] = rand() % 1500; // Random coordinate
    }
    if (shapeType == "SQUARE") {
        return new Square(size, coordinates);
    }
    else {
        return new Circle(size, coordinates);
    }
}

//// Overload the << operator for Shape
//std::ostream& operator<<(std::ostream& os, const Shape& shape) {
//    shape.printProperties(os); // Assuming printProperties() prints the shape's properties to the output stream
//    return os;
//}

// Internal: Sends a string representation of a shape to a subscriber
void Publisher::sendShapeString(const std::string& shapeString, const SendingInfo& sendingInfo) {
    sendto(socketDescriptor, shapeString.c_str(), shapeString.length(), 0, reinterpret_cast<const sockaddr*>(&sendingInfo.address), sizeof(sendingInfo.address));
}

// Internal: Generates a string representation of a square
std::string Publisher::generateSquareString(const Shape* shape) {
    std::stringstream ss;
    const Square* square = dynamic_cast<const Square*>(shape);
    if (square) {
        ss << "Square - Color: blue, Size: " << square->getSize() << ", Coordinates: ";
        const std::vector<int>& coordinates = square->getCoordinates();
        for (int coord : coordinates) {
            ss << coord << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

// Internal: Generates a string representation of a circle
std::string Publisher::generateCircleString(const Shape* shape) {
    std::stringstream ss;
    const Circle* circle = dynamic_cast<const Circle*>(shape);
    if (circle) {
        ss << "Circle - Color: green, Size: " << circle->getSize() << ", Coordinates: ";
        const std::vector<int>& coordinates = circle->getCoordinates();
        for (int coord : coordinates) {
            ss << coord << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

// Internal: Listens for new subscribers and manages their subscription
void Publisher::subscriberRegistrar() {
    while (running) {
        char receiveData[1024];
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        int bytesReceived = recvfrom(socketDescriptor, receiveData, 1024, 0, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (bytesReceived != SOCKET_ERROR) {
            std::string message(receiveData, bytesReceived);
            SendingInfo sendingInfo(clientAddress, ntohs(clientAddress.sin_port));
            
            // Iterate over subscribers to find matching shape type
            for (SubscriberShape& subscriber : subscribersList) {
                if (subscriber.shapeType == message) {
                    subscriber.specificTypeList.push_back(sendingInfo);
                    std::cout << "Registered subscriber for shape type: " << message << std::endl;
                    std::cout << "Total subscribers for " << message << ": " << subscriber.specificTypeList.size() << std::endl;
                    break; // Break the loop after finding the matching subscriber
                }
            }
        }
    }
}
