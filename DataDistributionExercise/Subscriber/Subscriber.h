#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <thread>

// Include Winsock headers for Windows socket programming
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link against Ws2_32.lib



enum class ShapeType {
    CIRCLE,
    SQUARE
};

class Subscriber {
public:
    Subscriber(const std::string& name);
    ~Subscriber();
    void subscribe(ShapeType shapeType, const std::string& publisherAddress, int publisherPort);
    void receiveData();

private:
    SOCKET socketDescriptor;

    std::string subscriberName;

    std::string shapeTypeToString(ShapeType shapeType);
};
