#ifndef _TX_MAP_H_
#define _TX_MAP_H_

#include "txSTLBase.h"

template<typename Key, typename Value>
class txMap : public txSTLBase
{
public:
	typedef typename std::map<Key, Value>::iterator iterator;
	typedef typename std::map<Key, Value>::reverse_iterator reverse_iterator;
public:
	txMap(){}
	virtual ~txMap(){ clear(); }
	reverse_iterator rbegin(){return mMap.rbegin();}
	reverse_iterator rend(){return mMap.rend();}
	iterator begin(){return mMap.begin();}
	iterator end(){return mMap.end();}
	const Value& tryGet(const Key& k, const Value& defaultValue)
	{
		iterator iter = find(k);
		if (iter != end())
		{
			return iter->second;
		}
		else
		{
			return defaultValue;
		}
	}
	void trySet(const Key& k, const Value& value)
	{
		auto iter = find(k);
		if (iter != end())
		{
			iter->second = value;
		}
		else
		{
			insert(k, value);
		}
	}
	iterator find(const Key& k){return mMap.find(k);}
	bool contains(const Key& key){return mMap.find(key) != mMap.end();}
	std::pair<iterator, bool> insert(const Key& k, const Value& v)
	{
		checkLock();
		return mMap.insert(std::make_pair(k, v));
	}
	iterator tryInsert(const Key& k, const Value& value)
	{
		auto iter = find(k);
		if (iter == end())
		{
			iter = insert(k, value).first;
		}
		return iter;
	}
	void erase(iterator iter, bool check = true)
	{
		if (check)
		{
			checkLock();
		}
		mMap.erase(iter);
	}
	// 返回值表示移除成功或失败
	bool tryErase(const Key& key, bool check = true)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			erase(iter);
			return true;
		}
		return false;
	}
	void clear()
	{
		checkLock();
		mMap.clear();
	}
	int size(){return (int)mMap.size();}
	Value& operator[](const Key& k){return mMap[k];}
protected:
	std::map<Key, Value> mMap;
};

#endif