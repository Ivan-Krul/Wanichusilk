#include "Locker.h"

template<class T, typename Cont>
void Locker<T, Cont>::popFromLocker(LockerIndex index) {
    static_assert(std::is_destructible<T>::value || std::is_arithmetic<T>::value, "Has to be a destructor or a primitive variable");

    if (index >= maLockArray.size()) return;

    maOccupied[index] = false;
    maLockArray.erase(mapLockPtr[index]);
    mapLockPtr[index] = nullptr;
    mNearestFreeLocker = index;
}


