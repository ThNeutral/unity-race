#ifndef INPUT_MEMORY_STREAM_TEST_H
#define INPUT_MEMORY_STREAM_TEST_H

#include <doctest.h>

#include <streams/byte/InputMemoryStream.h>
#include <streams/byte/OutputMemoryStream.h>

TEST_CASE("MemoryStream") {
    char* bufferPtr; 
    size_t bufferSize;
    SUBCASE("OutputMemoryStream") {
        auto stream = OutputMemoryStream();
        uint32_t expectedCapacity = 0;
        CHECK(stream.GetLength() == expectedCapacity);
        CHECK(stream.GetCapacity() == 32);
        
        uint32_t bigEndian = 0x12345678;
        stream.Write(bigEndian);
        expectedCapacity += 4;
        CHECK(stream.GetLength() == expectedCapacity);
        CHECK(stream.GetCapacity() == 32);
        auto buf = stream.GetBufferPtr();
        CHECK(buf[3] == 0x12);
        CHECK(buf[2] == 0x34);
        CHECK(buf[1] == 0x56);
        CHECK(buf[0] == 0x78);

        size_t size = 32 * sizeof(char);
        void* data = malloc(size);
        memset(data, 1, size);

        stream.Write(data, size);
        buf = stream.GetBufferPtr();
        expectedCapacity += size;
        CHECK(stream.GetLength() == expectedCapacity);
        CHECK(stream.GetCapacity() == 64);

        bool valid = true;
        for (int i = 0; i < 32; i++) {
            valid = valid && (buf[4 + i] == 1);
        }
        CHECK(valid);

        free(data);

        size = 256 * sizeof(char);
        data = malloc(size);
        memset(data, 2, size);

        stream.Write(data, size);
        buf = stream.GetBufferPtr();
        expectedCapacity += size;
        CHECK(stream.GetLength() == expectedCapacity);
        CHECK(stream.GetCapacity() == expectedCapacity);

        valid = true;
        for (int i = 0; i < 256; i++) {
            valid = valid && (buf[36 + i] == 2);
        }
        CHECK(valid);

        free(data);

        bufferSize = stream.GetLength();
        bufferPtr = static_cast<char*>(malloc(bufferSize));

        memcpy(bufferPtr, stream.GetBufferPtr(), bufferSize);
    }
    
    SUBCASE("InputMemoryStream") {
        auto stream = InputMemoryStream(bufferPtr, bufferSize);

        CHECK(stream.GetRemainingDataSize() == bufferSize);

        char buf1[4];
        stream.Read(buf1, 4);
        CHECK(stream.GetRemainingDataSize() == bufferSize - 4);

        CHECK(buf1[0] == 0x78);
        CHECK(buf1[1] == 0x56);
        CHECK(buf1[2] == 0x34);
        CHECK(buf1[3] == 0x12);

        char buf2[32];
        stream.Read(buf2, 32);
        CHECK(stream.GetRemainingDataSize() == bufferSize - 4 - 32);

        bool valid = true;
        for (int i = 0; i < 32; i++) {
            valid = valid && (buf2[i] == 1);
        }
        CHECK(valid);

        char buf3[256];
        stream.Read(buf3, 256);
        CHECK(stream.GetRemainingDataSize() == bufferSize - 4 - 32 - 256);

        valid = true;
        for (int i = 0; i < 256; i++) {
            valid = valid && (buf3[i] == 2);
        }
        CHECK(valid);

        char buf4[2];
        CHECK_THROWS(stream.Read(buf4, 2));
    }
}

#endif