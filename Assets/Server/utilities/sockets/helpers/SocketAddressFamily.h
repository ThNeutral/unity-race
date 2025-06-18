#ifndef SOCKET_ADDRESS_FAMILY_H
#define SOCKET_ADDRESS_FAMILY_H

#include <sys/socket.h>

enum SocketAddressFamily {
    INET = AF_INET,
    INET6 = AF_INET6,
};

#endif