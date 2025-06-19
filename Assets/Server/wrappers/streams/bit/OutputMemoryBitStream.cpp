#include <streams/bit/OutputMemoryBitStream.h>

#include <stdlib.h>
#include <algorithm>

OutputMemoryBitStream::OutputMemoryBitStream() {
    ReallocBuffer(256);
}

OutputMemoryBitStream::~OutputMemoryBitStream() {
    free(mBuffer);
}

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount) {
    uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
    if (nextBitHead > mBitCapacity) {
        ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
    }

    uint32_t byteOffset = mBitHead >> 3;
    uint32_t bitOffset = mBitHead & 0x7;

    uint8_t currentMask = ~(0xff << bitOffset);
    mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);
    
    uint32_t bitsFreeThisByte = 8 - bitOffset;

    if (bitsFreeThisByte < inBitCount) {
        mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
    }

    mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount) {
    const char* srcByte = static_cast<const char*>(inData);

    while (inBitCount > 8) {
        WriteBits(*srcByte, 8);
        srcByte += 1;
        inBitCount -= 8;
    }

    if (inBitCount > 0) {
        WriteBits(*srcByte, inBitCount);
    }
}

const char* OutputMemoryBitStream::GetBufferPtr() const {
    return mBuffer;
}

uint32_t OutputMemoryBitStream::GetBitLenght() const {
    return mBitHead;
}

uint32_t OutputMemoryBitStream::GetByteLenght() const {
    return (mBitHead + 7) >> 3;
}

void OutputMemoryBitStream::WriteBytes(const void* inData, size_t inByteCount) {
    return WriteBits(inData, inByteCount << 3);
}

template<typename T> 
void OutputMemoryBitStream::Write(T inData, size_t inBitCount = sizeof(T) * 8) {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types");
    WriteBits(&inData, inBitCount);
}

void OutputMemoryBitStream::Write(bool inData) {
    WriteBits(&inData, 1);
}