#include <iostream>
#include <vector>

#include <sockets/helpers/SocketUtil.h>
#include <sockets/address/SocketAddressFactory.h>
#include <error/ErrorCodes.h>

int main() {
    auto listenSocket = SocketUtil::CreateTCPSocket(INET6);
    auto receivingAddress = SocketAddressFactory::CreateIPv6FromString("localhost:8080");
    if (listenSocket->Bind(*receivingAddress) != NO_ERROR) {
        return 1;
    }
    std::cout << "Bound socket to " << receivingAddress->ToString() << std::endl;

    std::vector<TCPSocketPtr> readBlockSockets;
    readBlockSockets.push_back(listenSocket);

    int error = listenSocket->Listen();
    if (error != NO_ERROR) {
        return 1;
    }

    std::vector<TCPSocketPtr> readableSockets;

    std::cout << "Started listen loop" << std::endl;
    while (true) {
        if (SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr)) {
            for (const auto socket : readableSockets) {
                if (socket == listenSocket) {
                    std::cout << "New connection" << std::endl;
                    SocketAddress newClientAddress;
                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSockets.push_back(newSocket);
                    std::cout << "New connection from " << newClientAddress.ToString() << std::endl;
                } else {
                    const int bufSize = 512;
                    char buf[bufSize];
                    int receivedCount = socket->Receive(buf, bufSize);
                    if (receivedCount < 0) continue;
                    std::cout << "Received " << receivedCount << " bytes: \n" 
                        << "------------START------------\n"
                        << std::string(buf, receivedCount)
                        << "-------------END-------------"
                        << std::endl;
                    }
            }
        }
    }
    

    return 0;
}