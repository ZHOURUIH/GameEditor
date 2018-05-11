#ifndef _TX_THREAD_LOCK_H_
#define _TX_THREAD_LOCK_H_

#include "EngineDefine.h"

#define LOCK(l) l.waitForUnlock(__FILE__, __LINE__)
#define UNLOCK(l) l.unlock()

class txThreadLock
{
public:
	txThreadLock()
	{
		mLock = 0;
		mLine = 0;
		mThreadID = 0;
	}
	~txThreadLock()
	{
		;
	}
	void waitForUnlock(const char* file, const int& line);
	void unlock();
public:
	volatile unsigned int mLock;	// 1表示锁定,0表示未锁定
	volatile char mFile[256];		// 加锁的文件名
	volatile int mLine;				// 加锁的行号
	volatile int mThreadID;			// 加锁线程的ID
};

#endif