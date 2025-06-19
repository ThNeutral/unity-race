#include <streams/bit/InputMemoryBitStream.h>

#include <stdlib.h>
#include <logger/Logger.h>

InputMemoryBitStream::InputMemoryBitStream(char* inData) : mBuffer(inData) {}

InputMemoryBitStream::~InputMemoryBitStream() {
    free(mBuffer);
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, size_t inBitCount) {
    if (inBitCount >= mBitHead) {
        Logger::ReportFatal("InputMemoryBitStream::ReadBits", -1, "tried to read more data than exists in buffer");
    }

    uint32_t byteOffset = mBitHead >> 3;
    uint32_t bitOffset = mBitHead & 0x7;

    uint8_t currentByte = mBuffer[byteOffset];
    uint32_t bitsFreeThisByte = 8 - bitOffset;

    uint8_t mask = ~(0xFF << inBitCount);

    if (bitsFreeThisByte >= inBitCount) {
        outData = (currentByte >> bitOffset) & mask;
    } else {
        uint8_t nextByte = mBuffer[byteOffset + 1];
        uint16_t combined = (nextByte << 8) | currentByte;
        combined >>= bitOffset;

        outData = combined & mask;
    }

    mBitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, size_t inBitCount) {
    char* buf = static_cast<char*>(outData);
    
    while (inBitCount > 8) {
        ReadBits(buf, 8);
        buf += 1;
        inBitCount -= 8;
    }

    if (inBitCount > 0) {
        ReadBits(buf, inBitCount);
    }
}

const char* InputMemoryBitStream::GetBufferPtr() const {
    return mBuffer;
}

uint32_t InputMemoryBitStream::GetBitLenght() const {
    return mBitHead;
}

uint32_t InputMemoryBitStream::GetByteLenght() const {
    return (mBitHead) >> 3;
}

void InputMemoryBitStream::ReadBytes(void* outData, size_t inByteCount) {
    ReadBits(outData, inByteCount * 8);
}

void InputMemoryBitStream::Read(bool& outData) {
    ReadBits(&outData, 1);
}

template<typename T> 
void InputMemoryBitStream::Read(T& outData, size_t inBitCount = sizeof(T) * 8) {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Read only supports primitive data types");
    ReadBits(&outData, inBitCount);
}