#pragma once
#include <type_traits>
#include <list>

namespace test_list {
	struct A {
		int a;
		int b;
	};

	struct C : public A{
		int d;
	};
}

// structure, that let store huge variety of huge objects in heap

template<typename TBase>
class PolyPointerList {
	static_assert(!std::is_pointer<TBase>::value, "The template must be not a pointer");
	static_assert(std::is_class<TBase>::value, "The template must be at some point a class or struct");
public:
	struct Node {
		Node* next = nullptr;
		Node* prev = nullptr;
		size_t typehash = 0;
		size_t size = 0;
		TBase* pocket = nullptr;
	};

	class Iterator {
	private:
		Node* ptr = nullptr;

	public:
		inline TBase* get() const noexcept { return &(ptr->pocket); }
		inline TBase* operator*() const noexcept { return get(); }
		inline TBase* operator->() const noexcept { return get(); }
		inline Iterator& operator++() noexcept { ptr = ptr->next; return *this; }
		inline Iterator& operator--() noexcept { ptr = ptr->prev; return *this; }
    inline Iterator operator++(int) noexcept { ptr = ptr->next; return *this; }
		inline Iterator operator--(int) noexcept { ptr = ptr->prev; return *this; }
	};
public:
	template<typename T>
	typename std::enable_if<std::is_base_of<TBase, T>::value>::type push_back(const T& value);
	template<typename T>
	typename std::enable_if<std::is_base_of<TBase, T>::value>::type emplace_back(T&& value);

	const size_t size() const noexcept { return mSize; }

	

	// 
	// Set pointer to root
	// while(pointer != null) {
	//     temp = pointer->next;
	//     delete[] pointer;
	//     pointer = temp;
	// }
	// 


private:
	Node* pBegin = nullptr;
	Node* pReverseBegin = nullptr;

	size_t mSize = 0;

};

template<typename TBase>
template<typename T>
inline typename std::enable_if<std::is_base_of<TBase, T>::value>::type PolyPointerList<TBase>::push_back(const T& value) {
	Node* iter = pBegin;

	if (iter != nullptr) {
		while (iter->next != nullptr) {
			iter = iter->next;
		}
	}

	Node* temp = new char[sizeof(size_t) * 2 + sizeof(Node*) * 2 + sizeof(T)]; // we knot the variety by size values in node near the memory to not mess with calls

	temp->typehash = typeid(T);
	temp->size = sizeof(T);
	temp->prev = iter;

	// we need to mess with 2 level pointers
	memcpy(&(temp->pocket), &value, sizeof(T));
	
	iter->next = temp;
	pReverseBegin = temp;
}
