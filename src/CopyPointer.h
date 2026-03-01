#pragma once
#include <type_traits>
#include <utility>

// smart pointer, that copies stuff in it's instance and holds it
// it's not friendly when the deal comes with copying instances which was derived

template<typename T>
class CopyPointer {
public:
    static_assert(!std::is_pointer<T>::value, "CopyPointer already contains pointer. Pointers on pointers are prohibited");
    static_assert(!std::is_array<T>::value, "CopyPointer doesn't support array allocation. Use other class instead");

public:
    inline CopyPointer() = default;
    inline CopyPointer(std::nullptr_t) {}
    CopyPointer(T* manage_ptr); // let the pointer manage this memory
    template<typename P>
    CopyPointer(P* manage_ptr);
    CopyPointer(const T& copy); // let the pointer hold a memory
    template<typename P>
    CopyPointer(const P& copy);
    CopyPointer(T&& move);      // let the pointer take the memory
    template<typename P>
    CopyPointer(P&& move);
    CopyPointer(CopyPointer&& inst);
    CopyPointer(const CopyPointer& inst) = delete;

    inline CopyPointer& operator=(CopyPointer<T>&& inst) noexcept { mpPtr = inst.mpPtr; inst.mpPtr = nullptr; return *this; }
    inline CopyPointer& operator=(std::nullptr_t) noexcept { if (mpPtr) delete mpPtr; mpPtr = nullptr; return *this; }
    inline CopyPointer& operator=(const CopyPointer<T>&) noexcept = delete;

    inline typename std::enable_if<std::is_copy_constructible<T>::value>::type copy(const T& item) { if (mpPtr) delete mpPtr; mpPtr = new T(item); }
    inline typename std::enable_if<std::is_move_constructible<T>::value>::type move(T&& item) { if (mpPtr) delete mpPtr; mpPtr = new T(std::move(item)); }
    template<typename P> inline typename std::enable_if<std::is_copy_constructible<P>::value && std::is_base_of<T, P>::value>::type copy(const P& item) { if (mpPtr) delete mpPtr; mpPtr = new P(item); }
    template<typename P> inline typename std::enable_if<std::is_move_constructible<P>::value && std::is_base_of<T, P>::value>::type move(P&& item) { if (mpPtr) delete mpPtr; mpPtr = new P(std::move(item)); }
    inline T* get() const noexcept { return mpPtr; }
    inline void swap(CopyPointer<T>& inst) { T* ptr = inst.mpPtr; inst.mpPtr = mpPtr; mpPtr = ptr; }
    inline T* release() noexcept { T* tmp = mpPtr; mpPtr = nullptr; return tmp; }

    inline explicit operator bool() { return mpPtr; }
    inline T* operator->() const noexcept { return mpPtr; }
    inline T* operator*() const noexcept { return mpPtr; }
    inline bool operator==(const CopyPointer<T>& comp) const noexcept { return mpPtr == comp.mpPtr; }
    inline bool operator!=(const CopyPointer<T>& comp) const noexcept { return mpPtr != comp.mpPtr; }

    inline ~CopyPointer() { delete mpPtr; }

private:
    T* mpPtr = nullptr;
};

template<typename T>
inline CopyPointer<T>::CopyPointer(T* manage_ptr) {
    mpPtr = manage_ptr;
}

template<typename T> template<typename P>
inline CopyPointer<T>::CopyPointer(P* manage_ptr) {
    static_assert(std::is_base_of<T, P>::value, "the pointer type has to be derived from base");

    mpPtr = manage_ptr;
}

template<typename T>
inline CopyPointer<T>::CopyPointer(const T& copy) {
    static_assert(std::is_copy_constructible<T>::value, "the pointer type has to have a copy constructor");
    // using regular memcpy is dangerous with it's inner modules and dependancies
    mpPtr = new T(copy);
}

template<typename T> template<typename P>
inline CopyPointer<T>::CopyPointer(const P& copy) {
    static_assert(std::is_base_of<T, P>::value, "the pointer type has to be derived from base");
    static_assert(std::is_copy_constructible<P>::value, "the pointer type has to have a copy constructor");

    mpPtr = new P(copy);
}

template<typename T>
inline CopyPointer<T>::CopyPointer(T&& move) {
    static_assert(std::is_move_constructible<T>::value, "the pointer type has to have a move constructor");

    mpPtr = new T(std::move(move));
}

template<typename T> template<typename P>
inline CopyPointer<T>::CopyPointer(P&& move) {
    static_assert(std::is_base_of<T, P>::value, "the pointer type has to be derived from base");
    static_assert(std::is_move_constructible<T>::value, "the pointer type has to have a move constructor");

    mpPtr = new P(std::move(P));
}

template<typename T>
inline CopyPointer<T>::CopyPointer(CopyPointer&& inst) {
    mpPtr = inst.mpPtr;
    inst.mpPtr = nullptr;
}
