#ifndef TYPE_ALIASER_TEST_H
#define TYPE_ALIASER_TEST_H

#include <string.h>
#include <doctest.h>

#include <types/TypeAliaser.h>

TEST_CASE("TypeAliaser") {
    SUBCASE("float to uint32_t") {
        float f = 1.0f;
        TypeAliaser<float, uint32_t> aliaser(f);

        CHECK(aliaser.mAsFromType == doctest::Approx(1.0f));

        uint32_t expectedBits;
        std::memcpy(&expectedBits, &f, sizeof(float));
        CHECK(aliaser.Get() == expectedBits);
    }

    SUBCASE("uint32_t to float") {
        uint32_t intBits = 0x3F800000;
        TypeAliaser<uint32_t, float> aliaser(intBits);
    
        CHECK(aliaser.mAsFromType == intBits);
        CHECK(aliaser.Get() == doctest::Approx(1.0f));    
    }
}

#endif