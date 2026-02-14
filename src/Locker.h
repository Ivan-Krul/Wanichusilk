#pragma once
#include <utility>

#include "define.h"

typedef int LockerIndex;

// based on index/id/data access
//
// index:         [2][0][1][] as constant as possible
// 
// index indexes: [1][2][0][] as binded with data as possible
// 
// data:   [data-1][data-2][data-0][]
// 
//

template<typename T>
class Locker {
public:
    static_assert(std::is_move_constructible<T>::value, "template must have a move constructor");
    static_assert(std::is_destructible<T>::value, "template must have a destructor");

public:
    class Iterator {
    public:
        friend class Locker<T>;

        explicit inline Iterator(T* ptr) : data(ptr) {}
        inline T* get() const noexcept { return data; }
        inline T* operator*() const noexcept { return get(); }
        inline T* operator->() const noexcept { return get(); }
        inline Iterator& operator++() noexcept { data++; return *this; }
        inline Iterator& operator--() noexcept { data--; return *this; }
        inline Iterator& operator++(int) noexcept { data++; return *this; }
        inline Iterator& operator--(int) noexcept { data--; return *this; }
        inline Iterator& operator+(int n) noexcept {
            data += n;
            return *this;
        }
        inline Iterator operator-(int n) noexcept {
            data -= n;
            return *this;
        }
        inline bool operator==(const Iterator& iter) const noexcept { return iter.data == data; }
        inline bool operator!=(const Iterator& iter) const noexcept { return iter.data != data; }
    private:
        T* data;
    };
public:

    LockerIndex pushInLocker(T&& elem);
    LockerIndex pushInLocker(const T& elem);
    _NODISCARD inline T& operator[] (LockerIndex index) { return mapData[index]; }
    _NODISCARD inline const T& at(LockerIndex index) const { return mapData[index]; }
    void popFromLocker(LockerIndex index);
    Iterator popFromLocker(Iterator it);

    inline bool isEmpty() const noexcept { return mSize == 0; }
    inline void clear() noexcept {
        for (LockerIndex i = 0; i < mSize; i++) {
            mapData[i].~T();
            mapIndexes[i] = i;
            mapIndexIndexes[i] = i;
        }
    }

    inline Iterator begin() { return Iterator(mapData); }
    inline Iterator end() { return Iterator(mapData + mSize); }

    inline Iterator cbegin() { return Iterator(mapData); }
    inline Iterator cend() { return Iterator(mapData + mSize); }

    ~Locker();
private:
    void allocate();

    T* mapData = nullptr;
    LockerIndex* mapIndexes = nullptr;
    LockerIndex* mapIndexIndexes = nullptr;
    LockerIndex mSize = 0;
    LockerIndex mCapacity = 0;

};

template<typename T>
inline LockerIndex Locker<T>::pushInLocker(T&& elem) {
    static_assert(std::is_move_constructible<T>::value, "must be move constructor");
    if (mSize == mCapacity) allocate();

    new ((void*)(mapData + mSize)) T(std::move(elem));

    return mapIndexIndexes[mSize++];
}

template<typename T>
inline LockerIndex Locker<T>::pushInLocker(const T& elem) {
    static_assert(std::is_move_constructible<T>::value, "must be copy constructor");
    if (mSize == mCapacity) allocate();

    new ((void*)(mapData + mSize)) T(elem);

    return mapIndexIndexes[mSize++];
}

template<typename T>
inline void Locker<T>::popFromLocker(LockerIndex index) {
    assert(index != -1);
    assert(index < mSize);

    mapData[index].~T();
    new ((void*)(mapData + index)) T(std::move(mapData[mSize - 1]));

    LockerIndex from = mapIndexIndexes[mSize - 1];
    mapIndexIndexes[mSize - 1] = index;
    mapIndexIndexes[index] = from;
    mSize--;
}

template<typename T>
inline typename Locker<T>::Iterator Locker<T>::popFromLocker(Iterator it) {
    if (it == end()) return it;
    assert(it.data >= mapData);
    assert(it.data < end().data);

    it.data->~T();
    new ((void*)it.data) T(std::move(mapData[mSize - 1]));

    LockerIndex index = (mapData - it.data) / sizeof(T);
    LockerIndex from = mapIndexIndexes[mSize - 1];
    mapIndexIndexes[mSize - 1] = index;
    mapIndexIndexes[index] = from;
    mSize--;
    return Iterator(it.data);
}

template<typename T>
inline Locker<T>::~Locker() {
    delete[] mapData;
    delete[] mapIndexes;
    delete[] mapIndexIndexes;
}

template<typename T>
inline void Locker<T>::allocate() {
    mCapacity = (!mCapacity) ? 16 : mCapacity << 1;
    
    T* newData = new T[mCapacity];
    LockerIndex* newIndex = new LockerIndex[mCapacity];
    LockerIndex* newIndexIndexes = new LockerIndex[mCapacity];

    for (LockerIndex i = 0; i < mCapacity; i++) {
        if (i < mSize) {
            new ((void*)(newData + i)) T(std::move(mapData[i]));
            newIndex[i] = mapIndexes[i];
            newIndexIndexes[i] = mapIndexIndexes[i];
        }
        else {
            newIndex[i] = i;
            newIndexIndexes[i] = i;
        }
    }

    delete[] mapData; // inline destruct invoking, but it has to be empty
    mapData = newData;

    delete[] mapIndexes;
    mapIndexes = newIndex;

    delete[] mapIndexIndexes;
    mapIndexIndexes = newIndexIndexes;
}


