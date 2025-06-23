#ifndef INPUT_MEMORY_BIT_STREAM_H
#define INPUT_MEMORY_BIT_STREAM_H

#include <stdlib.h>
#include <stdint.h>
#include <type_traits>

class InputMemoryBitStream {
    public: 
        InputMemoryBitStream(char* inData, uint32_t mBitCount);
        ~InputMemoryBitStream();

        uint32_t GetRemainingBitDataSize() const;
        uint32_t GetRemainingByteDataSize() const;

        void ReadBits(uint8_t& outData, size_t inBitCount);
        void ReadBits(void* outData, size_t inBitCount);

        const char* GetBufferPtr() const;
        uint32_t GetBitLenght() const;
        uint32_t GetByteLenght() const;

        void ReadBytes(void* outData, size_t inByteCount);

        template<typename T> 
        void Read(T& outData, size_t inBitCount = sizeof(T) * 8) {
            static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Read only supports primitive data types");
            ReadBits(&outData, inBitCount);
        }
        
        void Read(bool& outData);
    
    private:
        char* mBuffer;
        uint32_t mBitHead;
        uint32_t mBitCapacity;
};

#endif