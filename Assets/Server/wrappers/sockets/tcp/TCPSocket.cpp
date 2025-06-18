#include <sockets/tcp/TCPSocket.h>
#include <error/ErrorCodes.h>
#include <unistd.h>

int TCPSocket::Connect(const SocketAddress& inAddress) {
    int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
    if (err < 0) {
        SocketUtil::ReportError(L"TCPSocket::Connect");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress& inBindAddress) {
    int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (err != 0) {
        SocketUtil::ReportError(L"TCPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Listen(int inBacklog) {
    int err = listen(mSocket, inBacklog);
    if (err < 0) {
        SocketUtil::ReportError(L"TCPSocket::Listen");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress) {
    socklen_t length = inFromAddress.GetSize();
    int newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);
    if (newSocket != INVALID_SOCKET) {
        return TCPSocketPtr(new TCPSocket(newSocket));
    }

    SocketUtil::ReportError(L"TCPSocket::Accept");
    return nullptr;
}

int TCPSocket::Send(const void* inData, int inLen) {
    int bytesSendCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
    if (bytesSendCount < 0) {
        SocketUtil::ReportError(L"TCPSocket::Send");
        return SocketUtil::GetLastError();
    }
    return bytesSendCount;
}

int TCPSocket::Receive(void* inData, int inLen) {
    int bytesReceivedCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
    if (bytesReceivedCount < 0) {
        SocketUtil::ReportError(L"TCPSocket::Receive");
        return SocketUtil::GetLastError();
    }
    return bytesReceivedCount;
}

int TCPSocket::GetFd() const {
    return mSocket;
}

TCPSocket::~TCPSocket() {
    close(mSocket);
}