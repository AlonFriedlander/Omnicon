#pragma once

#include <WinSock2.h> // For sockaddr_in

class SendingInfo {
public:
    SendingInfo(const sockaddr_in& address, int port);

    // Getter functions for address and port
    const sockaddr_in& getAddress() const;
    int getPort() const;

    sockaddr_in address;
    int port;
};
