#ifndef _TX_THREAD_LOCK_H_
#define _TX_THREAD_LOCK_H_

#include "EngineDefine.h"

// 线程加锁类型
enum LOCK_TYPE
{
	LT_READ,		// 锁定后需要进行读取
	LT_WRITE,		// 锁定后需要进行写入
};
#define LOCK(l, flag) l.waitForUnlock(__FILE__, __LINE__, flag)
#define UNLOCK(l, flag) l.unlock(flag)

class txThreadLock
{
public:
	txThreadLock()
	{
		mLock = false;
		mLine = 0;
		mRead = LT_WRITE;
		mReadLockCount = 0;
		mThreadID = 0;
		mShowLockError = 0;
	}
	void waitForUnlock(const char* file, const int& line, const LOCK_TYPE& read, const bool& showDebug = false);
	void unlock(const LOCK_TYPE& read, const bool& showDebug = false);
public:
	volatile bool mLock;			// 是否锁定
	volatile char mFile[256];		// 加锁的文件名
	volatile int mLine;				// 加锁的行号
	volatile int mThreadID;			// 加锁线程的ID
	volatile LOCK_TYPE mRead;		// 锁定后是想要读(true)或者写(false)
	volatile int mReadLockCount;	// 锁定读的次数,当锁定读的次数为0时才能完全解锁
	volatile int mShowLockError;
};

#endif