#ifndef OUTPUT_MEMORY_STREAM_H
#define OUTPUT_MEMORY_STREAM_H

#include <stdint.h>
#include <stddef.h> 
#include <vector>
#include <streams/link/LinkingContext.h>

class OutputMemoryStream {
    public:
        OutputMemoryStream();
        ~OutputMemoryStream();
        const char* GetBufferPtr() const;
        uint32_t GetLength() const;
        uint32_t GetCapacity() const;

        void Write(const void* inData, size_t inByteCount);
        void Write(uint32_t inData);
        void Write(int32_t inData);

        void Write(const GameObject* inGameObject) {
            uint32_t id = mLinkingContext->GetNetworkID(inGameObject);
            Write(id);
        }

        template<typename T>
        void Write(const T& inData) {
            Write(&inData, sizeof(T));
        }

        template<typename T>
        void Write(const std::vector<T>& inVector) {
            Write((uint32_t)inVector.size());
            for (const T& element : inVector) {
                Write<T>(element);
            } 
        }

    private:
        void ReallocBuffer(uint32_t inNewLength);

        char* mBuffer;
        uint32_t mHead;
        uint32_t mCapacity;

        LinkingContext* mLinkingContext;
};

#endif