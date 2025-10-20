#pragma once
#include <list>
#include <vector>
#include <bitset>
#include <functional>

#include "TextureManager.h"

#include "define.h"
#include "ResizableBitset.h"

template<typename T>
class LockerSimple {
private:
    std::list<T> maLockArray;
public:
    using Iterator = typename std::list<T>::iterator;
    using ConstIterator = decltype(maLockArray.cbegin());

    static_assert(std::is_copy_constructible<T>::value || std::is_move_constructible<T>::value, "copy or move constructor must be");
    static_assert(std::is_destructible<T>::value || std::is_arithmetic<T>::value, "has to be a destructor or a primitive variable");

    LockerIndex pushInLocker(T&& elem);
    LockerIndex pushInLocker(const T& elem);
    _NODISCARD inline T& operator[] (LockerIndex index) { return *(mapLockPtr[index]); }
    void popFromLocker(LockerIndex index);
    inline Iterator popFromLocker(Iterator it);

    inline bool isEmpty() const noexcept { return maLockArray.empty(); }
    inline void clear() noexcept {
        maLockArray.clear();
        mapLockPtr.clear();
        maOccupied.clear();
        mNearestFreeLocker = 0;
    }

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

    std::vector<Iterator> mapLockPtr;
protected:
    ResBitset maOccupied;

    LockerIndex mNearestFreeLocker = 0;
};

template<typename T>
inline LockerIndex LockerSimple<T>::pushInLocker(T&& elem) {
    if (mNearestFreeLocker == maLockArray.size()) {
        maLockArray.emplace_back(elem);
        if (mapLockPtr.size() == mNearestFreeLocker) {
            mapLockPtr.push_back(--maLockArray.end());
        } else {
            mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
        }
        maOccupied.set(maOccupied.maxcalledbit(), true);
        return mNearestFreeLocker++;
    }

    maLockArray.emplace_back(elem);
    maOccupied.set(mNearestFreeLocker, true);
    mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
    LockerIndex next = mNearestFreeLocker;
    updateLockerStatus();
    return next;
}

template<typename T>
inline LockerIndex LockerSimple<T>::pushInLocker(const T& elem) {
    if (mNearestFreeLocker == maLockArray.size()) {
        maLockArray.emplace_back(elem);
        if (mapLockPtr.size() == mNearestFreeLocker) {
            mapLockPtr.push_back(--maLockArray.end());
        }
        else {
            mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
        }
        maOccupied.set(maOccupied.maxcalledbit(), true);
        return mNearestFreeLocker++;
    }

    maLockArray.emplace_back(elem);
    maOccupied.set(mNearestFreeLocker, true);
    mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
    LockerIndex next = mNearestFreeLocker;
    updateLockerStatus();
    return next;
}

template<typename T>
inline void LockerSimple<T>::popFromLocker(LockerIndex index) {
    if (index >= maLockArray.size()) return;

    maOccupied.set(index, false);
    maLockArray.erase(mapLockPtr[index]);
    mapLockPtr[index]._Ptr = nullptr;
    mNearestFreeLocker = index;
}

template<typename T>
inline typename LockerSimple<T>::Iterator LockerSimple<T>::popFromLocker(Iterator it) {
    auto indx = std::distance(maLockArray.begin(), it);

    maOccupied.set(indx, false);
    auto ret_it = maLockArray.erase(it);
    mapLockPtr[indx]._Ptr = nullptr;
    mNearestFreeLocker = std::min<LockerIndex>(indx, mNearestFreeLocker);

    return ret_it;
}

template<typename T>
inline void LockerSimple<T>::updateLockerStatus() {
    for (mNearestFreeLocker; mNearestFreeLocker < maLockArray.size(); mNearestFreeLocker++)
        if (!maOccupied.get(mNearestFreeLocker)) break;
}
