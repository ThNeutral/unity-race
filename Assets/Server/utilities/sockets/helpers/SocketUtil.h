#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <sockets/udp/UDPSocket.h>
#include <sys/socket.h>

#define INVALID_SOCKET -1

enum SocketAddressFamily {
    INET = AF_INET,
    INET6 = AF_INET6,
};

class SocketUtil {
    public:
        static void ReportError(const wchar_t* funcName);
        static int GetLastError();
        static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
};

#endif