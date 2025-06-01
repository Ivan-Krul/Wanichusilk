#pragma once
#include <vector>
#include <bitset>
#include <functional>

#include "define.h"
#include "Texture.h"

typedef size_t LockerIndex;

template<class T, typename Cont>
class Locker {
public:
    Locker(std::function<bool(T&, Cont)> check_create);

    LockerIndex pushInLocker(Cont container);
    inline T& operator[] (LockerIndex index) { return maLockArray[index]; }
    void popFromLocker(LockerIndex index);

    inline size_t getCapacity() const { return maLockArray.size(); }
    inline size_t getOccuipedLocks() const {
        size_t count = 0;
        for (size_t i = 0; i < maLockArray.size(); i++) count += maOccupied[i];
        return count;
    }

private:
    void updateLockerStatus();

    std::function<bool(T&, Cont)> mfCheckCreate;

    std::vector<T> maLockArray;
    std::vector<bool> maOccupied;

    LockerIndex mNearestFreeLocker = 0;
};

template<class T, typename Cont>
inline Locker<T, Cont>::Locker(std::function<bool(T&, Cont)> check_create) {
    mfCheckCreate = check_create;
}

template<class T, typename Cont>
inline LockerIndex Locker<T, Cont>::pushInLocker(Cont container) {
    static_assert(std::is_default_constructible<T>::value, "Default constructor must be, empty one");
    if (mNearestFreeLocker == maLockArray.size()) {
        maLockArray.emplace_back();
        if (!mfCheckCreate(maLockArray.back(), container)) {
            maLockArray.pop_back();
            return -1;
        }
        maOccupied.push_back(true);
        return mNearestFreeLocker++;
    }

    maLockArray[mNearestFreeLocker] = T();
    if (!mfCheckCreate(maLockArray[mNearestFreeLocker], container)) {
        return -1;
    }
    maOccupied[mNearestFreeLocker] = true;
    LockerIndex next = mNearestFreeLocker;
    updateLockerStatus();
    return next;
}

template<class T, typename Cont>
inline void Locker<T, Cont>::updateLockerStatus() {
    for (mNearestFreeLocker; mNearestFreeLocker < maLockArray.size(); mNearestFreeLocker++)
        if (!maOccupied[mNearestFreeLocker]) break;
}
