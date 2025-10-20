#pragma once
#include "Locker.h"

template<class T, typename Hash, typename Cont>
class LockerSigned : public Locker<T,Cont> {
public:
    inline LockerSigned(std::function<bool(T&, Cont)> check_create) : Locker<T, Cont>(check_create) {}

    inline LockerIndex pushInLockerSigned(Hash name, Cont container);
    inline LockerIndex getNameIndexed(Hash name);
    inline void popFromLockerSigned(LockerIndex index);

    inline void clearHash();

    virtual ~LockerSigned() = default;
private:

    std::vector<Hash> mNames;
};

template<class T, typename Hash, typename Cont>
inline LockerIndex LockerSigned<T, Hash, Cont>::pushInLockerSigned(Hash name, Cont container) {
    LockerIndex index = pushInLocker(container);
    if (index == -1) return -1;
    if (index == mNames.size())
        mNames.emplace_back(name);
    else
        mNames[index] = name;

    return index;
}

template<class T, typename Hash, typename Cont>
inline LockerIndex LockerSigned<T, Hash, Cont>::getNameIndexed(Hash name) {
    LockerIndex indx = 0;
    for (indx; indx < mNames; indx++) {
        if (!maOccupied[indx]) continue;

        if (name == mNames[indx]) return indx;
    }
    return -1;
}

template<class T, typename Hash, typename Cont>
inline void LockerSigned<T, Hash, Cont>::popFromLockerSigned(LockerIndex index) {
    popFromLocker(index);
    mNames[mNearestFreeLocker] = Hash();
}
