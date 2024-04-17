#include "SendingInfo.h"

SendingInfo::SendingInfo(const sockaddr_in& address, int port)
    : address(address), port(port) {}

const sockaddr_in& SendingInfo::getAddress() const {
    return address;
}

int SendingInfo::getPort() const {
    return port;
}
