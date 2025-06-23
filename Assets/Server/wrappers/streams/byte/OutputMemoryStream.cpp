#include <streams/byte/OutputMemoryStream.h>
#include <logger/Logger.h>

#include <stdlib.h>
#include <algorithm>
#include <exception>

OutputMemoryStream::OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0), mLinkingContext() {
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

uint32_t OutputMemoryStream::GetCapacity() const {
    return mCapacity;
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength) {
    char* newBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
    if (newBuffer == nullptr) {
        Logger::ReportFatal("OutputMemoryStream::ReallocBuffer", -1, "realloc failed", std::bad_alloc());
    }
    
    mBuffer = newBuffer;
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