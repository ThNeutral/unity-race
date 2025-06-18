#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string>
#include <memory>
#include <sockets/address/SocketAddress.h>

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort) {
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr) {
    std::memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

size_t SocketAddress::GetSize() const {
    return sizeof(sockaddr);
}

sockaddr_in* SocketAddress::GetAsSockAddrIn() {
    return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}

const sockaddr& SocketAddress::ConstSockAddr() const {
    return mSockAddr;
}

sockaddr* SocketAddress::MutableSockAddr() {
    return &mSockAddr;
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

    return std::string("<invalid>");
}