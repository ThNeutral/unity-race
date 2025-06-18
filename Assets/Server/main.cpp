#include <iostream>
#include <vector>

#include <sockets/helpers/SocketUtil.h>
#include <sockets/address/SocketAddressFactory.h>
#include <error/ErrorCodes.h>

int main() {
    auto listenSocket = SocketUtil::CreateTCPSocket(INET);
    auto receivingAddress = SocketAddressFactory::CreateIPv4FromString("localhost:8080");
    if (listenSocket->Bind(*receivingAddress) != NO_ERROR) {
        return 1;
    }

    std::vector<TCPSocketPtr> readBlockSockets;
    readBlockSockets.push_back(listenSocket);

    std::vector<TCPSocketPtr> readableSockets;

    while (true) {
        if (SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr)) {
            for (const auto socket : readableSockets) {
                if (socket == listenSocket) {
                    SocketAddress newClientAddress;
                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSockets.push_back(newSocket);
                }
            }
        }
    }
    

    return 0;
}