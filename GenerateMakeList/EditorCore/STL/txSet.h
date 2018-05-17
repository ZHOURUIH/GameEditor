#ifndef _TX_SET_H_
#define _TX_SET_H_

#include "txSTLBase.h"

template<typename T>
class txSet : public txSTLBase
{
public:
	typedef typename std::set<T>::iterator iterator;
public:
	txSet(){}
	virtual ~txSet(){ clear(); }
	iterator find(const T& elem) const
	{
		return mSet.find(elem);
	}
	iterator begin() const
	{
		return mSet.begin();
	}
	iterator end() const
	{
		return mSet.end();
	}
	std::pair<iterator, bool> insert(const T& elem)
	{
		checkLock();
		return mSet.insert(elem);
	}
	void erase(const iterator& iter)
	{
		checkLock();
		mSet.erase(iter);
	}
	void clear()
	{
		checkLock();
		mSet.clear();
	}
	int size() const
	{
		return mSet.size();
	}
protected:
	std::set<T> mSet;
};

#endif