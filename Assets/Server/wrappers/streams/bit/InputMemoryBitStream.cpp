#include <streams/bit/InputMemoryBitStream.h>

#include <stdlib.h>
#include <logger/Logger.h>

InputMemoryBitStream::InputMemoryBitStream(char* inData, uint32_t mBitCount) : mBuffer(inData), mBitHead(0), mBitCapacity(mBitCount) {}

InputMemoryBitStream::~InputMemoryBitStream() {
    free(mBuffer);
}

uint32_t InputMemoryBitStream::GetRemainingBitDataSize() const {
    return mBitCapacity - mBitHead;
}

uint32_t InputMemoryBitStream::GetRemainingByteDataSize() const {
    return (GetRemainingBitDataSize() + 7) >> 3;
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, size_t inBitCount) {
    if (inBitCount > GetRemainingBitDataSize()) {
        Logger::ReportFatal("InputMemoryBitStream::ReadBits", "tried to read more data than exists in buffer");
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
    
    while (inBitCount >= 8) {
        uint8_t outData;
        ReadBits(outData, 8);

        *buf = outData; 
        buf += 1;
        inBitCount -= 8;
    }

    if (inBitCount > 0) {
        uint8_t outData;
        ReadBits(outData, inBitCount);

        *buf = outData; 
    }
}

const char* InputMemoryBitStream::GetBufferPtr() const {
    return mBuffer;
}

uint32_t InputMemoryBitStream::GetBitLenght() const {
    return mBitHead;
}

uint32_t InputMemoryBitStream::GetByteLenght() const {
    return (mBitHead + 7) >> 3;
}

void InputMemoryBitStream::ReadBytes(void* outData, size_t inByteCount) {
    ReadBits(outData, inByteCount * 8);
}

void InputMemoryBitStream::Read(bool& outData) {
    ReadBits(&outData, 1);
}