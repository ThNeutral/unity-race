#include <streams/bit/OutputMemoryBitStream.h>

#include <stdlib.h>
#include <algorithm>
#include <logger/Logger.h>

OutputMemoryBitStream::OutputMemoryBitStream() : mBuffer(nullptr), mBitHead(0), mBitCapacity(0) {
    ReallocBuffer(256);
}

OutputMemoryBitStream::~OutputMemoryBitStream() {
    free(mBuffer);
}

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount) {
    uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
    if (nextBitHead > mBitCapacity) {
        ReallocBuffer(std::max(mBitCapacity * 2, (nextBitHead >> 3) + 1));
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

    while (inBitCount >= 8) {
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

uint32_t OutputMemoryBitStream::GetBitCapacity() const {
    return mBitCapacity;
}

uint32_t OutputMemoryBitStream::GetByteCapacity() const {
    return (mBitCapacity + 7) >> 3;
}

void OutputMemoryBitStream::WriteBytes(const void* inData, size_t inByteCount) {
    return WriteBits(inData, inByteCount << 3);
}

void OutputMemoryBitStream::Write(bool inData) {
    WriteBits(&inData, 1);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewLength) {
    char* newBuffer = static_cast<char*>(std::realloc(mBuffer, (inNewLength + 7) >> 3));
    if (newBuffer == nullptr) {
        Logger::ReportFatal("OutputMemoryStream::ReallocBuffer", -1, "realloc failed", std::bad_alloc());
    }
    
    mBuffer = newBuffer;
    mBitCapacity = inNewLength;
}