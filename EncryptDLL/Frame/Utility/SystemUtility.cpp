#include "SystemUtility.h"

ullong SystemUtility::mIDSeed = 0;
uint SystemUtility::mMainThread = 0;
ulong SystemUtility::mTimeMS = 0;

void SystemUtility::sleep(ulong timeMS)
{
#ifdef _WINDOWS
	Sleep(timeMS);
#else
	usleep(timeMS * 1000);
#endif
}

ulong SystemUtility::getRealTimeMS()
{
#ifdef _WINDOWS
	return timeGetTime();
#else
	struct timeval tv;
	if (gettimeofday(&tv, NULL) != 0)
	{
		ERROR("time get error : " + intToString(errno));
	}
	return tv.tv_sec * 1000 + (ulong)(tv.tv_usec * 0.001f);
#endif
}

void SystemUtility::getTime(int& weekDay, int& year, int& month, int& day, int& hour, int& minute, int& second)
{
#ifdef _WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	weekDay = sys.wDayOfWeek;
	year = sys.wYear;
	month = sys.wMonth;
	day = sys.wDay;
	hour = sys.wHour;
	minute = sys.wMinute;
	second = sys.wSecond;
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	weekDay = date.tm_wday;
	year = date.tm_year + 1900;
	month = date.tm_mon + 1;
	day = date.tm_mday;
	hour = date.tm_hour;
	minute = date.tm_min;
	second = date.tm_sec;
#endif
}

void SystemUtility::getTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
#ifdef _WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	year = sys.wYear;
	month = sys.wMonth;
	day = sys.wDay;
	hour = sys.wHour;
	minute = sys.wMinute;
	second = sys.wSecond;
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	year = date.tm_year + 1900;
	month = date.tm_mon + 1;
	day = date.tm_mday;
	hour = date.tm_hour;
	minute = date.tm_min;
	second = date.tm_sec;
#endif
}

void SystemUtility::getTime(int& hour, int& minute, int& second)
{
#ifdef _WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	hour = sys.wHour;
	minute = sys.wMinute;
	second = sys.wSecond;
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	hour = date.tm_hour;
	minute = date.tm_min;
	second = date.tm_sec;
#endif
}

string SystemUtility::getTime(bool timeStamp)
{
	array<char, 128> timeBuffer{ 0 };
#ifdef _WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (timeStamp)
	{
		SPRINTF(timeBuffer.data(), timeBuffer.size(), "%d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	}
	else
	{
		SPRINTF(timeBuffer.data(), timeBuffer.size(), "%04d年%02d月%02d日%02d时%02d分%02d秒%03d毫秒", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	}
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	if (timeStamp)
	{
		SPRINTF(timeBuffer.data(), timeBuffer.size(), "%d-%02d-%02d %02d:%02d:%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	}
	else
	{
		SPRINTF(timeBuffer.data(), timeBuffer.size(), "%04d年%02d月%02d日%02d时%02d分%02d秒", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	}
#endif
	return timeBuffer.data();
}

int SystemUtility::getDayOfWeek()
{
#ifdef _WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	return sys.wDayOfWeek;
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	return date.tm_wday;
#endif
	
}

bool SystemUtility::isSameDay(const time_t& timeStamp0, const time_t& timeStamp1)
{
	struct tm dateTime0;
	struct tm dateTime1;
#ifdef _WINDOWS
	localtime_s(&dateTime0, &timeStamp0);
	localtime_s(&dateTime1, &timeStamp1);
#else
	localtime_r(&timeStamp0, &dateTime0);
	localtime_r(&timeStamp1, &dateTime1);
#endif
	return dateTime0.tm_year == dateTime1.tm_year &&
		   dateTime0.tm_mon == dateTime1.tm_mon &&
		   dateTime0.tm_mday == dateTime1.tm_mday;
}

// 获得cpu核心数
int SystemUtility::getCPUCoreCount()
{
#ifdef _WINDOWS
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#else
	return get_nprocs();
#endif
}

void SystemUtility::print(const char* str, bool nextLine)
{
#ifdef _WINDOWS
	cout << str;
	if (nextLine)
	{
		cout << "\n";
	}
#else
	printf("%s", str);
	if (nextLine)
	{
		printf("%s", "\n");
	}
#endif
}

void SystemUtility::input(string& str)
{
#ifdef _WINDOWS
	cin >> str;
#else
	array<char, 256> i{ 0 };
	scanf("%s", i.data());
	str = i.data();
#endif
}

bool SystemUtility::launchExe(const string& path, const string& fullName)
{
#ifdef _WINDOWS
	// 启动游戏程序
	SHELLEXECUTEINFO sei;
	ZERO_MEMORY(&sei, sizeof(sei));
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
#else
	if (vfork() == 0)
	{
		char* argv[] = { (char*)"", (char*)NULL};
		execv(fullName.c_str(), argv);
	}
	return true;
#endif
}

#ifdef _WINDOWS
bool SystemUtility::isExeRunning(const string& fileName)
{
	// 判断当前是否已经运行了该游戏,如果已经运行则直接退出
	mySet<string> processList;
	getAllProcess(processList);
	return processList.contains(fileName);
}

void SystemUtility::getAllProcess(mySet<string>& processList)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄 
	PROCESSENTRY32 info;
	info.dwSize = sizeof(PROCESSENTRY32);
	//调用一次 Process32First 函数，从快照中获取进程列表 
	Process32First(handle, &info);
	//重复调用 Process32Next，直到函数返回 FALSE 为止 
	while (Process32Next(handle, &info))
	{
		processList.insert(info.szExeFile);
	}
}
#else
void SystemUtility::getPidFromStr(const char* str, char* pid, uint size)
{
	uint pos1 = 0;
	uint pos2 = 0;
	uint length = strlen(str);
	for (uint i = 0; i < length; i++)
	{
		uint tmp = 0;
		if (tmp == 0 && str[i] >= '0' && str[i] <= '9')
		{
			tmp = 1;
			pos1 = i;
		}
		if (tmp == 1 && (str[i] < '0' || str[i] > '9'))
		{
			pos2 = i;
			break;
		}
	}
	memset(pid, 0, size);
	uint j = 0;
	for (uint i = pos1; i < pos2; ++i, ++j) 
	{
		pid[j] = str[i];
	}
}

bool SystemUtility::isExeRunning(const string& fileName)
{
	array<char, 16> sCurrPid{ 0 };
	sprintf(sCurrPid.data(), "%d", getpid());

	FILE* fstream = NULL;
	array<char, 1024> buff{ 0 };
	if (NULL == (fstream = popen(("ps -e -o pid,comm | grep " + fileName + " | grep -v PID | grep -v grep").c_str(), "r")))
	{
		return false;
	}
	bool isRunning = false;
	while (NULL != fgets(buff.data(), buff.size(), fstream))
	{
		array<char, 8> oldPID{0};
		getPidFromStr(buff.data(), (char*)oldPID.data(), (uint)oldPID.size());
		if (strcmp(sCurrPid.data(), oldPID.data()) != 0)
		{
			isRunning = true;
			break;
		}
	}
	pclose(fstream);
	return isRunning;
}
#endif

#ifdef _WINDOWS
string SystemUtility::getStackTrace(int depth)
{
	const int MAX_DEPTH = 20;
	clampMax(depth, MAX_DEPTH);
	array<void*, MAX_DEPTH> pStack{ NULL };
	HANDLE process = GetCurrentProcess();
	WORD frames = CaptureStackBackTrace(0, depth, pStack.data(), NULL);
	string oss = "stack traceback: \n";
	return oss;
}
#else
string SystemUtility::getStackTrace(int depth)
{
	string stack;
	array<void*, 512> buffer{ NULL };
	int nptrs = ::backtrace(buffer.data(), buffer.size());
	char** strings = ::backtrace_symbols(buffer.data(), nptrs);
	if (strings != NULL)
	{
		for (int i = 0; i < nptrs; ++i)
		{
			stack.append(strings[i]);
			stack.push_back('\n');
			if (i >= depth + 1)
			{
				break;
			}
		}
		free(strings);
	}
	return stack;
}
#endif