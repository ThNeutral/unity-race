#include <streams/byte/InputMemoryStream.h>
#include <error/ErrorCodes.h>
#include <logger/Logger.h>

#include <stdlib.h>
#include <string>

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inByteCount) : mBuffer(inBuffer), mCapacity(inByteCount), mHead(0), mLinkingContext() {}

InputMemoryStream::~InputMemoryStream() {
    free(mBuffer);
}

uint32_t InputMemoryStream::GetRemainingDataSize() const {
    return mCapacity - mHead;
}

void InputMemoryStream::Read(void* outData, uint32_t inByteCount) {
    if (inByteCount > GetRemainingDataSize()) {
        Logger::ReportFatal("InputMemoryStream::Read", "tried to read more data than exists in buffer");
    }

    memcpy(outData, mBuffer + mHead, inByteCount);
    mHead += inByteCount;
}

void InputMemoryStream::Read(uint32_t& outData) {
    return Read(&outData, sizeof(uint32_t));
}

void InputMemoryStream::Read(int32_t& outData) {
    return Read(&outData, sizeof(int32_t));
}