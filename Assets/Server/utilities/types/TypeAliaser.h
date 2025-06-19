#ifndef TYPE_ALIASER_H
#define TYPE_ALIASER_H

template <typename tFrom, typename tTo>
class TypeAliaser {
    public:
        TypeAliaser(tFrom inFromValue);
        tTo& Get();

        union {
            tFrom mAsFromType;
            tTo mAsToType;
        };
};

#endif