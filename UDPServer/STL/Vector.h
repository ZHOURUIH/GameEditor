#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "mySTL.h"
#include "Array.h"
#include <vector>

using std::vector;
using std::initializer_list;
using std::move;

template<typename T>
class Vector : public mySTL
{
public:
	typedef typename vector<T>::iterator iterator;
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::const_iterator const_iterator;
public:
	Vector() = default;
	explicit Vector(initializer_list<T> _Ilist)
	{
		mVector.insert(mVector.begin(), _Ilist);
	}
	~Vector() override { clear(); }
	T* data() const					{ return (T*)mVector.data(); }
	int size() const				{ return (int)mVector.size(); }
	iterator begin()				{ return mVector.begin(); }
	iterator end()					{ return mVector.end(); }
	reverse_iterator rbegin()		{ return mVector.rbegin(); }
	reverse_iterator rend()			{ return mVector.rend(); }
	const_iterator cbegin() const	{ return mVector.cbegin(); }
	const_iterator cend() const		{ return mVector.cend(); }
	// 返回最后一个元素,并且将该元素移除,如果当前列表为空,则返回defaultValue
	T popBack(const T& defaultValue, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		int curSize = size();
		if (curSize == 0)
		{
			return defaultValue;
		}
		const T value = mVector[curSize - 1];
		mVector.erase(mVector.begin() + curSize - 1);
		return value;
	}
	void addRange(const T* values, const int count, const bool check = true)
	{
		if (values == nullptr || count == 0)
		{
			return;
		}
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		const int oldSize = (int)mVector.size();
		mVector.resize(oldSize + count);
		MEMCPY(mVector.data() + oldSize, count * sizeof(T), values, count * sizeof(T));
	}
	void addRange(const Vector<T>& values, const bool check = true)
	{
		if (values.size() == 0)
		{
			return;
		}
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		const int count = (int)values.size();
		const int oldSize = (int)mVector.size();
		mVector.resize(oldSize + count);
		MEMCPY(mVector.data() + oldSize, count * sizeof(T), values.data(), count * sizeof(T));
	}
	template<size_t Length>
	void setData(const Array<Length, T>& values, const int count, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.clear();
		mVector.reserve(count);
		FOR_I(count)
		{
			mVector.push_back(values[i]);
		}
	}
	void setData(const T* values, const int count, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.clear();
		mVector.reserve(count);
		FOR_I(count)
		{
			mVector.push_back(values[i]);
		}
	}
	void push_back()
	{
		mVector.push_back(mDefaultValue);
	}
	void addUnique(const T& elem)
	{
		if (!contains(elem))
		{
			push_back(elem);
		}
	}
	void push_back(const T& elem, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.push_back(move(elem));
	}
	void push_back(const T& elem0, const T& elem1, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		reserve(size() + 2);
		mVector.push_back(move(elem0));
		mVector.push_back(move(elem1));
	}
	void push_back(const T& elem0, const T& elem1, const T& elem2, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		reserve(size() + 3);
		mVector.push_back(move(elem0));
		mVector.push_back(move(elem1));
		mVector.push_back(move(elem2));
	}
	void push_back(const T& elem0, const T& elem1, const T& elem2, const T& elem3, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		reserve(size() + 4);
		mVector.push_back(move(elem0));
		mVector.push_back(move(elem1));
		mVector.push_back(move(elem2));
		mVector.push_back(move(elem3));
	}
	void push_back(const T& elem0, const T& elem1, const T& elem2, const T& elem3, const T& elem4, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		reserve(size() + 5);
		mVector.push_back(move(elem0));
		mVector.push_back(move(elem1));
		mVector.push_back(move(elem2));
		mVector.push_back(move(elem3));
		mVector.push_back(move(elem4));
	}
	void push_back(const T& elem0, const T& elem1, const T& elem2, const T& elem3, const T& elem4, const T& elem5, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		reserve(size() + 6);
		mVector.push_back(move(elem0));
		mVector.push_back(move(elem1));
		mVector.push_back(move(elem2));
		mVector.push_back(move(elem3));
		mVector.push_back(move(elem4));
		mVector.push_back(move(elem5));
	}
	iterator erase(const iterator& iter, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		return mVector.erase(iter);
	}
	iterator erase(const iterator& iter, const iterator& end, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		return mVector.erase(iter, end);
	}
	iterator eraseAt(int index, int count = 1, const bool check = true)
	{
#if _DEBUG
		if (index >= (int)mVector.size())
		{
			return mVector.end();
		}
		return erase(mVector.begin() + index, mVector.begin() + index + count, check);
#else
		return mVector.erase(mVector.begin() + index, mVector.begin() + index + count);
#endif
	}
	void eraseAll(const T& value, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		for (int i = 0; i < (int)mVector.size(); )
		{
			if (mVector[i] == value)
			{
				mVector.erase(mVector.begin() + i);
			}
			else
			{
				++i;
			}
		}
	}
	bool eraseElement(const T& value, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		const int count = (int)mVector.size();
		FOR_I(count)
		{
			if (mVector[i] == value)
			{
				mVector.erase(mVector.begin() + i);
				return true;
			}
		}
		return false;
	}
	void clear()
	{
#if _DEBUG
		checkLock();
#endif
		if (mVector.size() > 0)
		{
			mVector.clear();
		}
	}
	void merge(const Vector<T>& other)
	{
		auto& otherVector = other.mVector;
		const int count = (int)otherVector.size();
		reserve(size() + count);
		FOR_I(count)
		{
#if _DEBUG
			push_back(other[i]);
#else
			mVector.push_back(otherVector[i]);
#endif
		}
	}
	void insert(int index, const T& elem, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.insert(mVector.begin() + index, move(elem));
	}
	void insert(const iterator& iter, const T& elem, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.insert(iter, move(elem));
	}
	const T& operator[](const int i) const
	{
		if (i < 0 || i >= (int)mVector.size())
		{
			// 因为函数为const属性,但是directError不是const属性,所以此处调用directError会报错
			//directError("vector index out of range!");
		}
		return mVector[i];
	}
	// 根据下标获取元素,如果下标不合法则返回默认值
	const T& get(const int index)
	{
		if (index < 0 || index >= (int)mVector.size())
		{
			return mDefaultValue;
		}
		return mVector[index];
	}
	const T& get(const int index, const T& defaultValue)
	{
		if (index < 0 || index >= (int)mVector.size())
		{
			return defaultValue;
		}
		return mVector[index];
	}
	T& operator[](const int i)
	{
		if (i < 0 || i >= (int)mVector.size())
		{
			IndependentLog::directError("vector index out of range!");
		}
		return mVector[i];
	}
	// 同时修改capacity和size
	void resize(const int s, const bool check = true)
	{
#if _DEBUG
		if (check)
		{
			checkLock();
		}
#endif
		mVector.resize(s);
	}
	// 只增加capacity,不改变当前size,如果当前的最大容量已经大于等于要设置的容量则不作任何事情
	void reserve(int capacity)
	{
		mVector.reserve(capacity);
	}
	bool contains(const T& value) const
	{
		return std::find(mVector.begin(), mVector.end(), value) != mVector.end();
	}
	int find(const T& value, int startIndex = 0)
	{
		const int count = (int)mVector.size();
		for(int i = startIndex; i < count; ++i)
		{
			if (mVector[i] == value)
			{
				return i;
			}
		}
		return -1;
	}
	void clone(Vector<T>& target) const
	{
		const int curSize = (int)mVector.size();
		auto& temp = target.mVector;
		temp.clear();
		temp.resize(curSize);
		FOR_I(curSize)
		{
			temp[i] = mVector[i];
		}
#if _DEBUG
		target.resetLock();
#endif
	}
	void shrink_to_fit()
	{
		mVector.shrink_to_fit();
	}
public:
	vector<T> mVector;
	static const Vector<T> mDefaultList;
private:
	static T mDefaultValue;
};

template<typename T>
T Vector<T>::mDefaultValue;

template<typename T>
const Vector<T> Vector<T>::mDefaultList;

#endif