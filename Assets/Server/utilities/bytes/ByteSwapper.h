#ifndef BYTE_SWAPPER_H
#define BYTE_SWAPPER_H

#include <stdlib.h>

template <typename T, size_t tSize> 
class ByteSwapper;

template <typename T>
class ByteSwapper<T, 2> {
    public:
        T Swap(T inData) const;
};

template <typename T>
class ByteSwapper<T, 4> {
    public:
        T Swap(T inData) const;
};

template <typename T>
class ByteSwapper<T, 8> {
    public:
        T Swap(T inData) const;
};

template <typename T>
T ByteSwap(T inData) {
    return ByteSwapper<T, sizeof(T)>().Swap(inData);
}

#endif