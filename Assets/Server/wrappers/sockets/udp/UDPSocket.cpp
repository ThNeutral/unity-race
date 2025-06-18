#include <sockets/udp/UDPSocket.h>
#include <error/ErrorCodes.h>

#include <fcntl.h>
#include <unistd.h>

int UDPSocket::Bind(const SocketAddress& inBindAddress) {
    int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (err != 0) {
        SocketUtil::ReportError(L"UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo) {
    int byteSentCount = sendto(mSocket, static_cast<const char*>(inData), inLen, 0, &inTo.mSockAddr, inTo.GetSize());
    if (byteSentCount >= 0) {
        return byteSentCount;
    }

    SocketUtil::ReportError(L"UDPSocket::SendTo");
    return SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom) {
    socklen_t fromLength = outFrom.GetSize();
    int readByteCount = recvfrom(mSocket, static_cast<char *>(inBuffer), inLen, 0, &outFrom.mSockAddr, &fromLength);
    if (readByteCount >= 0) {
        return readByteCount;
    }

    SocketUtil::ReportError(L"UDPSocket::ReceiveFrom");
    return SocketUtil::GetLastError();
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking) {
    int result;
    #if _WIN32
        u_long arg = inShouldBeNonBlocking ? 1 : 0;
        result = ioctlsocket(mSocket, FIONBIO, &arg);
    #else
        int flags = fcntl(mSocket, F_GETFL, 0);
        flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        result = fcntl(mSocket, F_SETFL, flags);
    #endif

    if (result == SOCKET_ERROR) {
        SocketUtil::ReportError(L"UDPSocket::SetNonBlockingMode");
        return SocketUtil::GetLastError();
    }

    return NO_ERROR;
}

UDPSocket::~UDPSocket() {
    close(mSocket);
}
