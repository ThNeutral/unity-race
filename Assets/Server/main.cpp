#include <iostream>

#include <sockets/helpers/SocketUtil.h>
#include <sockets/address/SocketAddressFactory.h>
#include <error/ErrorCodes.h>

int main() {
    auto udpSocket = SocketUtil::CreateUDPSocket(SocketAddressFamily::INET);
    auto address = SocketAddressFactory::CreateIPv4FromString("localhost:8080");
    
    if (udpSocket->Bind(*address) != NO_ERROR) {
        return 1;
    }
    std::cout << "Successfully bound socket to " << address->ToString() << std::endl;

    const int bufSize = 512;
    char buffer[bufSize];
    auto fromSockAddr = SocketAddress(sockaddr());
    auto readSize = udpSocket->ReceiveFrom(buffer, bufSize, fromSockAddr);
    if (readSize < 0) {
        return 1;
    }

    std::cout << "Received " << readSize << " bytes: "<< std::string(buffer, readSize) << std::endl;
    return 0;
}