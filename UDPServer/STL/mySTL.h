#ifndef _MY_STL_H_
#define _MY_STL_H_

#include <string>
#include <atomic>
#include "IndependentLog.h"

typedef unsigned char byte;
using std::atomic;
using std::string;

#if _DEBUG
enum class STL_LOCK : byte
{
	NONE,
	READ,
	WRITE,
};
#endif

class mySTL
{
public:
	mySTL() = default;
	mySTL(const mySTL& other)
	{
#if _DEBUG
		mLock.exchange(other.mLock);
		mReadLockCount.exchange(other.mReadLockCount);
		mLine.exchange(other.mLine);
		mFile.exchange(other.mFile);
#endif
	}
	mySTL& operator=(const mySTL& other)
	{
#if _DEBUG
		mLock.exchange(other.mLock);
		mReadLockCount.exchange(other.mReadLockCount);
		mLine.exchange(other.mLine);
		mFile.exchange(other.mFile);
#endif
		return *this;
	}
	virtual ~mySTL() = default;
#if _DEBUG
	// 循环遍历列表之前必须锁定
	void lock(const STL_LOCK lockType, const char* file, const int line);
	// 循环遍历列表结束以后必须解锁
	void unlock(const STL_LOCK lockType);
#endif
protected:
#if _DEBUG
	void checkLock() const;
	void resetLock();
#endif
protected:
#if _DEBUG
	volatile atomic<STL_LOCK> mLock = STL_LOCK::NONE;
	volatile atomic<int> mReadLockCount = 0;	// 读锁定的次数,读锁定可以叠加,计算读锁定的次数,当读锁定次数为0时,取消锁定
	volatile atomic<const char*> mFile = nullptr;
	volatile atomic<int> mLine = 0;
#endif
};

#endif