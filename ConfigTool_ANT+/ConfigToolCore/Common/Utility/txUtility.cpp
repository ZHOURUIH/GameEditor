#include "Utility.h"

ThreadLock txUtility::mTimeLock;

void txUtility::getAllProcess(txSet<std::string>& processList)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄 
	PROCESSENTRY32* info = TRACE_NEW(PROCESSENTRY32, info);
	info->dwSize = sizeof(PROCESSENTRY32);
	//调用一次 Process32First 函数，从快照中获取进程列表 
	Process32First(handle, info);
	//重复调用 Process32Next，直到函数返回 FALSE 为止 
	while (Process32Next(handle, info))
	{
		processList.insert(info->szExeFile);
	}
	TRACE_DELETE(info);
}

bool txUtility::launchExecutable(const std::string& path, const std::string& fullName)
{
	// 启动游戏程序
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = fullName.c_str();
	sei.lpDirectory = path.c_str();
	sei.lpVerb = "open";
#ifndef __WXWINCE__
	sei.nShow = SW_SHOWNORMAL; // SW_SHOWDEFAULT not defined under CE (#10216)
#else
	sei.nShow = SW_SHOWDEFAULT;
#endif
	sei.fMask = SEE_MASK_FLAG_NO_UI;
	return ShellExecuteEx(&sei) == TRUE;
}

void txUtility::setRegistryValue(const std::string& path, const std::string& key, const std::string& value)
{
	HKEY hKey;
	LONG nError = RegOpenKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, KEY_ALL_ACCESS, &hKey);
	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	if (nError != ERROR_SUCCESS)
	{
		return;
	}
	nError = RegSetValueExA(hKey, key.c_str(), NULL, REG_DWORD, (LPBYTE)(value.c_str()), value.length());
	if (nError != ERROR_SUCCESS)
	{
		return;
	}
}

std::string txUtility::getRegistryValue(const std::string& path, const std::string& key)
{
	HKEY hKey;
	LONG nError = RegOpenKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, KEY_ALL_ACCESS, &hKey);
	if (nError != ERROR_SUCCESS)
	{
		return "";
	}
	DWORD type = REG_SZ;
	int size = 1024;
	char value[1024];
	memset(value, 0, size);
	nError = RegQueryValueEx(hKey, key.c_str(), NULL, &type, (LPBYTE)(value), (LPDWORD)&(size));
	if (nError != ERROR_SUCCESS)
	{
		return "";
	}
	return std::string(value);
}

std::string txUtility::getDateTimeString()
{
	char date[256];
	SYSTEMTIME time;
	GetSystemTime(&time);
	SPRINTF(date, 256, "%d-%d-%d_%d-%d-%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return date;
}

void txUtility::sleep(const unsigned long& timeMS)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	Sleep(timeMS);
#elif RUN_PLATFORM == PLATFORM_LINUX
	usleep(timeMS * 1000);
#endif
}

long txUtility::getTimeMS()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
	struct timeval tv;
	if (gettimeofday(&tv, NULL) != 0)
	{
		LOG_ERROR("time get error : %d", errno);
	}
	return tv.tv_sec * 1000 + (long)(tv.tv_usec / 1000.0f);
#endif
}

const char* txUtility::getTime()
{
	LOCK(mTimeLock);
	static char timeBuffer[128] = { 0 };
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