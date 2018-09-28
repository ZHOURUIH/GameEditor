#include "SystemUtility.h"

ThreadLock SystemUtility::mTimeLock;

void SystemUtility::stop()
{
	;
}

void SystemUtility::sleep(unsigned long timeMS)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	Sleep(timeMS);
#elif RUN_PLATFORM == PLATFORM_LINUX
	usleep(timeMS * 1000);
#endif
}

long SystemUtility::getTimeMS()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
	struct timeval tv;
	if(gettimeofday(&tv, NULL) != 0)
	{
		LOG_ERROR("time get error : %d", errno);
	}
	return tv.tv_sec * 1000 + (long)(tv.tv_usec / 1000.0f);
#endif
}

const char* SystemUtility::getTime()
{
	static char timeBuffer[128] = { 0 };
	LOCK(mTimeLock);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	SPRINTF(timeBuffer, 128, "%d月%d日%d时%d分%d秒%d毫秒", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#elif RUN_PLATFORM == PLATFORM_LINUX
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	SPRINTF(timeBuffer, 64, "%d月%d日%d时%d分%d秒", date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
#endif
	UNLOCK(mTimeLock);
	return timeBuffer;
}