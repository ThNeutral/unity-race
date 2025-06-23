#ifndef INPUT_MEMORY_BIT_STREAM_TEST_H
#define INPUT_MEMORY_BIT_STREAM_TEST_H

#include <doctest.h>
#include <vector>
#include <stdlib.h>

#include <streams/bit/InputMemoryBitStream.h>
#include <streams/bit/OutputMemoryBitStream.h>

TEST_CASE("MemoryBitStream") {
    char* buffer;
    size_t size;

    SUBCASE("OutputMemoryBitStream") {
        auto stream = OutputMemoryBitStream();
        CHECK(stream.GetBitLenght() == 0);
        CHECK(stream.GetByteLenght() == 0);
        CHECK(stream.GetBitCapacity() == 32 * 8);
        CHECK(stream.GetByteCapacity() == 32);

        stream.Write(true);
        CHECK(stream.GetBitLenght() == 1);
        CHECK(stream.GetByteLenght() == 1);
        CHECK(stream.GetBitCapacity() == 32 * 8);
        CHECK(stream.GetByteCapacity() == 32);
        auto buf = stream.GetBufferPtr();
        CHECK(buf[0] == 0b00000001);

        uint8_t data7 = 0b01010101;
        stream.Write(data7, 7);
        CHECK(stream.GetBitLenght() == 8);
        CHECK(stream.GetByteLenght() == 1);
        CHECK(stream.GetBitCapacity() == 32 * 8);
        CHECK(stream.GetByteCapacity() == 32);
        buf = stream.GetBufferPtr();
        CHECK(buf[0] == (char)0b10101011);

        std::vector<char> vec (41, 0b01010101);
        stream.WriteBits(vec.data(), 40 * 8 + 5);
        CHECK(stream.GetBitLenght() == 8 + 40 * 8 + 5);
        CHECK(stream.GetByteLenght() == 42);
        CHECK(stream.GetBitCapacity() == 64 * 8);
        CHECK(stream.GetByteCapacity() == 64);
        buf = stream.GetBufferPtr();

        bool valid = true;
        for (int i = 0; i < 40; i++) {
            valid = valid && (buf[1 + i] == (char)0b01010101);
        }
        CHECK(valid);

        stream.Write(0b00000111, 3);
        CHECK(stream.GetBitLenght() == 8 + 40 * 8 + 8);
        CHECK(stream.GetByteLenght() == 42);
        CHECK(stream.GetBitCapacity() == 64 * 8);
        CHECK(stream.GetByteCapacity() == 64);
        CHECK(buf[1 + 40] == (char)0b11110101);

        size = 42 * 8;
        buffer = static_cast<char*>(malloc(42));
        memcpy(buffer, stream.GetBufferPtr(), 42);
    }

    SUBCASE("InputMemoryBitStream") {
        auto stream = InputMemoryBitStream(buffer, size);

        bool outBool = false;
        stream.Read(outBool);
        CHECK(outBool);
        CHECK(stream.GetRemainingBitDataSize() == (size - 1));

        uint8_t outChar = 0x00;
        stream.Read(outChar, 7);
        CHECK(outChar == 0b01010101);
        CHECK(stream.GetRemainingBitDataSize() == (size - 8));

        std::vector<char> vec (41);
        stream.ReadBytes(vec.data(), 41);
        CHECK(stream.GetRemainingBitDataSize() == (size - 8 - 41 * 8));

        bool valid = true;
        for (int i = 0; i < 40; i++) {
            valid = valid && (vec[i] == (char)0b01010101);
        }
        CHECK(valid);

        CHECK(vec[40] == (char)0b11110101);

        uint8_t outChar2;
        CHECK_THROWS(stream.Read(outChar2, 1));
    }
}

#endif