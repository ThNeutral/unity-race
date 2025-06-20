#include <sockets/address/SocketAddress.h>
#include <sockets/address/SocketAddressFactory.h>
#include <string>
#include <netdb.h>

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& inString) {
    auto pos = inString.find_last_of(":");

    std::string host, service;
    if (pos != std::string::npos) {
        host = inString.substr(0, pos);
        service = inString.substr(pos + 1);
    } else {
        host = inString;
        service = "0";
    }

    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;

    addrinfo* result;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error != 0 || result == nullptr) {
        return nullptr;
    }

    while (!result->ai_addr && result->ai_next) {
        result = result->ai_next;
    }

    if (!result->ai_addr) {
        freeaddrinfo(result);
        return nullptr;
    }

    auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

    freeaddrinfo(result);

    return toRet;
}

SocketAddressPtr SocketAddressFactory::CreateIPv6FromString(const std::string& inString) {
    auto pos = inString.find_last_of(":");

    std::string host, service;
    if (pos != std::string::npos) {
        host = inString.substr(0, pos);
        service = inString.substr(pos + 1);
    } else {
        host = inString;
        service = "0";
    }

    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET6;

    addrinfo* result;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error != 0 || result == nullptr) {
        return nullptr;
    }

    while (!result->ai_addr && result->ai_next) {
        result = result->ai_next;
    }

    if (!result->ai_addr) {
        freeaddrinfo(result);
        return nullptr;
    }

    auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

    freeaddrinfo(result);

    return toRet;
}