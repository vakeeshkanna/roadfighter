#ifndef ARRAY_CLASS_H
#define ARRAY_CLASS_H
#include <assert.h>
#include <iostream>

template <class T>
class EXPORT Array
{
public:
	Array(int size = 10);
	~Array();
	void init(int sz);
	int getSize() const;
	T& operator[](int);
	
private:
	int size;
	T *ptr;
};

template <class T>
Array<T>::Array(int sz)
{
	size = sz;
	ptr = new T[size];
	assert(ptr != 0);
}

template <class T>
Array<T>::~Array()
{
	if(ptr != NULL)
		delete []ptr;
	ptr = NULL;
}

template <class T>
void Array<T>::init(int sz)
{
	size = sz;
	ptr = new T[size];
	assert(ptr != 0);
}

template <class T>
int Array<T>::getSize() const
{
	return size;
}

template <class T>
T& Array<T>::operator[](int subscript)
{
	assert(subscript >= 0 && subscript < size);
	return ptr[subscript];
}
#endif
