#include "Publisher.h"

std::string multicastReceivingGroup = "234.5.6.7";
std::string multicastSendingGroup = "234.5.6.8";
int multicastReceivingPort = 8910;
int multicastSendingPort = 8911;

// Constructor
Publisher::Publisher() : running(true) {
    // Initialize Winsock
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::runtime_error("Error initializing Winsock");
    }

    createSockets();

    functionMap["size"] = std::bind(&Publisher::generateSize, this);
    functionMap["coordinates"] = std::bind(&Publisher::generateCoordinates, this);
    functionMap["colors"] = std::bind(&Publisher::generateColors, this);


    // Initialize list of subscribers
    initializeList();
}

// Destructor
Publisher::~Publisher() {
    stopPublishing();
    WSACleanup();
}

void Publisher::createSockets() {
    // Create UDP socket
    if ((multicastSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0)) == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }

    // Allow multiple sockets to use the same PORT number
    int yes = 1;
    if (setsockopt(multicastSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes)) == SOCKET_ERROR) {
        closesocket(multicastSocket);
        WSACleanup();
        throw std::runtime_error("Error setting socket options");
    }

    multicastSendingAddr;
    memset(&multicastSendingAddr, 0, sizeof(multicastSendingAddr));
    multicastSendingAddr.sin_family = AF_INET;
    inet_pton(AF_INET, (PCSTR)(multicastSendingGroup.c_str()), &multicastSendingAddr.sin_addr.s_addr);
    multicastSendingAddr.sin_port = htons(multicastSendingPort);

    // Set up the address structure for binding
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(multicastReceivingPort);

    // Bind the socket to the specified port
    if (bind(multicastSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(multicastSocket);
        WSACleanup();
        throw std::runtime_error("Error binding socket");
    }

    // Join the multicast group
    ip_mreq multicastRequest;
    inet_pton(AF_INET, (PCSTR)(multicastReceivingGroup.c_str()), &multicastRequest.imr_multiaddr.s_addr);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(multicastSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&multicastRequest), sizeof(multicastRequest)) == SOCKET_ERROR) {
        closesocket(multicastSocket);
        WSACleanup();
        throw std::runtime_error("Error joining multicast group");
    }

    // Create UDP socket
    if ((unicastSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }
    
    // Create UDP socket
    if ((sendApprovedSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket");
    }
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
    closesocket(multicastSocket);
    closesocket(unicastSocket);
}


// Internal: Initializes the list of subscribers
void Publisher::initializeList() {
    for (ShapeEnum::ShapeType shapeType : ShapeEnum::AllTypes) {
        // Create a subscriber shape object
        SubscriberShapePtr subscriberShapePtr = std::make_shared<SubscriberShape>(shapeTypeToString(shapeType), getFrequency(shapeType));

        // Add the subscriber shape object to the list
        subscribersList.push_back(subscriberShapePtr);

        // Insert a pointer to the subscriber shape object into the map
        map.insert(std::make_pair(shapeTypeToString(shapeType), subscriberShapePtr));
    }
}

//Gets the frequency for a given shape type
int Publisher::getFrequency(ShapeEnum::ShapeType shapeType) const {
    switch (shapeType) {
    case ShapeEnum::ShapeType::SQUARE: return 2; // 2 Hz
    case ShapeEnum::ShapeType::CIRCLE: return 3; // 3 Hz
    default: return 0;
    }
}

//Converts ShapeType enum value to string
std::string Publisher::shapeTypeToString(ShapeEnum::ShapeType shapeType) const {
    switch (shapeType) {
    case ShapeEnum::ShapeType::SQUARE: return "SQUARE";
    case ShapeEnum::ShapeType::CIRCLE: return "CIRCLE";
    default: return "";
    }
}

void Publisher::eventManager() {
    // Create a thread for handling circles
    std::thread circleThread(&Publisher::circleHandler, this);
    // Create a thread for handling squares
    std::thread squareThread(&Publisher::squareHandler, this);

     circleThread.join();
     squareThread.join();
}

// Function to handle circles
void Publisher::circleHandler() {
    // Create a JSON object
    nlohmann::json circleJson;

    // Add the shapeType field
    circleJson["shapeType"] = "Circle";

    while (running) {
        for (const auto& entry : functionMap) {
            circleJson[entry.first] = entry.second();// Call the function and store the result in JSON
        }
        
        std::string jsonString = circleJson.dump();

        auto i = map.find("CIRCLE");
        SubscriberShapePtr subscriberShapePtr = i->second;

        for (const SendingInfo& sendingInfo : subscriberShapePtr->specificTypeList) {
            sendShapeString(jsonString, sendingInfo);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(333)); // Sleep for 0.333 seconds
    }
}

// Function to handle squares
void Publisher::squareHandler() {
    // Create a JSON object
    nlohmann::json squareJson;

    // Add the shapeType field
    squareJson["shapeType"] = "Square";

    while (running) {
        for (const auto& entry : functionMap) {
            squareJson[entry.first] = entry.second();// Call the function and store the result in JSON
        }

        std::string jsonString = squareJson.dump();

        auto i = map.find("SQUARE");
        SubscriberShapePtr subscriberShapePtr = i->second;

        for (const SendingInfo& sendingInfo : subscriberShapePtr->specificTypeList) {
            sendShapeString(jsonString, sendingInfo);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for 0.5 seconds
    }
}


//// Function to generate JSON for shape properties
//void Publisher::generateShapeJson(nlohmann::json& shapeJson) {
//    for (const auto& entry : functionMap) {
//        auto i = entry.second();
//        shapeJson[entry.first] = i // Call the function and store the result in JSON
//    }
//}

////Manages the scheduling and sending shapes to subscribers
//void Publisher::eventManager() {
//    int counter = 0;
//    while (running) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 0.1 seconds
//        ++counter;
//        sendScheduledTasks(counter);
//    }
//}
//
//void Publisher::sendScheduledTasks(int counter) {
//    for (auto& subscriberPtr : subscribersList) {
//        auto& subscriber = *subscriberPtr; // Dereference the shared pointer to get the SubscriberShape object
//
//        if (subscriber.checkUpdateTime(counter)) {
//            sendToSubscriber(subscriber);
//        }
//    }
//}

//Sends a shape string to a subscriber
//void Publisher::sendToSubscriber(SubscriberShape& subscriberShape) {
//    std::string shapeString;
//    Shape* shape = generateShape(subscriberShape.shapeType);
//    if (subscriberShape.shapeType == "SQUARE") {
//        shapeString = generateSquareString(shape);
//    }
//    else {
//        shapeString = generateCircleString(shape);
//    }
//
//    for (const SendingInfo& sendingInfo : subscriberShape.specificTypeList) {
//        sendShapeString(shapeString, sendingInfo);
//    }
//    delete shape; // Remember to delete dynamically allocated shape
//}

// Generates a random size for the shape and returns it as a string
std::string Publisher::generateSize() {
    int size = (rand() % 100) + 1; // Random size between 1 and 100
    return std::to_string(size);
}

// Generates random coordinates for the shape and returns them as a string
std::string Publisher::generateCoordinates() {
    std::vector<int> coordinates(3);
    for (int i = 0; i < 3; ++i) {
        coordinates[i] = rand() % 1500; // Random coordinate
    }
    // Convert coordinates to string format
    std::string coordinatesString = "[" + std::to_string(coordinates[0]) + ", " +
        std::to_string(coordinates[1]) + ", " +
        std::to_string(coordinates[2]) + "]";
    return coordinatesString;
}

// Generates random colors for the shape and returns a string
std::string Publisher::generateColors() {
    std::vector<std::string> colors = { "Red", "Blue", "Green", "Yellow", "Purple" };
    int randomIndex = rand() % colors.size(); // Generate a random index within the range of colors vector
    return colors[randomIndex]; // Return the color at the random index
}


// Generates a random shape based on the given shape type
Shape* Publisher::generateShape(std::string& shapeType) {
    int size = (rand() % 100) + 1; // Random size between 1 and 100
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

//Sends a string representation of a shape to a subscriber
void Publisher::sendShapeString(const std::string& shapeString, const SendingInfo& sendingInfo) {
    const sockaddr_in& addr = sendingInfo.getAddress();
    sendto(unicastSocket, shapeString.c_str(), shapeString.length(), 0, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
}

//// Generates a string representation of a square
//std::string Publisher::generateSquareString(const Shape* shape) {
//    std::stringstream ss;
//    const Square* square = dynamic_cast<const Square*>(shape);
//    if (square) {
//        ss << "Square - Color: blue, Size: " << square->getSize() << ", Coordinates: ";
//        const std::vector<int>& coordinates = square->getCoordinates();
//        for (int coord : coordinates) {
//            ss << coord << " ";
//        }
//        ss << std::endl;
//    }
//    return ss.str();
//}
//
////Generates a string representation of a circle
//std::string Publisher::generateCircleString(const Shape* shape) {
//    std::stringstream ss;
//    const Circle* circle = dynamic_cast<const Circle*>(shape);
//    if (circle) {
//        ss << "Circle - Color: green, Size: " << circle->getSize() << ", Coordinates: ";
//        const std::vector<int>& coordinates = circle->getCoordinates();
//        for (int coord : coordinates) {
//            ss << coord << " ";
//        }
//        ss << std::endl;
//    }
//    return ss.str();
//}


//Listens for new subscribers and manages their subscription
void Publisher::subscriberRegistrar() {
    char receiveData[1024];
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    std::string delimiter = ":";

    while (running) {
        int bytesReceived = recvfrom(multicastSocket, receiveData, 1024, 0, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);

        if (bytesReceived != SOCKET_ERROR) {
            std::string jsonStr(receiveData, bytesReceived);
            auto jsonData = nlohmann::json::parse(jsonStr);

            int portNumber = jsonData["portNumber"];

            if (registeredPortNumbers.count(portNumber)) {
                continue;
            }

            // Register the port number
            registeredPortNumbers.insert(portNumber);

            //Create SendingInfo
            SendingInfo sendingInfo(portNumber);

            //Iterate over shape types and update SubscriberShape
            for (const auto& shapeType : jsonData["shapeTypes"]) {
                // Update subscriber shape in the map
                if (map.count(shapeType)) {
                    auto it = map.find(shapeType);
                    SubscriberShapePtr subscriberShapePtr = it->second;
                    subscriberShapePtr->specificTypeList.push_back(sendingInfo);

                    std::cout << "Registered subscriber for shape type: " << shapeType << std::endl;
                    std::cout << "Total subscribers for " << shapeType << ": " << subscriberShapePtr->specificTypeList.size() << std::endl;
                }
                else {
                    // Handle case when shape type is not found in map
                    // For example, log an error or ignore it
                }
            }
        }
    }
}











