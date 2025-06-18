#ifndef SOCKET_ADDRESS_SOCKET_H
#define SOCKET_ADDRESS_SOCKET_H

#include <sockets/address/SocketAddress.h>
#include <string>

class SocketAddressFactory {
    public:
        static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};

#endif