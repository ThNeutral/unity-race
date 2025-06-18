#include <sockets/helpers/SocketUtil.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/select.h>

int SocketUtil::GetLastError() {
    return errno;
}

void SocketUtil::ReportError(const wchar_t* funcName) {
    int err = GetLastError();
    std::wcerr << funcName << L" failed with error " << err << L": "
               << std::strerror(err) << std::endl;
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily) {
    int s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (s != INVALID_SOCKET) {
        return UDPSocketPtr(new UDPSocket(s));
    }

    ReportError(L"SocketUtil::CreateUDPSocket");
    return nullptr;
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily) {
    int s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
    if (s != INVALID_SOCKET) {
        return TCPSocketPtr(new TCPSocket(s));
    }

    ReportError(L"SocketUtil::CreateTCPSocket");
    return nullptr;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets) {
    if (inSockets) {
        FD_ZERO(&outSet);
        for (const TCPSocketPtr& socket : *inSockets) {
            FD_SET(socket->mSocket, &outSet);
        }
        return &outSet;
    } 

    return nullptr;
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set& inSet) {
    if (inSockets && outSockets) {
        outSockets->clear();
        for (const TCPSocketPtr& socket : *inSockets) {
            if (FD_ISSET(socket->mSocket, &inSet)) {
                outSockets->push_back(socket);
            }
        }
    }
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* inReadSet,
                       std::vector<TCPSocketPtr>* outReadSet,
                       const std::vector<TCPSocketPtr>* inWriteSet,
                       std::vector<TCPSocketPtr>* outWriteSet,
                       const std::vector<TCPSocketPtr>* inExceptSet,
                       std::vector<TCPSocketPtr>* outExceptSet
                    ) {
    // Construct fd_sets from vectors
    fd_set read, write, except;

    fd_set* readPtr = FillSetFromVector(read, inReadSet);
    fd_set* writePtr = FillSetFromVector(write, inWriteSet);
    fd_set* exceptPtr = FillSetFromVector(except, inExceptSet);

    int maxFd = -1;

    #ifdef _WIN32 
        maxFd = 0;
    #else
        auto updateMaxFd = [&](const std::vector<TCPSocketPtr>* sockets) {
            if (!sockets) return;
            for (const auto& sock : *sockets) {
                if (sock->GetFd() > maxFd) {
                    maxFd = sock->GetFd();
                }
            }
        };

        updateMaxFd(inReadSet);
        updateMaxFd(inWriteSet);
        updateMaxFd(inExceptSet);
    #endif

    int toRet = select(maxFd + 1, readPtr, writePtr, exceptPtr, nullptr);

    if (toRet > 0) {
        FillVectorFromSet(outReadSet, inReadSet, read);
        FillVectorFromSet(outWriteSet, inWriteSet, write);
        FillVectorFromSet(outExceptSet, inExceptSet, except);
    }

    return toRet;
}