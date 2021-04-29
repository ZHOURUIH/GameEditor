#ifndef _FRAME_MACRO_H_
#define _FRAME_MACRO_H_

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID PLATFORM_LINUX

// 正在运行的平台标识
#ifdef WINDOWS
#define RUN_PLATFORM PLATFORM_WINDOWS
#endif

#ifdef LINUX
#define RUN_PLATFORM PLATFORM_LINUX
#endif

#ifndef RUN_PLATFORM
#define RUN_PLATFORM -1
#error "wrong platform!"
#endif

// 最大并发连接数为64,需要在winsock.h之前进行定义
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 64

// 由于下面定义的部分宏容易在系统头文件中被定义,从而造成编译无法通关,所以先包含系统头文件,然后再定义自己的宏
#if RUN_PLATFORM == PLATFORM_WINDOWS
// 链接静态库
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#ifdef _MYSQL
#pragma comment(lib, "libmysql.lib")
#endif
#pragma warning(disable: 4005)
// libevent的头文件只能在windows库文件之前包含,否则会有定义冲突的报错
// 部分平台未安装libevent,所以需要使用宏来判断是否需要编译libevent相关代码
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <winsock.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#endif
#if RUN_PLATFORM == PLATFORM_LINUX
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <locale.h>
#include <execinfo.h>
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <fcntl.h>
#include <atomic>
#include <array>
// 部分平台未安装mysql,所以需要使用宏来判断是否需要编译mysql相关代码
#ifdef _MYSQL
#include <mysql.h>
#endif

#ifdef BUILDING_LIBCURL
#include "curl/curl.h"
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 宏定义
#if RUN_PLATFORM == PLATFORM_WINDOWS
#define MY_THREAD HANDLE
#define MY_SOCKET SOCKET
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
#define CLASS_NAME(T) string(typeid(T).name()).substr(strlen("class "))
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#define MEMCPY(dest, bufferSize, src, count) memcpy_s((void*)(dest), bufferSize, (void*)(src), count)
// 获取不同平台下的字面常量字符串的UTF8编码字符串,只能处理字面常量,也就是在代码中写死的字符串
// windows下需要由GB2312转换为UTF8,而linux则直接就是UTF8的
// 将一个字面常量字符串转换为UTF8后存储为变量
#define UNIFIED_UTF8(var, size, constantString)		\
array<char, size> var{ 0 };							\
StringUtility::ANSIToUTF8(constantString, var.data(), size, false)
#elif RUN_PLATFORM == PLATFORM_LINUX
#define MY_THREAD pthread_t
#define MY_SOCKET unsigned int
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
#define CLASS_NAME(T) removePreNumber(typeid(T).name())
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
// 因为bufferSize在windows下是需要的,linux下不需要,所以为了避免警告,仍然使用此参数参与,但是不产生任何影响
#define MEMCPY(dest, bufferSize, src, count) memcpy((void*)((char*)dest + (bufferSize) - (bufferSize)), (void*)(src), count)
// 将一个字面常量字符串转换为UTF8后存储为变量
#define UNIFIED_UTF8(var, size, constantString)		\
array<char, size> var{ 0 };							\
setString(var, constantString, false)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (unsigned int)~0
#endif

#ifndef NULL
#define NULL 0
#endif

#define NOT_FIND static_cast<size_t>(-1)

#ifdef _DEBUG
#define CAST dynamic_cast
#else
#define CAST static_cast
#endif

#define ZERO_MEMORY(buffer, size) memset(buffer, 0, size)
#define STR(t) #t
#define LINE_STR(v) STR(v)
// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, b, pos) value = (value & ~(0x000000FF << (8 * pos))) | (b << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000FF << (8 * pos))) >> (8 * pos)
#define GET_BIT(value, pos) (((value & (1 << (pos))) >> (pos)) & 1)
#define SET_BIT(value, pos, bit) value = (value & ~(1 << (pos))) | ((bit) << (pos))
#define GET_HIGHEST_BIT(value) GET_BIT(value, sizeof(value) * 8 - 1)
#define SET_HIGHEST_BIT(value, bit) SET_BIT(value, sizeof(value) * 8 - 1, bit)
#define GET_LOWEST_BIT(value) GET_BIT(value, 0)
#define SET_LOWEST_BIT(value, bit) SET_BIT(value, 0, bit)
#define HAS_MASK(value, bitMask) ((value) & (1 << (bitMask))) != 0
#define _FILE_LINE_ "File : " + string(__FILE__) + ", Line : " + LINE_STR(__LINE__)
// 生成静态字符串常量的名字
#define NAME(name) STR_##name

//--------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型转字符串
// 以_STR结尾的是构造一个char[]类型的字符串
#define INT_STR(strBuffer, value) \
char strBuffer[16];\
StringUtility::intToString(strBuffer, 16, value);

#define FLOAT_STR(strBuffer, value) \
char strBuffer[16];\
StringUtility::floatToString(strBuffer, 16, value);

#define ULLONG_STR(strBuffer, value)\
 char strBuffer[32];\
StringUtility::ullongToString(strBuffer, 32, value);

#define INTS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[16 * bufferCount];\
StringUtility::intsToString(strBuffer, 16 * bufferCount, valueArray, count);

#define FLOATS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[16 * bufferCount];\
StringUtility::floatsToString(strBuffer, 16 * bufferCount, valueArray, count);

#define ULLONGS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[20 * bufferCount];\
StringUtility::ullongsToString(strBuffer, 20 * bufferCount, valueArray, count);

// 以_CHARS结尾的构造出array<char, SIZE>类型的字符串
#define INT_CHARS(strBuffer, value) \
array<char, 16> strBuffer{0};\
StringUtility::intToString(strBuffer, value);

#define FLOAT_CHARS(strBuffer, value) \
array<char, 16> strBuffer{0};\
StringUtility::floatToString(strBuffer, value);

#define ULLONG_CHARS(strBuffer, value) \
array<char, 32> strBuffer{0};\
StringUtility::ullongToString(strBuffer, value);

#define INTS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 16 * bufferCount> strBuffer{0};\
StringUtility::intsToString(strBuffer, valueArray, count);

#define FLOATS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 16 * bufferCount> strBuffer{0};\
StringUtility::floatsToString(strBuffer, valueArray, count);

#define ULLONGS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 32 * bufferCount> strBuffer{0};\
StringUtility::ullongsToString(strBuffer, valueArray, count);

// 字符串拼接,将str0,str1等字符串拼接后放入charArray中,会覆盖charArray中的内容
// charArray为array<char, int>类型
#define STRCAT2(charArray, str0, str1)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT3(charArray, str0, str1, str2)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT4(charArray, str0, str1, str2, str3)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT5(charArray, str0, str1, str2, str3, str4)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT6(charArray, str0, str1, str2, str3, str4, str5)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT7(charArray, str0, str1, str2, str3, str4, str5, str6)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT8(charArray, str0, str1, str2, str3, str4, str5, str6, str7)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT9(charArray, str0, str1, str2, str3, str4, str5, str6, str7, str8)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT10(charArray, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8, str9};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

// 字符串拼接,将str0,str1等字符串拼接后放入charArray中,会覆盖charArray中的内容
// charArray为char[]类型,_N表示普通数组
#define STRCAT2_N(charArray, size, str0, str1)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT3_N(charArray, size, str0, str1, str2)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT4_N(charArray, size, str0, str1, str2, str3)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT5_N(charArray, size, str0, str1, str2, str3, str4)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT6_N(charArray, size, str0, str1, str2, str3, str4, str5)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT7_N(charArray, size, str0, str1, str2, str3, str4, str5, str6)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT8_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT9_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7, str8)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STRCAT10_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9)\
charArray[0] = '\0';\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8, str9};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

// 字符串拼接,将str0,str1等字符串拼接在charArray中的字符串后面,不会覆盖charArray的内容
// charArray为array<char, int>类型
#define STR_APPEND1(charArray, str0)\
const char* sourceArray[]{str0};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND2(charArray, str0, str1)\
const char* sourceArray[]{str0, str1};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND3(charArray, str0, str1, str2)\
const char* sourceArray[]{str0, str1, str2};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND4(charArray, str0, str1, str2, str3)\
const char* sourceArray[]{str0, str1, str2, str3};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND5(charArray, str0, str1, str2, str3, str4)\
const char* sourceArray[]{str0, str1, str2, str3, str4};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND6(charArray, str0, str1, str2, str3, str4, str5)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND7(charArray, str0, str1, str2, str3, str4, str5, str6)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND8(charArray, str0, str1, str2, str3, str4, str5, str6, str7)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND9(charArray, str0, str1, str2, str3, str4, str5, str6, str7, str8)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND10(charArray, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8, str9};\
StringUtility::strcat_s(charArray, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

// 字符串拼接,将str0,str1等字符串拼接在charArray中的字符串后面,不会覆盖charArray的内容
// charArray为char[]类型,_N表示普通数组
#define STR_APPEND1_N(charArray, size, str0)\
const char* sourceArray[]{str0};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND2_N(charArray, size, str0, str1)\
const char* sourceArray[]{str0, str1};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND3_N(charArray, size, str0, str1, str2)\
const char* sourceArray[]{str0, str1, str2};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND4_N(charArray, size, str0, str1, str2, str3)\
const char* sourceArray[]{str0, str1, str2, str3};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND5_N(charArray, size, str0, str1, str2, str3, str4)\
const char* sourceArray[]{str0, str1, str2, str3, str4};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND6_N(charArray, size, str0, str1, str2, str3, str4, str5)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND7_N(charArray, size, str0, str1, str2, str3, str4, str5, str6)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND8_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND9_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7, str8)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

#define STR_APPEND10_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9)\
const char* sourceArray[]{str0, str1, str2, str3, str4, str5, str6, str7, str8, str9};\
StringUtility::strcat_s(charArray, size, sourceArray, sizeof(sourceArray) / sizeof(sourceArray[0]));

//--------------------------------------------------------------------------------------------------------------------------------------------
// 线程锁相关宏
#ifdef LOCK
#undef LOCK
#endif
#define LOCK(lock) \
(lock).waitForUnlock(__FILE__, __LINE__);\
try\
{

#ifdef UNLOCK
#undef UNLOCK
#endif
#define UNLOCK(lock) \
}catch(...){}\
(lock).unlock()

//--------------------------------------------------------------------------------------------------------------------------------------------
// 日志打印相关宏
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(info)									cout << info + string(" | ") + _FILE_LINE_ << endl
#define LOG(info)									cout << info << endl
#define LOG_NO_PRINT(info)							cout << info << endl
#define PLAYER_ERROR(info, playerGUID)				cout << info + string(" | ") + _FILE_LINE_ << endl
#define PLAYER_LOG(info, playerGUID)				cout << info << endl
#define PLAYER_LOG_NO_PRINT(info, playerGUID)		cout << info << endl

//---------------------------------------------------------------------------------------------------------------------
//内存相关宏定义
#ifdef CHECK_MEMORY
// 带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)	\
NULL;									\
ptr = new className(__VA_ARGS__);		\
if(ptr != NULL)							\
{										\
	MemoryCheck::usePtr(ptr);			\
}										\
else									\
{										\
	ERROR(string("can not alloc memory! className :") + STR(className));\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)		\
NULL;												\
if(count <= 0)										\
{													\
	ERROR("无法申请大小为0的数组");					\
}													\
ptr = new className[count];							\
if (ptr != NULL)									\
{													\
	MemoryCheck::usePtr(ptr);						\
}													\
else												\
{													\
	ERROR(string("can not alloc memory array! className : ") + STR(className) + ", count : " + StringUtility::intToString(count));\
}

#define NORMAL_DELETE(ptr)			\
if (ptr != NULL)					\
{									\
	MemoryCheck::unusePtr(ptr);	\
	delete ptr;						\
	ptr = NULL;						\
}

#define NORMAL_DELETE_ARRAY(ptr)	\
if (ptr != NULL)					\
{									\
	MemoryCheck::unusePtr(ptr);	\
	delete[] ptr;					\
	ptr = NULL;						\
}
#else
// 不带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)			\
NULL;											\
ptr = new className(__VA_ARGS__);				\
if(ptr == NULL)									\
{												\
	ERROR(string("can not alloc memory! className : ") + STR(className));\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)		\
NULL;												\
if(count <= 0)										\
{													\
	ERROR("无法申请大小为0的数组");					\
}													\
ptr = new className[count];							\
if(ptr == NULL)										\
{													\
	ERROR(string("can not alloc memory array! className : ") + STR(className) + ", count : " + StringUtility::intToString(count));\
}

#define NORMAL_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

#define NORMAL_DELETE_ARRAY(ptr)\
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}
#endif

#ifdef NEW
#undef NEW
#endif

#ifdef DELETE
#undef DELETE
#endif

#ifdef NEW_ARRAY
#undef NEW_ARRAY
#endif

#ifdef DELETE_ARRAY
#undef DELETE_ARRAY
#endif

#ifdef TRACE_MEMORY
// 申请无参或者带参构造类的内存
#define NEW(className, ptr, ...)			\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	MemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// 申请无参构造的类或者基础数据类型数组内存
#define NEW_ARRAY(className, count, ptr)		\
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// 释放TRACE_NEW申请的内存
#define DELETE(ptr)					\
MemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// 释放TRACE_NEW_ARRAY申请的内存
#define DELETE_ARRAY(ptr)			\
MemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE_ARRAY(ptr)
#else
#define NEW(className, ptr, ...)			NORMAL_NEW(className, ptr, __VA_ARGS__)
#define NEW_ARRAY(className, count, ptr)	NORMAL_NEW_ARRAY(className, count, ptr)
#define DELETE(ptr)							NORMAL_DELETE(ptr)
#define DELETE_ARRAY(ptr)					NORMAL_DELETE_ARRAY(ptr)
#endif

// 创建一个命令对象
// debug模式下获取行号和文件
#ifdef _DEBUG
#define CMD(classType, cmd)										\
classType* cmd = mCommandSystem->newCmd(cmd, NAME(classType));	\
cmd->setLine(__LINE__);											\
cmd->setFile(__FILE__);											\
cmd->setShowDebugInfo(false);									\
cmd->setDelayCommand(false);

#define CMD_INFO(classType, cmd)								\
classType* cmd = mCommandSystem->newCmd(cmd, NAME(classType));	\
cmd->setLine(__LINE__);											\
cmd->setFile(__FILE__);											\
cmd->setShowDebugInfo(true);									\
cmd->setDelayCommand(false);

#define CMD_SHOW(classType, cmd, showInfo)						\
classType* cmd = mCommandSystem->newCmd(cmd, NAME(classType));	\
cmd->setLine(__LINE__);											\
cmd->setFile(__FILE__);											\
cmd->setShowDebugInfo(showInfo);								\
cmd->setDelayCommand(false);

#define CMD_DELAY(classType, cmd)								\
classType* cmd = mCommandSystem->newCmd(cmd, NAME(classType));	\
cmd->setLine(__LINE__);											\
cmd->setFile(__FILE__);											\
cmd->setShowDebugInfo(false);									\
cmd->setDelayCommand(true);

#define CMD_DELAY_INFO(classType, cmd)							\
classType* cmd = mCommandSystem->newCmd(cmd, NAME(classType));	\
cmd->setLine(__LINE__);											\
cmd->setFile(__FILE__);											\
cmd->setShowDebugInfo(true);									\
cmd->setDelayCommand(true);

#define PUSH_COMMAND(cmdClass, cmdReceiver)			\
{													\
	CMD(cmdClass, cmd);								\
	mCommandSystem->pushCommand(cmd, cmdReceiver);	\
}

#define PUSH_COMMAND_INFO(cmdClass, cmdReceiver)	\
{													\
	CMD_INFO(cmdClass, cmd);						\
	mCommandSystem->pushCommand(cmd, cmdReceiver);	\
}

#else
// release模式下不获取行号和文件提高效率
#define CMD(classType, cmd)										\
auto cmd = mCommandSystem->newCmd<classType>(NAME(classType));	\
cmd->setShowDebugInfo(false);									\
cmd->setDelayCommand(false);

#define CMD_INFO(classType, cmd)								\
auto cmd = mCommandSystem->newCmd<classType>(NAME(classType));	\
cmd->setShowDebugInfo(true);									\
cmd->setDelayCommand(false);

#define CMD_SHOW(classType, cmd, showInfo)						\
auto cmd = mCommandSystem->newCmd<classType>(NAME(classType));	\
cmd->setShowDebugInfo(showInfo);								\
cmd->setDelayCommand(false);

#define CMD_DELAY(classType, cmd)								\
auto cmd = mCommandSystem->newCmd<classType>(NAME(classType));	\
cmd->setShowDebugInfo(false);									\
cmd->setDelayCommand(true);

#define CMD_DELAY_INFO(classType, cmd)							\
auto cmd = mCommandSystem->newCmd<classType>(NAME(classType));	\
cmd->setShowDebugInfo(true);									\
cmd->setDelayCommand(true);

#define PUSH_COMMAND(cmdClass, cmdReceiver)				\
{														\
	CMD(cmdClass, cmd);									\
	mCommandSystem->pushCommand(cmd, cmdReceiver);		\
}

#define PUSH_COMMAND_DELAY(cmdClass, cmdReceiver)		\
{														\
	CMD_DELAY(cmdClass, cmd);							\
	mCommandSystem->pushDelayCommand(cmd, cmdReceiver);	\
}

#define PUSH_COMMAND_INFO(cmdClass, cmdReceiver)		\
{														\
	CMD_INFO(cmdClass, cmd);							\
	mCommandSystem->pushCommand(cmd, cmdReceiver);		\
}
#endif

// 命令调试信息宏
#define COMMAND_DEBUG(info)							\
if ((info)[0] != '\0')								\
{													\
	STRCAT3_N(buffer, size, mCmdType, " : ", info);	\
}													\
else												\
{													\
	StringUtility::strcpy_s(buffer, size, mCmdType);\
}

#define COMMAND_DEBUG_2(size, str0, str1)\
array<char, size> info{0};\
STRCAT2(info, str0, str1);\
COMMAND_DEBUG(info.data());

#define COMMAND_DEBUG_3(size, str0, str1, str2)\
array<char, size> info{0};\
STRCAT3(info, str0, str1, str2);\
COMMAND_DEBUG(info.data());

#define COMMAND_DEBUG_4(size, str0, str1, str2, str3)\
array<char, size> info{0};\
STRCAT4(info, str0, str1, str2, str3);\
COMMAND_DEBUG(info.data());

#define COMMAND_DEBUG_5(size, str0, str1, str2, str3, str4)\
array<char, size> info{0};\
STRCAT5(info, str0, str1, str2, str3, str4);\
COMMAND_DEBUG(info.data());

#define COMMAND_DEBUG_6(size, str0, str1, str2, str3, str4, str5)\
array<char, size> info{0};\
STRCAT6(info, str0, str1, str2, str3, str4, str5);\
COMMAND_DEBUG(info.data());

// 用于显示消息包的调试信息
#define PACKET_DEBUG(info)										\
if ((info)[0] != '\0')											\
{																\
	STRCAT3_N(buffer, size, mPacketName, " : ", info);			\
}																\
else															\
{																\
	StringUtility::strcpy_s(buffer, size, mPacketName);			\
}

#define PACKET_DEBUG_2(size, str0, str1)\
array<char, size> info{0};\
STRCAT2(info, str0, str1);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_3(size, str0, str1, str2)\
array<char, size> info{0};\
STRCAT3(info, str0, str1, str2);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_4(size, str0, str1, str2, str3)\
array<char, size> info{0};\
STRCAT4(info, str0, str1, str2, str3);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_5(size, str0, str1, str2, str3, str4)\
array<char, size> info{0};\
STRCAT5(info, str0, str1, str2, str3, str4);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_6(size, str0, str1, str2, str3, str4, str5)\
array<char, size> info{0};\
STRCAT6(info, str0, str1, str2, str3, str4, str5);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_7(size, str0, str1, str2, str3, str4, str5, str6)\
array<char, size> info{0};\
STRCAT7(info, str0, str1, str2, str3, str4, str5, str6);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_8(size, str0, str1, str2, str3, str4, str5, str6, str7)\
array<char, size> info{0};\
STRCAT8(info, str0, str1, str2, str3, str4, str5, str6, str7);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_9(size, str0, str1, str2, str3, str4, str5, str6, str7, str8)\
array<char, size> info{0};\
STRCAT9(info, str0, str1, str2, str3, str4, str5, str6, str7, str8);\
PACKET_DEBUG(info.data());

#define PACKET_DEBUG_10(size, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9)\
array<char, size> info{0};\
STRCAT10(info, str0, str1, str2, str3, str4, str5, str6, str7, str8, str9);\
PACKET_DEBUG(info.data());

// 为了简化代码书写而添加的宏
//--------------------------------------------------------------------------------------------------------------------------------------------
// 再次封装后的容器的遍历宏
#ifdef _DEBUG
// 需要在循环结束后添加END
#define FOR(stl, expression) stl.lock(STL_LOCK::WRITE, __FILE__, __LINE__);for (expression)
#define END(stl) stl.unlock(STL_LOCK::WRITE);
#else
#define FOR(stl, expression)for (expression)
#define END(stl)
#endif

// 使用迭代器遍历列表,要在循环结束后添加END
#define FOREACH(iter, stl)\
auto iter = stl.begin();\
auto iter##End = stl.end();\
FOR(stl, ; iter != iter##End; ++iter)

// 使用迭代器遍历列表,不需要在循环结束后添加END
#define FOREACH_CONST(iter, stl)\
auto iter = stl.cbegin();\
auto iter##End = stl.cend();\
for(; iter != iter##End; ++iter)

// 使用下标遍历列表,需要在循环结束后添加END
#define FOR_VECTOR(stl) uint stl##Count = stl.size(); FOR(stl, uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_J(stl) uint stl##Count = stl.size(); FOR(stl, uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_K(stl) uint stl##Count = stl.size(); FOR(stl, uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_INVERSE(stl) uint stl##Count = stl.size(); FOR(stl, int i = stl##Count - 1; i >= 0; --i)
// 使用下标遍历常量列表,不需要在循环结束后添加END
#define FOR_VECTOR_CONST(stl) uint stl##Count = stl.size(); for(uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_CONST_J(stl) uint stl##Count = stl.size(); for(uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_CONST_K(stl) uint stl##Count = stl.size(); for(uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_CONST_INVERSE(stl) uint stl##Count = stl.size(); for(stl, int i = stl##Count - 1; i >= 0; --i)
// 简单的for循环
#define FOR_I(count) for (uint i = 0; i < count; ++i)
#define FOR_J(count) for (uint j = 0; j < count; ++j)
#define FOR_K(count) for (uint k = 0; k < count; ++k)
#define FOR_INVERSE_I(count) for (int i = count - 1; i >= 0; --i)
#define FOR_INVERSE_J(count) for (int j = count - 1; j >= 0; --j)
#define FOR_INVERSE_K(count) for (int k = count - 1; k >= 0; --k)
#define FOR_ONCE for(byte tempI = 0; tempI < 1; ++tempI)

// 创建一个消息对象
#define PACKET(classType, packet) auto packet = mNetServer->createPacket<classType>(NAME(classType))
// 创建一个事件参数对象
#define EVENT(classType, eventParam) auto eventParam = mEventSystem->createEvent<classType>(NAME(classType))
// 注册一个系统组件
#define REGISTE_SYSTEM(type)							\
{														\
	type* component = NEW(type, component);				\
	component->setName(STR(type));						\
	mFrameComponentVector.push_back(component);			\
	mFrameComponentMap.insert(STR(type), component);	\
}
// 将类的基类重命名为base,方便使用
#define BASE_CLASS(baseClass) typedef baseClass base
// 从对象池中创建一个MySQLData对象
#define MYSQL_DATA(className, var) auto var = mMySQLDataBase->createData<className>(NAME(className))
#define MYSQL_DATA_1(className, var) var = mMySQLDataBase->createData<className>(NAME(className))
#define VECTOR(T, var)	myVector<T> var

// 当调用UN_VECTOR后,var就不应该再被访问,否则可能会引起不可预知的错误
#define UN_VECTOR(T, var)

#define VECTOR_THREAD(T, var)							\
myVector<T>* temp##var = NULL;							\
{														\
	auto pool = VectorPoolManager::getPoolThread<T>();	\
	if (pool == NULL)									\
	{													\
		ERROR("找不到对应的列表对象池");				\
	}													\
	temp##var = pool->newVector();						\
}														\
myVector<T>& var = *temp##var;

// 当调用UN_VECTOR后,var就不应该再被访问,否则可能会引起不可预知的错误
#define UN_VECTOR_THREAD(T, var)						\
{														\
	auto pool = VectorPoolManager::getPoolThread<T>();	\
	if (pool == NULL)									\
	{													\
		ERROR("找不到对应的列表对象池");				\
	}													\
	auto ptr = &var;									\
	pool->destroyVector(&ptr);							\
}

#ifndef ADD_COMPONENT
#define ADD_COMPONENT(T) CAST<T*>(addComponent(NAME(T)));
#endif

#ifndef ADD_COMPONENT_ACTIVE
#define ADD_COMPONENT_ACTIVE(T) CAST<T*>(addComponent(NAME(T), true));
#endif

#ifndef GET_COMPONENT
#define GET_COMPONENT(var, T) CAST<T*>((var)->getComponent(NAME(T)));
#endif

#define GET_SYSTEM(type) mServerFramework->getSystem<type>(STR(type), m##type)
#define DECALRE_SYSTEM(name) static name* m##name
#define DEFINE_SYSTEM_FRAME(name) name* FrameBase::m##name

#endif