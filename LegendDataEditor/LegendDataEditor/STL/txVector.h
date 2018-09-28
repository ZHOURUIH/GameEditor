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
	iterator erase(const iterator& iter, bool check = true)
	{
		if (check)
		{
			checkLock();
		}
		return mVector.erase(iter);
	}
	iterator erase(int index, bool check = true)
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
	void insert(const iterator& iter, const T& elem, bool check = true)
	{
		if (check)
		{
			checkLock();
		}
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
	const T& operator[](int i) const
	{
		if (i < 0 || i >= size())
		{
			//LOG_ERROR("%s", "vector index out of range!");
		}
		return mVector[i];
	}
	T& operator[](int i)
	{
		if (i < 0 || i >= size())
		{
			//LOG_ERROR("%s", "vector index out of range!");
		}
		return mVector[i];
	}
	void resize(int s)
	{
		checkLock();
		mVector.resize(s);
	}
	bool contains(const T& value)
	{
		return std::find(begin(), end(), value) != end();
	}
protected:
protected:
	std::vector<T> mVector;
};

#endif