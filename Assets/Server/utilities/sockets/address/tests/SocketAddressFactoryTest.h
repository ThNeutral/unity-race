#ifndef SOCKET_ADDRESS_FACTORY_TEST_H
#define SOCKET_ADDRESS_FACTORY_TEST_H

#include <doctest.h>

#include <sockets/address/SocketAddressFactory.h>

TEST_CASE("SocketAddressFactory") {
    SUBCASE("valid IPv4") {
        std::string ipv4 = "127.0.0.1:8080";
        auto addr = SocketAddressFactory::CreateIPv4FromString(ipv4);
        CHECK(addr != nullptr);
        CHECK(addr->ToString() == ipv4);
    }

    SUBCASE("valid IPv4 without port") {
        std::string ipv4 = "127.0.0.1";
        auto addr = SocketAddressFactory::CreateIPv4FromString(ipv4);
        CHECK(addr != nullptr);
        CHECK(addr->ToString() == (ipv4 + ":0"));
    }

    SUBCASE("invalid IPv4") {
        std::string ipv4 = "127123123.0213123.0132123.1123123";
        auto addr = SocketAddressFactory::CreateIPv4FromString(ipv4);
        CHECK(addr == nullptr);
    }

    SUBCASE("valid IPv6") {
        auto addr = SocketAddressFactory::CreateIPv6FromString("[::1]:8080");
        CHECK(addr != nullptr);
        CHECK(addr->ToString() == "[::]:8080");
    }

    SUBCASE("valid IPv6 without port") {
        auto addr = SocketAddressFactory::CreateIPv6FromString("[::1]");
        CHECK(addr != nullptr);
        CHECK(addr->ToString() == "[::]:0");
    }

    SUBCASE("invalid IPv6") {
        std::string ipv6 = "[asdasdas:asdasdas;asdas]";
        auto addr = SocketAddressFactory::CreateIPv6FromString(ipv6);
        CHECK(addr == nullptr);
    }
}

#endif