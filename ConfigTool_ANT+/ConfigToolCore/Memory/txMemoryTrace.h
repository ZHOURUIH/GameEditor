#ifndef _TX_MEMORY_TRACE_H_
#define _TX_MEMORY_TRACE_H_

//#define TRACE_MEMORY
//#define WRITE_FILE
//#define CHECK_MEMORY
#include "EngineDefine.h"
#include "txThreadLock.h"
#include "txMemoryCheck.h"

// 无论什么平台都需要定义以下正常的内存申请宏
#define NEW_MEMORY(className, ptr, check, ...)		\
NULL;												\
try													\
{													\
	ptr = new className(__VA_ARGS__);				\
	if (check)										\
	{												\
		txMemoryCheck::usePtr(ptr);					\
	}												\
}													\
catch (...)											\
{													\
	ptr = NULL;										\
	TOOL_CORE_ERROR("error : can not alloc memory!");\
}

// 正常的申请数组内存
#define NEW_MEMORY_ARRAY(className, count, ptr, check)		\
NULL;														\
if (count <= 0)												\
{															\
	ptr = NULL;												\
}															\
else														\
{															\
	try														\
	{														\
		ptr = new className[count];							\
		memset(ptr, 0, sizeof(className)* count);			\
		if (check)											\
		{													\
			txMemoryCheck::usePtr(ptr);						\
		}													\
	}														\
	catch (...)												\
	{														\
		ptr = NULL;											\
		TOOL_CORE_ERROR("error : can not alloc memory array!");\
	}														\
}															\

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
#include "txShareMemoryServer.h"
// 注意事项!!!
// 需要确保MemeryTrace对象只有一个
// 所有的堆内存申请和释放都必须使用TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW申请的内存只能用TRACE_DELETE释放,TRACE_NEW_ARRAY申请的内存只能用TRACE_DELETE_ARRAY释放
// 虽然可以像new和delete一样的语法使用TRACE相关宏,但是不能直接在return后面加TRACE相关宏
// TRACE相关宏也不能在构造函数初始化列表中使用

struct MemoryInfo
{
	MemoryInfo(int s, std::string f, int l, std::string t)
		:
		size(s),
		file(f),
		line(l),
		type(t)
	{}
	int size;			// 内存大小
	std::string file;	// 开辟内存的文件
	int line;			// 开辟内存的代码行号
	std::string type;	// 内存的对象类型
};

struct MemoryType
{
	MemoryType(const std::string& t = "")
	:
	type(t),
	count(0),
	size(0)
	{}
	MemoryType(const std::string& t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};
const int MAX_COUNT = 1000;
class txMemoryTrace
{
public:
	txMemoryTrace();
	virtual ~txMemoryTrace();
	// writeOrDebug为真则表示将信息写入共享内存,然后使用MemeryViewer查看,为假时表示在控制台显示内存信息
	void init(bool writeOrDebug);
	static DWORD WINAPI debugMemoryTrace(LPVOID lpParameter);
#ifdef WRITE_FILE
	static DWORD WINAPI writeMemoryTrace(LPVOID lpParameter);
#endif
	static void insertPtr(void* ptr, MemoryInfo info);
	static void erasePtr(void* ptr);
	static void setIgnoreClass(txSet<std::string>& classList){mIgnoreClass = classList;}
	static void setIgnoreClassKeyword(txSet<std::string>& classList){mIgnoreClassKeyword = classList;}
	static void setShowOnlyDetailClass(txSet<std::string>& classList){mShowOnlyDetailClass = classList;}
	static void setShowOnlyStatisticsClass(txSet<std::string>& classList){mShowOnlyStatisticsClass = classList;}
	static void setShowDetail(bool show){mShowDetail = show;}
	static void setShowStatistics(bool show){mShowStatistics = show;}
	static void setShowAll(bool show){mShowAll = show;}
protected:
	// 内存申请总信息表
	static txMap<void*, MemoryInfo> mMemoryInfo;
	// 内存统计信息表, first是类型名,second的first是该类型名的内存个数,second是该类型占得总内存大小,单位是字节
	static txMap<std::string, MemoryType> mMemoryType;
	// 不显示该列表中类型的内存详细信息以及统计信息
	static txSet<std::string> mIgnoreClass;
	// 如果详细信息以及统计信息中的类型包含该列表中的关键字,则不显示
	static txSet<std::string> mIgnoreClassKeyword;
	// 只显示该列表中类型的内存详细信息,如果该列表为空,则全部显示
	static txSet<std::string> mShowOnlyDetailClass;
	// 只显示该列表中类型的内存统计信息,如果该列表为空,则全部显示
	static txSet<std::string> mShowOnlyStatisticsClass;
	// 是否显示总信息表的详细内容
	static bool mShowDetail;
	// 是否显示内存统计信息
	static bool mShowStatistics;
	// 是否显示内存总个数
	static bool mShowTotalCount;
	static int mInstanceCount;
	static bool mShowAll;
	static txThreadLock mInfoLock;
	HANDLE mThread;
	static txShareMemoryServer* mShareMemoryServer;
#ifdef WRITE_FILE
	static txMap<std::string, int> mMemoryTypeIndex;
	static MemoryType mMemoryList[MAX_COUNT];
	static int mMemoryCount;
#endif
};
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