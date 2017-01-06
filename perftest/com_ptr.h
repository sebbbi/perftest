#pragma once

template <typename T>
class com_ptr
{
public:
	com_ptr() : ptr(nullptr) {}
	com_ptr(T* ptr) : ptr(ptr) {}
	com_ptr(const com_ptr<T>& other) = delete;
	
	com_ptr(com_ptr<T>&& other)
	{
		ptr = other.ptr;
		other.ptr = nullptr;
	}

	~com_ptr()
	{
		if (ptr)
			ptr->Release();
	}

	T& operator*() { return *ptr; }
	T* operator->() { return ptr; }
	T** operator&() { return &ptr; }
	const T& operator*() const { return *ptr; }
	void operator=(T *ptr) { this->ptr = ptr; }
	operator T*() const { return ptr; }

private:
	T* ptr;
};
