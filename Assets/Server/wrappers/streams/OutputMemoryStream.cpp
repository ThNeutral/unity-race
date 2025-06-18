#include <wrappers/streams/OutputMemoryStream.h>

#include <stdlib.h>
#include <algorithm>

OutputMemoryStream::OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0) {
    ReallocBuffer(32);
}

OutputMemoryStream::~OutputMemoryStream() {
    free(mBuffer);
}

const char* OutputMemoryStream::GetBufferPtr() const {
    return mBuffer;
}

uint32_t OutputMemoryStream::GetLength() const {
    return mHead;
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength) {
    // TODO: handle realloc error
    mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
    mCapacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount) {
    uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);
    if (resultHead > mCapacity) {
        ReallocBuffer(std::max(mCapacity * 2, resultHead));
    }

    std::memcpy(mBuffer + mHead, inData, inByteCount);
    mHead = resultHead;
}

void OutputMemoryStream::Write(uint32_t inData) {
    return Write(&inData, sizeof(inData));
}

void OutputMemoryStream::Write(int32_t inData) {
    return Write(&inData, sizeof(inData));
}