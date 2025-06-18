#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <memory>

class TCPSocket;
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

#include <sockets/address/SocketAddress.h>
#include <sockets/helpers/SocketUtil.h>

class TCPSocket {
    public:
        ~TCPSocket();
        int Connect(const SocketAddress& inAddress);
        int Bind(const SocketAddress& inToAddress);
        int Listen(int inBackLog = 32);
        TCPSocketPtr Accept(SocketAddress& inFromAddress);
        int Send(const void* inData, int inLen);
        int Receive(void* inBuffer, int inLen);
        int GetFd() const;
    private:
        friend class SocketUtil;
        TCPSocket(int inSocket) : mSocket(inSocket) {}
        int mSocket;
};

#endif