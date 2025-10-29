#pragma once
#include <type_traits>
#include <list>

namespace test_list {
	struct A {
		int a;
		int b;
	};

	struct C : public A {
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

		inline       void* object_ptr()       { return reinterpret_cast<void*>(this + 1); }
		inline const void* object_ptr() const { return reinterpret_cast<const void*>(this + 1); }

		inline       TBase* object()       { return reinterpret_cast<TBase*>(object_ptr()); }
		inline const TBase* object() const { return reinterpret_cast<const TBase*>(object_ptr()); }
	};

	class Iterator {
	private:
		Node* ptr;

	public:
		friend class PolyPointerList<TBase>;

		inline explicit Iterator(Node* node) : ptr(node) {}
		inline TBase* get() const noexcept { return ptr->object(); }
		inline TBase* operator*() const noexcept { return get(); }
		inline TBase* operator->() const noexcept { return get(); }
		inline Iterator& operator++() noexcept { ptr = ptr->next; return *this; }
		inline Iterator& operator--() noexcept { ptr = ptr->prev; return *this; }
		inline Iterator operator++(int) noexcept { 
			auto old = *this;
			ptr = ptr->next; return old;
		}
		inline Iterator operator--(int) noexcept {
			auto old = *this;
			ptr = ptr->prev; return old;
		}
		inline Iterator operator+(int n) const noexcept {
			Iterator temp(*this);
			if (n > 0)
				while (n-- && temp.ptr) temp.ptr = temp.ptr->next;
			else if (n < 0)
				while (n++ && temp.ptr) temp.ptr = temp.ptr->prev;
			return temp;
		}
		inline Iterator operator-(int n) const noexcept {
			return (*this) + (-n);
		}
		inline bool match(const std::type_info& info) { return ptr && info.hash_code() == ptr->typehash; }
		inline bool operator==(const Iterator& iter) const noexcept { return iter.ptr == ptr; }
		inline bool operator!=(const Iterator& iter) const noexcept { return iter.ptr != ptr; }
	};
public:
	template<typename T, typename... Args>
	typename std::enable_if<std::is_base_of<TBase, T>::value>::type emplace_back(Args&&... args);
	template<typename T>
	inline typename std::enable_if<std::is_base_of_v<TBase, T>>::type
		push_back(const T& value) { emplace_back<T>(value); }

	template<typename T, typename... Args>
	typename std::enable_if<std::is_base_of<TBase, T>::value>::type emplace_front(Args&&... args);
	template<typename T>
	inline typename std::enable_if<std::is_base_of_v<TBase, T>>::type
		push_front(const T& value) { emplace_front<T>(value); }

	template<typename T>
	typename std::enable_if<std::is_base_of<TBase, T>::value, Iterator>::type insert(Iterator pos, const T& value);

	Iterator erase(Iterator pos);

	inline const size_t size() const noexcept { return mSize; }
	inline const bool empty() const noexcept { return !mSize; }

	inline Iterator begin() noexcept { return Iterator(pBegin); }
	inline Iterator end() noexcept { return Iterator(nullptr); }
	inline Iterator rbegin() noexcept { return Iterator(pReverseBegin); }
	inline Iterator rend() noexcept { return Iterator(nullptr); }
	
	inline Iterator cbegin() const noexcept { return Iterator(pBegin); }
	inline Iterator cend() const noexcept { return Iterator(nullptr); }
	inline Iterator crbegin() const noexcept { return Iterator(pReverseBegin); }
	inline Iterator crend() const noexcept { return Iterator(nullptr); }

	~PolyPointerList();

private:
	Node* pBegin = nullptr;
	Node* pReverseBegin = nullptr;

	size_t mSize = 0;

};

template<typename TBase>
template<typename T, typename... Args>
inline typename std::enable_if<std::is_base_of<TBase, T>::value>::type PolyPointerList<TBase>::emplace_back(Args&&... args) {
	void* raw = ::operator new(sizeof(Node) + sizeof(T)); // we knot the variety by size values in node near the memory to not mess with calls

	Node* temp = new (raw) Node();
	temp->typehash = typeid(T).hash_code();
	temp->size = sizeof(T);

	// we need to mess with 2 level pointers
	new (temp->object_ptr()) T(std::forward<Args>(args)...);

	mSize++;
	if (!pBegin) {
		pBegin = pReverseBegin = temp;
		return;
	}

	pReverseBegin->next = temp;
	temp->prev = pReverseBegin;
	pReverseBegin = temp;
}

template<typename TBase>
template<typename T, typename ...Args>
inline typename std::enable_if<std::is_base_of<TBase, T>::value>::type PolyPointerList<TBase>::emplace_front(Args && ...args) {
	void* raw = ::operator new(sizeof(Node) + sizeof(T)); // we knot the variety by size values in node near the memory to not mess with calls

	Node* temp = new (raw) Node();
	temp->typehash = typeid(T).hash_code();
	temp->size = sizeof(T);

	// we need to mess with 2 level pointers
	new (temp->object_ptr()) T(std::forward<Args>(args)...);

	mSize++;
	if (!pReverseBegin) {
		pBegin = pReverseBegin = temp;
		return;
	}

	pBegin->prev = temp;
	temp->next = pBegin;
	pBegin = temp;
}

template<typename TBase>
template<typename T>
inline typename std::enable_if<std::is_base_of<TBase, T>::value, PolyPointerList<TBase>::Iterator>::type PolyPointerList<TBase>::insert(Iterator pos, const T& value) {
	void* raw = ::operator new(sizeof(Node) + sizeof(T)); // we knot the variety by size values in node near the memory to not mess with calls

	Node* temp = new (raw) Node();
	temp->typehash = typeid(T).hash_code();
	temp->size = sizeof(T);

	// we need to mess with 2 level pointers
	new (temp->object_ptr()) T(std::forward<Args>(args)...);

	mSize++;
	Node* ptr_beg = pos.ptr;
	Node* ptr_end = pos.ptr->next;

	if (!pBegin) {
		pBegin = pReverseBegin = temp;
		return begin();
	}
	if (ptr_beg) {
		ptr_beg->next = temp;
		temp->next = ptr_end;
	}
	if (ptr_end) {
		ptr_end->prev = temp;
		temp->prev = ptr_beg;
	}

	return Iterator(temp);
}

template<typename TBase>
inline PolyPointerList<TBase>::Iterator PolyPointerList<TBase>::erase(Iterator pos) {
	Node* ptr_beg = pos.ptr->prev;
	Node* ptr_end = pos.ptr->next;

	pos.ptr->object()->~TBase();
	::operator delete(pos.ptr);

	if(ptr_beg) ptr_beg->next = ptr_end;
	if(ptr_end) ptr_end->next = ptr_beg;

	mSize--;
	return Iterator(ptr_end);
}

template<typename TBase>
inline PolyPointerList<TBase>::~PolyPointerList() {
	Node* node = pBegin;
	while (node) {
		Node* next = node->next;

		node->object()->~TBase();
		::operator delete(node);

		node = next;
	}
}
