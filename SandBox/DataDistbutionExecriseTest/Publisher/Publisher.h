#pragma once

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <array>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <map>
#include <thread>
#include <memory>
#include <chrono>
#include <stdexcept>
#include "CommonLibrary\Common.h"
#include "Shape.h"
#include "SubscriberShape.h"
#include "SendingInfo.h"


// Include Winsock headers for Windows socket programming
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link against Ws2_32.lib

// Forward declarations
class Shape;

namespace ShapeEnum
{
    // Define ShapeType enum
    enum class ShapeType {
        SQUARE = 2,
        CIRCLE = 3
    };

    // Define static array containing all shape types
    static const ShapeType AllTypes[] = { ShapeType::SQUARE, ShapeType::CIRCLE };
}


// Define Publisher class
class Publisher {
public:
    Publisher();
    ~Publisher();

    void startPublishing();
    void stopPublishing();

private:
    // Private member functions
    void initializeList();
    int getFrequency(ShapeEnum::ShapeType shapeType) const;
    std::string shapeTypeToString(ShapeEnum::ShapeType shapeType) const;
    void eventManager();
    void sendScheduledTasks(int counter);
    void sendToSubscriber(SubscriberShape& subscribersShape);
    Shape* generateShape(std::string& shapeType);
    void sendShapeString(const std::string& shapeString, const SendingInfo& sendingInfo);
    void subscriberRegistrar();
    std::string generateSquareString(const Shape* shape);
    std::string generateCircleString(const Shape* shape);

    void createSockets();

    // Private data members
    bool running;
    SOCKET multicastSocket;
    SOCKET unicastSocket;
    SOCKET sendApprovedSocket;
    sockaddr_in multicastSendingAddr;
    std::vector<SubscriberShape> subscribersList;
    std::map<std::string, SubscriberShape> map;
};

#endif // PUBLISHER_H
