#include <sockets/udp/UDPSocket.h>
#include <error/ErrorCodes.h>
#include <unistd.h>

int UDPSocket::Bind(const SocketAddress& inBindAddress) {
    int err = bind(mSocket, &inBindAddress.ConstSockAddr(), inBindAddress.GetSize());
    if (err != 0) {
        SocketUtil::ReportError(L"UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo) {
    int byteSentCount = sendto(mSocket, static_cast<const char*>(inData), inLen, 0, &inTo.ConstSockAddr(), inTo.GetSize());
    if (byteSentCount >= 0) {
        return byteSentCount;
    }

    SocketUtil::ReportError(L"UDPSocket::SendTo");
    return SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom) {
    socklen_t fromLength = outFrom.GetSize();
    int readByteCount = recvfrom(mSocket, static_cast<char *>(inBuffer), inLen, 0, outFrom.MutableSockAddr(), &fromLength);
    if (readByteCount >= 0) {
        return readByteCount;
    }

    SocketUtil::ReportError(L"UDPSocket::ReceiveFrom");
    return SocketUtil::GetLastError();
}

UDPSocket::~UDPSocket() {
    close(mSocket);
}
