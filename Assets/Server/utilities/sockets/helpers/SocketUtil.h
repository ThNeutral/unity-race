#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <sockets/udp/UDPSocket.h>
#include <sockets/tcp/TCPSocket.h>
#include <sockets/helpers/SocketAddressFamily.h>

#include <sys/socket.h>

#define INVALID_SOCKET -1

class SocketUtil {
    public:
        static void ReportError(const wchar_t* funcName);
        static int GetLastError();
        static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
        static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
        static fd_set* FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets);
        static void FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set& inSet);
        static int Select(const std::vector<TCPSocketPtr>* inReadSet,
            std::vector<TCPSocketPtr>* outReadSet,
            const std::vector<TCPSocketPtr>* inWriteSet,
            std::vector<TCPSocketPtr>* outWriteSet,
            const std::vector<TCPSocketPtr>* inExceptSet,
            std::vector<TCPSocketPtr>* outExceptSet
        );
};

#endif