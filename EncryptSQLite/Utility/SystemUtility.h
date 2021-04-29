#ifndef _SYSTEM_UTILITY_H_
#define _SYSTEM_UTILITY_H_

#include "FileUtility.h"

class SystemUtility : public FileUtility
{
public:
	static void sleep(ulong timeMS);
	// 获取系统从启动到现在所经过的毫秒,每帧更新一次的
	static ulong getTimeMS() { return mTimeMS; }
	static void setTimeMS(ulong timeMS) { mTimeMS = timeMS; }
	// 获取系统从启动到现在所经过的毫秒,实时的
	static ulong getRealTimeMS();
	// 获取当前时间的周几(从1开始),年(从1970开始),月(从1开始),天(从1开始),时(从0开始),分,秒
	static void getTime(int& weekDay, int& year, int& month, int& day, int& hour, int& minute, int& second);
	// 获取当前时间的年,月,天,时,分,秒
	static void getTime(int& year, int& month, int& day, int& hour, int& minute, int& second);
	// 获取当前时间的时,分,秒
	static void getTime(int& hour, int& minute, int& second);
	// 获取当前时间,以字符串形式表示
	static string getTime(bool timeStamp = false);
	// 获取从1970年1月1日到现在的秒数
	static ullong getTimeSecond() { return time(NULL); }
	// 获得今天是周几
	static int getDayOfWeek();
	// 判断两次时间戳是否是在同一天
	static bool isSameDay(const time_t& timeStamp0, const time_t& timeStamp1);
	// 获得cpu核心数
	static int getCPUCoreCount();
	static string getIPFromSocket(const MY_SOCKET& socket);
	static void print(const char* str, bool nextLine = true);
	static void input(string& str);
	static uint getThreadID()
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		return GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_LINUX
		return pthread_self();
#endif
	}
	// 获得程序当前所在路径,带文件名
	static string getProgramFile()
	{
		array<char, 256> name{ 0 };
#if RUN_PLATFORM == PLATFORM_WINDOWS
		GetModuleFileNameA(NULL, name.data(), (uint)name.size());
#elif RUN_PLATFORM == PLATFORM_LINUX
		ssize_t ret = readlink("/proc/self/exe", name.data(), name.size());
		if (ret == -1)
		{
			return "";
		}
#endif
		return name.data();
	}
	// 参数为绝对路径,并且在windows上需要将所有的'/'转换为'\\'
	static bool launchExe(const string& path, const string& fullName);
	// 参数为文件名,不带路径,不带后缀
	static bool isExeRunning(const string& fileName);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static void getAllProcess(mySet<string>& processList);
#elif RUN_PLATFORM == PLATFORM_LINUX
	static void getPidFromStr(const char* str, char* pid, uint size);
#endif
	static ullong makeID() { return ++mIDSeed; }
	static string getStackTrace(int depth);
	static bool isMainThread() { return getThreadID() == mMainThread; }
	static void setMainThread(uint id) { mMainThread = id; }
protected:
	static ullong mIDSeed;
	static ulong mTimeMS;		// 系统从启动到现在所经过的毫秒,每帧获取一次,避免频繁获取造成性能下降
	static uint mMainThread;
};

#endif
