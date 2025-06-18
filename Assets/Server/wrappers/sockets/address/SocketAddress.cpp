#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string>
#include <memory>
#include <sockets/address/SocketAddress.h>

SocketAddressPtr SocketAddress::IPv4(const char inAddress[4], uint16_t inPort) {
    auto addr = new SocketAddress();
    auto addr_in = addr->GetAsSockAddrIn();
    memcpy(addr_in, 0, sizeof(sockaddr_in));

    addr_in->sin_family = AF_INET;
    uint32_t uintaddr;
    memcpy(&uintaddr, inAddress, 4 * sizeof(char));
    addr_in->sin_addr.s_addr = htonl(uintaddr);
    addr_in->sin_port = htons(inPort);

    return SocketAddressPtr(addr);
}

SocketAddressPtr SocketAddress::IPv6(const char inAddress[16], uint16_t inPort) {
    auto addr = new SocketAddress();
    auto addr_in6 = addr->GetAsSockAddrIn6();
    memcpy(addr_in6, 0, sizeof(sockaddr_in6));

    addr_in6->sin6_family = AF_INET6;
    memcpy(&addr_in6->sin6_addr, inAddress, 16 * sizeof(char));
    addr_in6->sin6_port = htons(inPort);

    return SocketAddressPtr(addr);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr) {
    std::memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

SocketAddress::SocketAddress() {}

size_t SocketAddress::GetSize() const {
    switch (mSockAddr.sa_family) {
        case AF_INET6:
            return sizeof(sockaddr_in6);
        case AF_INET:
            return sizeof(sockaddr_in);
        default:
            return sizeof(sockaddr);
    }
}

sockaddr_in* SocketAddress::GetAsSockAddrIn() {
    return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}

sockaddr_in6* SocketAddress::GetAsSockAddrIn6() {
    return reinterpret_cast<sockaddr_in6*>(&mSockAddr);
}

std::string SocketAddress::ToString() const {
    auto sa_ptr = &mSockAddr;
    char ip[INET6_ADDRSTRLEN] = {0};

    switch (sa_ptr->sa_family) {
        case AF_INET: {
            auto sin = reinterpret_cast<const sockaddr_in*>(sa_ptr);
            if (inet_ntop(AF_INET, &sin->sin_addr, ip, sizeof(ip))) {
                return std::string(ip) + ":" + std::to_string(ntohs(sin->sin_port));
            }
            break;
        }
        case AF_INET6: {
            auto sin6 = reinterpret_cast<const sockaddr_in6*>(sa_ptr);
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, ip, sizeof(ip))) {
                return "[" + std::string(ip) + "]:" + std::to_string(ntohs(sin6->sin6_port));
            }
            break;
        }
        default:
            break;
    }

    return std::string("<invalid_socket_address>");
}