#ifndef OUTPUT_MEMORY_STREAM_H
#define OUTPUT_MEMORY_STREAM_H

#include <stdint.h>
#include <stddef.h> 

class OutputMemoryStream {
    public:
        OutputMemoryStream();
        ~OutputMemoryStream();
        const char* GetBufferPtr() const;
        uint32_t GetLength() const;

        void Write(const void* inData, size_t inByteCount);
        void Write(uint32_t inData);
        void Write(int32_t inData);

    private:
        void ReallocBuffer(uint32_t inNewLength);

        char* mBuffer;
        uint32_t mHead;
        uint32_t mCapacity;
};

#endif