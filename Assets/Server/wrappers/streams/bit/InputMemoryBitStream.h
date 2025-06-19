#ifndef INPUT_MEMORY_BIT_STREAM_H
#define INPUT_MEMORY_BIT_STREAM_H

#include <stdlib.h>
#include <stdint.h>

class InputMemoryBitStream {
    public: 
        InputMemoryBitStream(char* inData);
        ~InputMemoryBitStream();

        void ReadBits(uint8_t& outData, size_t inBitCount);
        void ReadBits(void* outData, size_t inBitCount);

        const char* GetBufferPtr() const;
        uint32_t GetBitLenght() const;
        uint32_t GetByteLenght() const;

        void ReadBytes(void* outData, size_t inByteCount);

        template<typename T> 
        void Read(T& outData, size_t inBitCount = sizeof(T) * 8);
        
        void Read(bool& outData);
    
    private:
        void ReallocBuffer(uint32_t inNewBitCapacity);

        char* mBuffer;
        uint32_t mBitHead;
        uint32_t mBitCapacity;
};

#endif