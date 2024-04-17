#pragma once

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <array>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <map>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "Shape.h"
#include "SubscriberShape.h"
#include "SendingInfo.h"

// Include Winsock headers for Windows socket programming
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link against Ws2_32.lib

// Forward declarations
class Shape;

// Define ShapeType enum
enum class ShapeType {
    SQUARE = 2,
    CIRCLE = 3
};

// Define Publisher class
class Publisher {
public:
    Publisher(int port);
    ~Publisher();

    void startPublishing();
    void stopPublishing();

private:
    // Private member functions
    void initializeMapAndList();
    int getFrequency(ShapeType shapeType) const;
    std::string shapeTypeToString(ShapeType shapeType) const;
    void eventManager();
    void sendScheduledTasks(int counter);
    void sendToSubscriber(SubscriberShape& subscribersShape);
    Shape* generateShape(std::string& shapeType);
    void sendShape(const Shape* shape, const SendingInfo& sendingInfo);
    void sendShapeString(const std::string& shapeString, const SendingInfo& sendingInfo);
    void subscriberRegistrar();
    std::string generateSquareString(const Shape* shape);
    std::string generateCircleString(const Shape* shape);

    // Private data members
    bool running;
    SOCKET socketDescriptor;
    std::vector<SubscriberShape> subscribersList;
    std::map<std::string, SubscriberShape> map;
};

#endif // PUBLISHER_H
