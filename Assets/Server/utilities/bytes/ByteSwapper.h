#ifndef BYTE_SWAPPER_H
#define BYTE_SWAPPER_H

#include <stdlib.h>
#include <types/TypeAliaser.h>

template <typename T, size_t tSize> 
class ByteSwapper;

template <typename T>
T ByteSwap(T inData) {
    return ByteSwapper<T, sizeof(T)>().Swap(inData);
}

template <typename T>
class ByteSwapper<T, 2ul> {
    public:
        T Swap(T inData) const {
            uint16_t result = ByteSwap2(TypeAliaser<T, uint16_t>(inData).Get());
            return TypeAliaser<uint16_t, T>(result).Get();
        }
};

template <typename T>
class ByteSwapper<T, 4ul> {
    public:
        T Swap(T inData) const {
            uint32_t result = ByteSwap4(TypeAliaser<T, uint32_t>(inData).Get());
            return TypeAliaser<uint32_t, T>(result).Get();
        }
};

template <typename T>
class ByteSwapper<T, 8ul> {
    public:
        T Swap(T inData) const {
            uint64_t result = ByteSwap8(TypeAliaser<T, uint64_t>(inData).Get());
            return TypeAliaser<uint64_t, T>(result).Get();
        }
};

#endif