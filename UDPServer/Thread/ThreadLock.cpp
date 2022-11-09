#include "ThreadLock.h"

void ThreadLock::waitForUnlock(const char* file, const int line)
{
	if (!mEnable)
	{
		return;
	}
	// 原子自旋操作
	// 如果mLock当前为true,则设置为true,并继续等待,如果mLock为false,则设置为true,不再循环等待
	while (mLock.test_and_set(std::memory_order_acquire))
	{
		sleep(0);
	}
	mCurLock = true;
#if _DEBUG
	int length = strlen(file) + 1;
	if (length >= mFile.size())
	{
		length = mFile.size() - 1;
	}
	MEMCPY(mFile.toBuffer(), mFile.size(), file, length);
	mFile[length - 1] = 0;
#endif
	mLine = line;
	mThreadID = getThreadID();
}

void ThreadLock::unlock()
{
	if (!mEnable)
	{
		return;
	}
	mCurLock = false;
	mLock.clear(std::memory_order_release);
}