#ifndef BYTE_SWAPS_TEST_H
#define BYTE_SWAPS_TEST_H

#include <doctest.h>
#include <bytes/ByteSwaps.h>

TEST_CASE("ByteSwaps") {
    SUBCASE("2-byte") {
        CHECK(ByteSwap2(0x1234) == 0x3412);
        CHECK(ByteSwap2(0x1200) == 0x0012);
        CHECK(ByteSwap2(0x0034) == 0x3400);
    }

    SUBCASE("4-byte") {
        CHECK(ByteSwap4(0x12345678) == 0x78563412);
        CHECK(ByteSwap4(0x12000000) == 0x00000012);
        CHECK(ByteSwap4(0x00003400) == 0x00340000);
    }

    SUBCASE("8-byte") {
        CHECK(ByteSwap8(0x0123456789ABCDEF) == 0xEFCDAB8967452301);
        CHECK(ByteSwap8(0x0000000000000012) == 0x1200000000000000);
        CHECK(ByteSwap8(0x0000000034000000) == 0x0000003400000000);
    }
}

#endif