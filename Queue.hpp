#pragma once
#include <iostream>
#include <cassert>

template <typename T>
class Queue
{
public:
	Queue() :front(0), back(-1), size(0), capacity(16)
	{
		data = new T[capacity];
	}
	Queue(const Queue& other)
	{
		copy(other);
	}
	Queue& operator=(const Queue& other)
	{
		if (this != &other)
		{
			del();
			copy(other);
		}
		return *this;
	}
	~Queue()
	{
		delete[]data;
	}

	bool isEmpty()const
	{
		return !size;
	}

	void enqueue(const T &element)
	{
		if ((back + 1) % capacity == front)
			resize();
		size++;
		back = (back+1) % capacity;
		data[back] = element;
	}
	T dequeue()
	{
		assert(!isEmpty());
		size--;
		front = (front + 1) % capacity;
		return data[front-1];
	}
	T& peek()const
	{
		assert(!isEmpty());
		return data[front];
	}
	int getSize()const
	{
		return size;
	}


private:
	int front;
	int back;
	int size;
	int capacity;
	T* data;
	//Private functions
	void copy(const Queue& other)
	{
		size = other.size;
		front = 0;
		back = size - 1;
		capacity = other.capacity;
		data = new T[capacity];
		for (int i = 0; i < size; i++)
		{
			data[i] = other.data[(other.front + i) % capacity];
		}
	}
	void del()
	{
		delete[]data;
	}
	void resize()
	{
		capacity *= 2;
		front = 0;
		back = size - 1;
		T* temp = data;
		data = new T[capacity];
		for (int i = 0; i < size; i++)
		{
			data[i] = temp[(front + i) % capacity];
		}
		delete[]temp;
	}
};
