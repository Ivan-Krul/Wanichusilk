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
    using Iterator = typename std::list<T>::iterator;
    using ConstIterator = decltype(maLockArray.cbegin());

    static_assert(std::is_default_constructible<T>::value, "default constructor must be, empty one");
    static_assert(std::is_destructible<T>::value || std::is_arithmetic<T>::value, "has to be a destructor or a primitive variable");

    Locker(checkFunctionPtr check_create) : mfCheckCreateRaw(check_create) {}

    LockerIndex pushInLocker(Cont container);
    _NODISCARD inline T& operator[] (LockerIndex index) { return *(mapLockPtr[index]); }
    _NODISCARD inline const T& at(LockerIndex index) const { return *(mapLockPtr.at(index)); }
    void popFromLocker(LockerIndex index);
    Iterator popFromLocker(Iterator it);

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

    virtual ~Locker() = default;
private:
    void updateLockerStatus();

    const checkFunctionPtr mfCheckCreateRaw;
    
    std::vector<Iterator> mapLockPtr;
protected:
    ResBitset maOccupied;

    LockerIndex mNearestFreeLocker = 0;
};

template<class T, typename Cont>
inline LockerIndex Locker<T, Cont>::pushInLocker(Cont container) {
    if (mNearestFreeLocker == maLockArray.size()) {
        maLockArray.emplace_back();
        if (!mfCheckCreateRaw(maLockArray.back(), container)) {
            maLockArray.pop_back();
            return -1;
        }
        if(mNearestFreeLocker == mapLockPtr.size())
            mapLockPtr.push_back(--maLockArray.end());
        else
            mapLockPtr[mNearestFreeLocker] = --maLockArray.end();
        maOccupied.set_lastcalled(true);
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
inline typename Locker<T, Cont>::Iterator Locker<T, Cont>::popFromLocker(Iterator it) {
    auto indx = std::distance(maLockArray.begin(), it);

    maOccupied.set(indx, false);
    auto ret_it = maLockArray.erase(it);
    mapLockPtr[indx]._Ptr = nullptr;
    mNearestFreeLocker = std::min(indx, mNearestFreeLocker);

    return ret_it;
}

template<class T, typename Cont>
inline void Locker<T, Cont>::updateLockerStatus() {
    for (mNearestFreeLocker; mNearestFreeLocker < maLockArray.size(); mNearestFreeLocker++)
        if (!maOccupied.get(mNearestFreeLocker)) break;
}
