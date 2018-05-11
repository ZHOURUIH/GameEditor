#include "ThreadLock.h"
#include "Utility.h"

void ThreadLock::waitForUnlock(const char* file, const int& line)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_ANDROID
	int threadID = 0;
#endif
	// 暂不区分读锁定和写锁定
	while (InterlockedExchange(&mLock, 1) == 1)
	{
		txUtility::sleep(1);
	}
	memset((char*)mFile, 0, 256);
	memcpy((char*)mFile, file, strlen(file));
	mLine = line;
	mThreadID = threadID;
}

void ThreadLock::unlock()
{
	InterlockedExchange(&mLock, 0);
}