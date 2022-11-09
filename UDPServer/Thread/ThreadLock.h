#ifndef _THREAD_LOCK_H_
#define _THREAD_LOCK_H_

#include "CommonDefine.h"
#include "Utility.h"
#include "Array.h"

class ThreadLock : public Utility
{
public:
	ThreadLock():
		mCurLock(false),
		mEnable(true)
	{
		mLock.clear(std::memory_order_release);
#if _DEBUG
		mFile[0] = '\0';
#endif
	}
	virtual ~ThreadLock() = default;
	void waitForUnlock(const char* file, const int line);
	void unlock();
	void setEnable(const bool enable)	{ mEnable = enable; }
	bool isEnable() const				{ return mEnable; }
	bool isLocked() const				{ return mCurLock.load(); }
public:
	volatile atomic_flag mLock;			// true表示锁定,false表示未锁定
	volatile atomic<bool> mCurLock;		// true表示锁定,false表示未锁定
	volatile atomic<bool> mEnable;		// true表示线程锁启用,false表示线程锁被禁用
	volatile int mLine = 0;				// 加锁的行号
	volatile int mThreadID = 0;			// 加锁线程的ID
#if _DEBUG
	Array<256> mFile{0};				// 加锁的文件名
#endif
};

#endif