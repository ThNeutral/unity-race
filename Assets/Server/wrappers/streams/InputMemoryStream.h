#ifndef INPUT_MEMORY_STREAM_H
#define INPUT_MEMORY_STREAM_H

#include <stdint.h>

class InputMemoryStream {
    public:
        InputMemoryStream(char* inBuffer, uint32_t inByteCount);
        ~InputMemoryStream();

        uint32_t GetRemainingDataSize() const;

        int Read(void* outData, uint32_t inByteCount);
        int Read(uint32_t& outData);
        int Read(int32_t& outData);
    private:
        char* mBuffer;
        uint32_t mHead;
        uint32_t mCapacity;
};

#endif