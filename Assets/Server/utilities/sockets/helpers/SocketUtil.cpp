#include <sockets/helpers/SocketUtil.h>

#include <cerrno>
#include <cstring>
#include <iostream>

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