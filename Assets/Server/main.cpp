#include <iostream>
#include <vector>
#include <string>
#include <errno.h>

#include <sockets/helpers/SocketUtil.h>
#include <sockets/address/SocketAddressFactory.h>
#include <logger/Logger.h>
#include <error/ErrorCodes.h>

void closeSocket(std::vector<TCPSocketPtr>* sockets, TCPSocketPtr socket) {
    auto toRemove = std::remove(sockets->begin(), sockets->end(), socket);
    sockets->erase(toRemove);
    socket->Close();
}

int main() {
    auto listenSocket = SocketUtil::CreateTCPSocket(INET6);

    std::string desiredIP = "[::1]";
    auto receivingAddress = SocketAddressFactory::CreateIPv6FromString(desiredIP);
    if (receivingAddress == nullptr) {
        Logger::ReportError("main::setup", ((std::string("Failed to parse ") + desiredIP).c_str()));
        return 1;
    }
    
    if (listenSocket->Bind(*receivingAddress) != NO_ERROR) {
        return 1;
    }
    Logger::ReportInfo("main::setup", (std::string("Bound socket to ") + receivingAddress->ToString()).c_str());

    std::vector<TCPSocketPtr> readBlockSockets;
    readBlockSockets.push_back(listenSocket);

    int error = listenSocket->Listen();
    if (error != NO_ERROR) {
        return 1;
    }

    std::vector<TCPSocketPtr> readableSockets;

    Logger::ReportInfo("main::setup", "Started listen loop");
    while (true) {
        if (SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr)) {
            for (const auto socket : readableSockets) {
                if (socket == listenSocket) {
                    SocketAddress newClientAddress;
                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSockets.push_back(newSocket);
                    Logger::ReportInfo("main::listen_loop", (std::string("New connection from ") + newClientAddress.ToString()).c_str());
                    continue;
                } 

                const int bufSize = 512;
                char buf[bufSize];
                int receivedCount = socket->Receive(buf, bufSize);
                
                if (receivedCount == 0) {
                    Logger::ReportWarning("main::listen_loop", "Client disconnected");
                    closeSocket(&readableSockets, socket);
                    continue;
                }

                if (receivedCount < 0) {
                    Logger::ReportError("main::listen_loop", errno, std::strerror(errno));
                    closeSocket(&readableSockets, socket);
                    continue;
                }

                std::cout << "Received " << receivedCount << " bytes: \n" 
                    << "------------START------------\n"
                    << std::string(buf, receivedCount)
                    << "-------------END-------------\n"
                    << std::endl;
            }
        }
    }
    

    return 0;
}