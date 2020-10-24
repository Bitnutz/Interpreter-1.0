#pragma once
#include <cassert>

template<typename T>
class stack
{
public:
	stack() :capacity(4), size(0)
	{
		data = new T[capacity];
	}
	stack(const stack &other) { copy(other); }
	stack& operator=(const stack &other)
	{
		if (this != &other)
		{
			del();
			copy(other);
		}
		return *this;
	}
	~stack()
	{
		del();
	}

	bool isEmpty()const
	{
		return !size;
	}
	T peek()const
	{
		assert(!isEmpty());
		return data[size - 1];
	}
	int getSize()const
	{
		return size;
	}
	
	void push(const T& element)
	{
		if (size == capacity)
			resize();
		data[size++] = element;
	}
	T pop()
	{
		assert(!isEmpty());
		return data[--size];
	}

private:
	int size;
	int capacity;
	T* data;
	void copy(const stack& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];
		for (int i = 0; i < size; i++)
		{
			data[i] = other.data[i];
		}
	}
	void del() { delete[]data; }
	void resize()
	{
		capacity *= 2;
		T* temp = data;
		data = new T[capacity];
		for (int i = 0; i < size; i++)
		{
			data[i] = temp[i];
		}
		delete[]temp;
	}

};