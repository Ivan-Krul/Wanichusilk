#pragma once
#include <list>
#include <vector>
#include <bitset>
#include <functional>

#include "TextureManager.h"

#include "define.h"
#include "ResizableBitset.h"

typedef int LockerIndex;

template<class T, typename Cont>
class Locker {
private: 
    std::list<T> maLockArray;
public:
    using checkFunctionPtr = bool(*)(T&, Cont);
    using Iterator = decltype(maLockArray.begin());
    using ConstIterator = decltype(maLockArray.cbegin());

    static_assert(std::is_default_constructible<T>::value, "default constructor must be, empty one");
    static_assert(std::is_destructible<T>::value || std::is_arithmetic<T>::value, "has to be a destructor or a primitive variable");

    Locker(checkFunctionPtr check_create);

    LockerIndex pushInLocker(Cont container);
    inline T& operator[] (LockerIndex index) { return *(mapLockPtr[index]); }
    void popFromLocker(LockerIndex index);

    inline size_t getCapacity() const { return maLockArray.size(); }
    inline size_t getOccuipedLocks() const {
        size_t count = 0;
        for (size_t i = 0; i < maLockArray.size(); i++) count += maOccupied.get(i);
        return count;
    }

    inline Iterator begin() { return maLockArray.begin(); }
    inline Iterator end() { return maLockArray.end(); }

    inline ConstIterator cbegin() const { return maLockArray.cbegin(); }
    inline ConstIterator cend() const { return maLockArray.cend(); }
private:
    void updateLockerStatus();

    checkFunctionPtr mfCheckCreateRaw;

    
    std::vector<Iterator> mapLockPtr;
protected:
    ResBitset maOccupied;

    LockerIndex mNearestFreeLocker = 0;
};

template<class T, typename Cont>
inline Locker<T, Cont>::Locker(checkFunctionPtr check_create) {
    mfCheckCreateRaw = check_create;
}

template<class T, typename Cont>
inline LockerIndex Locker<T, Cont>::pushInLocker(Cont container) {
    if (mNearestFreeLocker == maLockArray.size()) {
        maLockArray.emplace_back();
        if (!mfCheckCreateRaw(maLockArray.back(), container)) {
            maLockArray.pop_back();
            return -1;
        }
        mapLockPtr.push_back(--maLockArray.end());
        maOccupied.set(maOccupied.maxcalledbit(), true);
        return mNearestFreeLocker++;
    }

    maLockArray.emplace_back();
    if (!mfCheckCreateRaw(maLockArray.back(), container)) {
        return -1;
    }
    maOccupied.set(mNearestFreeLocker, true);
    mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
    LockerIndex next = mNearestFreeLocker;
    updateLockerStatus();
    return next;
}

template<class T, typename Cont>
inline void Locker<T, Cont>::popFromLocker(LockerIndex index) {
    if (index >= maLockArray.size()) return;

    maOccupied.set(index, false);
    maLockArray.erase(mapLockPtr[index]);
    mapLockPtr[index]._Ptr = nullptr;
    mNearestFreeLocker = index;
}

template<class T, typename Cont>
inline void Locker<T, Cont>::updateLockerStatus() {
    for (mNearestFreeLocker; mNearestFreeLocker < maLockArray.size(); mNearestFreeLocker++)
        if (!maOccupied.get(mNearestFreeLocker)) break;
}
