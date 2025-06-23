#ifndef INPUT_MEMORY_STREAM_H
#define INPUT_MEMORY_STREAM_H

#include <stdint.h>
#include <vector>

#include <streams/link/LinkingContext.h>
#include <gameobject/GameObject.h>

class InputMemoryStream {
    public:
        InputMemoryStream(char* inBuffer, uint32_t inByteCount);
        ~InputMemoryStream();

        uint32_t GetRemainingDataSize() const;

        void Read(void* outData, uint32_t inByteCount);
        void Read(uint32_t& outData);
        void Read(int32_t& outData);
        
        void Read(GameObject*& outGameObject) {
            uint32_t id;
            Read(id);
            outGameObject = mLinkingContext->GetGameObject(id);
        }

        template<typename T>
        void Read(T& outData) {
            Read(&outData, sizeof(T));
        }

        template<typename T>
        void Read(std::vector<T>& outVector) {
            uint32_t count;
            Read(count);
            outVector.resize(count);
            for (T& element : outVector) {
                Read(element);
            } 
        }
    private:
        char* mBuffer;
        uint32_t mHead;
        uint32_t mCapacity;

        LinkingContext* mLinkingContext;
};

#endif