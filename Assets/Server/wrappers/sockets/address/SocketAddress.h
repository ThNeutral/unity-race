#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <memory>

class SocketAddress;
typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

#include <sockets/udp/UDPSocket.h>
#include <sockets/tcp/TCPSocket.h>
#include <sockets/helpers/SocketAddressFamily.h>

class SocketAddress {
public:
    static SocketAddressPtr IPv4(const char inAddress[4], uint16_t inPort);
    static SocketAddressPtr IPv6(const char inAddress[16], uint16_t inPort);
    SocketAddress(const sockaddr& inSockAddr);
    SocketAddress();
    size_t GetSize() const;
    std::string ToString() const;
private:
    sockaddr mSockAddr;
    sockaddr_in* GetAsSockAddrIn();
    sockaddr_in6* GetAsSockAddrIn6();
    
    friend class UDPSocket;
    friend class TCPSocket;
};

#endif