#ifndef _ENGINE_DEFINE_H_
#define _ENGINE_DEFINE_H_

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID PLATFORM_LINUX

// 正在运行的平台标识
#if defined(WINDOWS) || defined(WIN32) || defined(_WINDOWS)
#define RUN_PLATFORM PLATFORM_WINDOWS
#endif

#if defined(LINUX)
#define RUN_PLATFORM PLATFORM_LINUX
#endif

#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <mmsystem.h>
#include <iphlpapi.h>
#include <comutil.h>  
#include <Wbemidl.h>  
#include <tchar.h>  
#include <strsafe.h>  
#include <algorithm>  
#include <atlconv.h>  
#include <ntddndis.h>
#include <winioctl.h>
#include <io.h>
#include <direct.h>
#include <cctype>
#include <shellapi.h>
#include <tlhelp32.h>
#include <DbgHelp.h>
#include <atomic>
#include <Dbt.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 宏定义
#if RUN_PLATFORM == PLATFORM_WINDOWS
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#define TX_THREAD HANDLE
#define TX_SOCKET SOCKET
#define NULL_THREAD NULL
#define THREAD_CALLBACK_DECLEAR(func) static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func) DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args) thread = CreateThread(NULL, 0, func, args, 0, NULL)
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	TerminateThread(thread, 0);	\
	CloseHandle(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) closesocket(socket);
#elif RUN_PLATFORM == PLATFORM_LINUX
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
#define TX_THREAD pthread_t
#define TX_SOCKET unsigned int
#define NULL_THREAD 0
#define SOCKADDR_IN sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func) static void* func(void* args)
#define THREAD_CALLBACK(class, func) void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args) pthread_create(&thread, NULL, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	pthread_cancel(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) close(socket);
#ifdef __GNUC__
#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"
#define LC_NAME_zh_CN   "zh_CN"
#endif
#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (unsigned int)~0
#endif

#define INVALID_ID (unsigned long)~0
#define INVALID_INT_ID -1

#ifndef NULL
#define NULL 0
#endif

#define TOSTRING(t) #t
#define LINE_STR(v) TOSTRING(v)
// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, byte, pos) value = (value & ~(0x000000ff << (8 * pos))) | (byte << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000ff << (8 * pos))) >> (8 * pos)
#define _FILE_LINE_ "File : " + std::string(__FILE__) + ", Line : " + LINE_STR(__LINE__)

#define DEBUG_EMPTY "%s", ""
#define COMMAND_PARAM __FILE__, __LINE__
#define COMMAND_CONSTRUCT(className, baseCommand)					\
	className(const char* file, int line, bool showInfo = true)		\
	:																\
	baseCommand(file, line, showInfo)

#define COMMAND_DECLARE_FUNCTION					\
	virtual void execute();							\
	virtual std::string showDebugInfo();

#define COMMAND_DEBUG(...)																\
{																						\
	static char strBuf[256];															\
	sprintf_s(strBuf, 256, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();											\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));	\
	std::string str = cmdName;															\
	if (strlen(strBuf) > 0)																\
	{																					\
		str += std::string(" : ") + strBuf;												\
	}																					\
	return str;																			\
}

typedef bool(*CustomThreadCallback)(void* args);

#endif