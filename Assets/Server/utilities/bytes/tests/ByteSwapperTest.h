#ifndef BYTE_SWAPPER_TEST_H
#define BYTE_SWAPPER_TEST_H

#include <doctest.h>
#include <bytes/ByteSwapper.h>

TEST_CASE("ByteSwapper") {
    SUBCASE("2-byte") {
        CHECK(ByteSwap((uint16_t)0x1234) == (uint16_t)0x3412);
        CHECK(ByteSwap((uint16_t)0x1200) == (uint16_t)0x0012);
        CHECK(ByteSwap((uint16_t)0x0034) == (uint16_t)0x3400);
    }

    SUBCASE("4-byte") {
        CHECK(ByteSwap((uint32_t)0x12345678) == (uint32_t)0x78563412);
        CHECK(ByteSwap((uint32_t)0x12000000) == (uint32_t)0x00000012);
        CHECK(ByteSwap((uint32_t)0x00003400) == (uint32_t)0x00340000);
    }

    SUBCASE("8-byte") {
        CHECK(ByteSwap((uint64_t)0x0123456789ABCDEF) == (uint64_t)0xEFCDAB8967452301);
        CHECK(ByteSwap((uint64_t)0x0000000000000012) == (uint64_t)0x1200000000000000);
        CHECK(ByteSwap((uint64_t)0x0000000034000000) == (uint64_t)0x0000003400000000);
    }
}

#endif