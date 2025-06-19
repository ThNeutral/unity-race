#include <types/TypeAliaser.h>

template <typename tFrom, typename tTo>
TypeAliaser<tFrom, tTo>::TypeAliaser(tFrom inFromValue) : mAsFromType(inFromValue) {}

template <typename tFrom, typename tTo>
tTo& TypeAliaser<tFrom, tTo>::Get() {
    return mAsToType;
}
