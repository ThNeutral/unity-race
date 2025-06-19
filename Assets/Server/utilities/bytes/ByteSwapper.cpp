#include <bytes/ByteSwapper.h>

#include <bytes/BytesSwaps.h>

template <typename T>
T ByteSwapper<T, 2>::Swap(T) const {
    uint16_t result = ByteSwap2(TypeAliaser<T, uint16_t>(inData).Get());
    return TypeAliaser<uint16_t, T>(result).Get()
} 

template <typename T>
T ByteSwapper<T, 4>::Swap(T) const {
    uint32_t result = ByteSwap2(TypeAliaser<T, uint32_t>(inData).Get());
    return TypeAliaser<uint32_t, T>(result).Get()
} 

template <typename T>
T ByteSwapper<T, 8>::Swap(T) const {
    uint64_t result = ByteSwap2(TypeAliaser<T, uint64_t>(inData).Get());
    return TypeAliaser<uint64_t, T>(result).Get()
} 