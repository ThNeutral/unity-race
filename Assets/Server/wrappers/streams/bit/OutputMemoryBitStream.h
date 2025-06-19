#ifndef OUTPUT_MEMORY_BIT_STREAM_H
#define OUTPUT_MEMORY_BIT_STREAM_H

#include <stdlib.h>
#include <stdint.h>

class OutputMemoryBitStream {
    public: 
        OutputMemoryBitStream();
        ~OutputMemoryBitStream();

        void WriteBits(uint8_t inData, size_t inBitCount);
        void WriteBits(const void * inData, size_t inBitCount);

        const char* GetBufferPtr() const;
        uint32_t GetBitLenght() const;
        uint32_t GetByteLenght() const;

        void WriteBytes(const void* inData, size_t inByteCount);
    
        template<typename T>
        void Write(T inData, size_t inBitCount = sizeof(T) * 8);
        
        void Write(bool inData);
    
    private:
        void ReallocBuffer(uint32_t inNewBitCapacity);

        char* mBuffer;
        uint32_t mBitHead;
        uint32_t mBitCapacity;
};      

#endif