#ifndef UDP_ADDRESS_H
#define UDP_ADDRESS_H

#include <memory>

class UDPSocket;
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

#include <sockets/address/SocketAddress.h>
#include <sockets/helpers/SocketUtil.h>

class UDPSocket {
    public:
        ~UDPSocket();
        int Bind(const SocketAddress& inToAddress);
        int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
        int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);
    private:
        UDPSocket(int inSocket) : mSocket(inSocket) {}
        int mSocket;
        friend class SocketUtil;
};

#endif