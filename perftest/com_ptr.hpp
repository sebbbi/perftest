#pragma once

template <typename T>
struct com_ptr
{
	T* ptr;

	com_ptr() : ptr(nullptr) {}
	com_ptr(T *ptr) : ptr(ptr) {}
	com_ptr(const com_ptr<T> &other) = delete;

	~com_ptr()
	{
		ptr->Release();
	}

	T& operator*() { return *ptr; }
	T* operator->() { return ptr; }
	const T& operator*() const { return *ptr; }
	void operator=(T *ptr) { this->ptr = ptr; }
};
