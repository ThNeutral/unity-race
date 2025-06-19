#include <streams/byte/InputMemoryStream.h>
#include <error/ErrorCodes.h>
#include <logger/Logger.h>

#include <stdlib.h>
#include <string>

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inByteCount) : mBuffer(inBuffer), mCapacity(inByteCount), mHead(0) {}

InputMemoryStream::~InputMemoryStream() {
    free(mBuffer);
}

uint32_t InputMemoryStream::GetRemainingDataSize() const {
    return mCapacity - mHead;
}

int InputMemoryStream::Read(void* outData, uint32_t inByteCount) {
    if (inByteCount >= mHead) {
        Logger::ReportError("InputMemoryStream::Read", -1, "tried to read more data than exists in buffer");
        return STREAM_ERROR;
    }

    memcpy(outData, mBuffer + (mHead - inByteCount), inByteCount);
    mHead -= inByteCount;

    return NO_ERROR;
}

int InputMemoryStream::Read(uint32_t& outData) {
    return Read(&outData, sizeof(uint32_t));
}

int InputMemoryStream::Read(int32_t& outData) {
    return Read(&outData, sizeof(int32_t));
}