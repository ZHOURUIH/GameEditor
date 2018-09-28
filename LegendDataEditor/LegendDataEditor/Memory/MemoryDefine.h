#ifndef _MEMORY_DEFINE_H_
#define _MEMORY_DEFINE_H_

//#define TRACE_MEMORY
//#define CHECK_MEMORY

#include "ServerDefine.h"
#include "txMemoryCheck.h"

// 无论什么平台都需要定义以下正常的内存申请宏
#define NEW_MEMORY(className, ptr, check, ...)	\
NULL;											\
try												\
{												\
	ptr = new className(__VA_ARGS__);			\
	if (check)									\
	{											\
		txMemoryCheck::usePtr(ptr);				\
	}											\
}												\
catch (...)										\
{												\
	ptr = NULL;									\
}

// 正常的申请数组内存
#define NEW_MEMORY_ARRAY(className, count, ptr, check)	\
NULL;													\
if (count <= 0)											\
{														\
	ptr = NULL;											\
}														\
else													\
{														\
	try													\
	{													\
		ptr = new className[count];						\
		memset(ptr, 0, sizeof(className)* count);		\
		if (check)										\
		{												\
			txMemoryCheck::usePtr(ptr);					\
		}												\
	}													\
	catch (...)											\
	{													\
		ptr = NULL;										\
	}													\
}														\

// 正常的释放内存
#define DELETE_MEMORY(ptr, check)		\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete ptr;							\
	ptr = NULL;							\
}

// 正常的释放数组内存
#define DELETE_MEMORY_ARRAY(ptr, check)	\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete[] ptr;						\
	ptr = NULL;							\
}

#ifdef CHECK_MEMORY
// 带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, true, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, true)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, true)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, true)
#else
// 不带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, false, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, false)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, false)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, false)
#endif

#ifdef TRACE_MEMORY
// 申请无参或者带参构造类的内存
#define TRACE_NEW(className, ptr, ...)		\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// 申请无参构造的类或者基础数据类型数组内存
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// 释放TRACE_NEW申请的内存
#define TRACE_DELETE(ptr)			\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// 释放TRACE_NEW_ARRAY申请的内存
#define TRACE_DELETE_ARRAY(ptr)		\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE_ARRAY(ptr)
#else
#define TRACE_NEW(className, ptr, ...)			NORMAL_NEW(className, ptr, __VA_ARGS__)
#define TRACE_NEW_ARRAY(className, count, ptr)  NORMAL_NEW_ARRAY(className, count, ptr)
#define TRACE_DELETE(ptr)						NORMAL_DELETE(ptr)
#define TRACE_DELETE_ARRAY(ptr)					NORMAL_DELETE_ARRAY(ptr)
#endif

#endif