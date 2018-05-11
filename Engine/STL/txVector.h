#ifndef _TX_VECTOR_H_
#define _TX_VECTOR_H_

#include "txSTLBase.h"

template<typename T>
class txVector : public txSTLBase
{
public:
	typedef typename std::vector<T>::iterator iterator;
public:
	txVector()
	{}
	virtual ~txVector(){ clear(); }
	void push_back(const T& elem)
	{
		checkLock();
		mVector.push_back(elem);
	}
	iterator erase(const iterator& iter, const bool& check = true)
	{
		if (check)
		{
			checkLock();
		}
		return mVector.erase(iter);
	}
	iterator erase(const int& index, const bool& check = true)
	{
		if (index < 0 || index >= size())
		{
			return end();
		}
		return erase(begin() + index);
	}
	void clear()
	{
		checkLock();
		mVector.clear();
	}
	void insert(const iterator& iter, const T& elem)
	{
		checkLock();
		mVector.insert(iter, elem);
	}
	int size() const
	{
		return mVector.size();
	}
	iterator begin()
	{
		return mVector.begin();
	}
	iterator end()
	{
		return mVector.end();
	}
	const T& operator[](const int& i) const
	{
		if (i < 0 || i >= size())
		{
#if RUN_PLATFORM == PLATFORM_WINDOWS
			MessageBoxA(NULL, "error : vector index out of range!", "", MB_OK);
#elif RUN_PLATFORM == PLATFORM_ANDROID
			LOGI("error : vector index out of range!");
#endif
		}
		return mVector[i];
	}
	T& operator[](const int& i)
	{
		if (i < 0 || i >= size())
		{
#if RUN_PLATFORM == PLATFORM_WINDOWS
			MessageBoxA(NULL, "error : vector index out of range!", "", MB_OK);
#elif RUN_PLATFORM == PLATFORM_ANDROID
			LOGI("error : vector index out of range!");
#endif
		}
		return mVector[i];
	}
	void resize(const int& s)
	{
		checkLock();
		mVector.resize(s);
	}
protected:
protected:
	std::vector<T> mVector;
};

#endif