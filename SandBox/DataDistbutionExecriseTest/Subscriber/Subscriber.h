#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <thread>
#include "CommonLibrary\Common.h"
#include "../../../external/argparse/include/argparse/argparse.hpp"


// Include Winsock headers for Windows socket programming
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link against Ws2_32.lib


class Subscriber {
public:
    enum class ShapeType {
        CIRCLE,
        SQUARE
    };

    Subscriber(const std::string& name, int portNum);
    ~Subscriber();

    void subscribe(ShapeType shapeType, const std::string& publisherAddress);

private:
    SOCKET sendSocketDescriptor;
    SOCKET recvSocketDescriptor;
    SOCKET unicastSocket;
    std::string subscriberName;
    int portNumber;
    bool flag = true;
    sockaddr_in multicastSendingAddr;

    void registerToPublisher();
    void receiveUnicastData();
    std::string shapeTypeToString(ShapeType shapeType);
    void createSockets();
};
