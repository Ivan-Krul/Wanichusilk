#pragma once
#include <utility>
#include "Text.h"

#include "define.h"
#include "ResizableBitset.h"

typedef short LockerIndex;

template<typename T = int>
class Locker {
public:
    static_assert(std::is_copy_constructible<T>::value || std::is_move_constructible<T>::value, "copy or move constructor must be");
    static_assert(std::is_destructible<T>::value || std::is_arithmetic<T>::value, "has to be a destructor or a primitive variable");

    LockerIndex pushInLocker(T&& elem);
    LockerIndex pushInLocker(const T& elem);
    _NODISCARD inline T& operator[] (LockerIndex index) { return mapData[index]; }
    _NODISCARD inline const T& at(LockerIndex index) const { return mapData[index]; }
    void popFromLocker(LockerIndex index);

private:
    void allocate();

    LockerIndex mCapacity = 0;
    LockerIndex mSize = 0;
    T* mapData = nullptr;
    LockerIndex* mapIndexes = nullptr;
    LockerIndex* mapIndexIndexes = nullptr;

};

template<typename T>
inline LockerIndex Locker<T>::pushInLocker(T&& elem) {
    if (mSize == mCapacity) allocate();

    new (mapData[mSize]) T(std::move(elem));

    return mapIndexIndexes[mSize++];
}

template<typename T>
inline LockerIndex Locker<T>::pushInLocker(const T& elem) {
    if (mSize == mCapacity) allocate();

    new (mapData[mSize]) T(elem);

    return mapIndexIndexes[mSize++];
}

template<typename T>
inline void Locker<T>::popFromLocker(LockerIndex index) {
    LockerIndex from = mSize - 1;

    mapData[index].~T();

    memcpy(mapData + index, mapData + from, sizeof(T));

    from = mapIndexIndexes[from];
    mapIndexIndexes[from] = index;
    mapIndexIndexes[index] = from;

}

template<typename T>
inline void Locker<T>::allocate() {
    mCapacity = (!mCapacity) ? 16 : mCapacity << 1;
    
    T* newData = new T[mCapacity];
    LockerIndex* newIndex = new LockerIndex[mCapacity];
    LockerIndex* newIndexIndexes = new LockerIndex[mCapacity];

    memcpy(newData, mapData, mSize * sizeof(T));
    memcpy(newIndex, mapIndexes, mSize * sizeof(LockerIndex));
    memcpy(newIndexIndexes, mapIndexIndexes, mSize * sizeof(LockerIndex));

    for (LockerIndex i = mSize; i < mCapacity; i++) {
        mapIndexes[i] = i;
        mapIndexIndexes[i] = i;
    }

    delete[] mapData;
    mapData = newData;

    delete[] mapIndexes;
    mapIndexes = newIndex;

    delete[] mapIndexIndexes;
    mapIndexIndexes = newIndexIndexes;
}

