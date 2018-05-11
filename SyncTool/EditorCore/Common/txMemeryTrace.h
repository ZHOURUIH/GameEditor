#ifndef _TX_MEMERY_TRACE_H_
#define _TX_MEMERY_TRACE_H_

#ifdef _TRACE_MEMERY

#include <map>
#include <set>
#include <string>
#include <typeinfo.h>
#include <assert.h>

#define _WRITE_FILE

#include <windows.h>

#include "txShareMemeryServer.h"

// 注意事项!!!
// 需要确保MemeryTrace对象只有一个
// 所有的堆内存申请和释放都必须使用TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW申请的内存只能用TRACE_DELETE释放,TRACE_NEW_ARRAY申请的内存只能用TRACE_DELETE_ARRAY释放
// 虽然可以像new和delete一样的语法使用TRACE相关宏,但是不能直接在return后面加TRACE相关宏
// TRACE相关宏也不能在构造函数初始化列表中使用

const int MAX_COUNT = 1000;

struct MemeryInfo
{
	MemeryInfo(int s, std::string f, int l, std::string t)
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

struct MemeryType
{
	MemeryType(std::string t = "")
	:
	type(t),
	count(0),
	size(0)
	{}
	MemeryType(std::string t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};

class MemeryTrace
{
public:
	MemeryTrace();
	virtual ~MemeryTrace();
	// writeOrDebug为真则表示将信息写入共享内存,然后使用MemeryViewer查看,为假时表示在控制台显示内存信息
	void init(bool writeOrDebug)
	{
#ifdef _WRITE_FILE
		if (writeOrDebug)
		{
			mThread = CreateThread(NULL, 0, MemeryTrace::writeMemeryTrace, NULL, 0, NULL);
		}
		else
		{
			mThread = CreateThread(NULL, 0, MemeryTrace::debugMemeryTrace, NULL, 0, NULL);
		}
#else
		mThread = CreateThread(NULL, 0, MemeryTrace::debugMemeryTrace, NULL, 0, NULL);
#endif
	}
	static DWORD WINAPI debugMemeryTrace(LPVOID lpParameter);
#ifdef _WRITE_FILE
	static DWORD WINAPI writeMemeryTrace(LPVOID lpParameter);
#endif
	static void insertPtr(void* ptr, MemeryInfo info)
	{
		// 首先检测是否可以写入,如果不可以,则等待解锁写入
		waitUnlockWrite();
		// 正在对列表进行写入操作,所以要锁定列表的读取
		lockReadInfo();
		int lastPos = info.file.find_last_of('\\');
		if (lastPos != -1)
		{
			info.file = info.file.substr(lastPos + 1, info.file.length() - lastPos - 1);
		}
		mMemeryInfo.insert(std::make_pair(ptr, info));

		auto iterType = mMemeryType.find(info.type);
		if (iterType != mMemeryType.end())
		{
			++(iterType->second.count);
			iterType->second.size += info.size;
		}
		else
		{
			mMemeryType.insert(std::make_pair(info.type, MemeryType(info.type, 1, info.size)));
		}

#ifdef _WRITE_FILE
		// 在类型下标列表中查找该类型,如果有,则更新类型信息
		auto iterIndex = mMemeryTypeIndex.find(info.type);
		if (iterIndex != mMemeryTypeIndex.end())
		{
			auto iterType = mMemeryType.find(info.type);
			mMemeryList[iterIndex->second] = iterType->second;
		}
		// 如果没有,则添加类型信息
		else
		{
			if (mMemeryCount < MAX_COUNT)
			{
				auto iterType = mMemeryType.find(info.type);
				mMemeryTypeIndex.insert(std::make_pair(info.type, mMemeryCount));
				mMemeryList[mMemeryCount] = iterType->second;
				++mMemeryCount;
			}
			else
			{
				unlockReadInfo();
				return;
			}
		}
#endif

		// 解锁列表的读取
		unlockReadInfo();
	}
	static void erasePtr(void* ptr)
	{
		// 首先检测是否可以写入,如果不可以,则等待解锁写入
		waitUnlockWrite();
		// 正在对列表进行写入操作,所以要锁定列表的读取
		lockReadInfo();
		
		auto iterTrace = mMemeryInfo.find(ptr);
		std::string type;
		int size = 0;
		if (iterTrace != mMemeryInfo.end())
		{
			type = iterTrace->second.type;
			size = iterTrace->second.size;
			mMemeryInfo.erase(iterTrace);
		}
		else
		{
			unlockReadInfo();
			return;
		}

		auto iterType = mMemeryType.find(type);
		if (iterType != mMemeryType.end())
		{
			--(iterType->second.count);
			iterType->second.size -= size;
		}
		else
		{
			unlockReadInfo();
			return;
		}

#ifdef _WRITE_FILE
		// 在下标列表中查找该类型的下标,如果有,则将类型信息中的信息清空
		auto iterIndex = mMemeryTypeIndex.find(type);
		if (iterIndex != mMemeryTypeIndex.end())
		{
			std::string type = mMemeryList[iterIndex->second].type;
			--(mMemeryList[iterIndex->second].count);
			mMemeryList[iterIndex->second].size -= size;
		}
		else
		{
			unlockReadInfo();
			return;
		}
#endif

		// 解锁列表的读取
		unlockReadInfo();
	}
	static void setIgnoreClass(std::set<std::string>& classList){mIgnoreClass = classList;}
	static void setIgnoreClassKeyword(std::set<std::string>& classList){mIgnoreClassKeyword = classList;}
	static void setShowOnlyDetailClass(std::set<std::string>& classList){mShowOnlyDetailClass = classList;}
	static void setShowOnlyStatisticsClass(std::set<std::string>& classList){mShowOnlyStatisticsClass = classList;}
	static void setShowDetail(bool show){mShowDetail = show;}
	static void setShowStatistics(bool show){mShowStatistics = show;}
	static void setShowAll(bool show){mShowAll = show;}
	static void lockWriteInfo() { mLockWriteInfo = true; }
	static void unlockWriteInfo() { mLockWriteInfo = false; }
	static void waitUnlockWrite()
	{
		while (mLockWriteInfo)
		{}
	}
	static void lockReadInfo() { mLockReadInfo = true; }
	static void unlockReadInfo() { mLockReadInfo = false; }
	static void waitUnlockRead()
	{
		while (mLockReadInfo)
		{}
	}
protected:
	// 内存申请总信息表
	static std::map<void*, MemeryInfo> mMemeryInfo;
	// 内存统计信息表, first是类型名,second的first是该类型名的内存个数,second是该类型占得总内存大小,单位是字节
	static std::map<std::string, MemeryType> mMemeryType;
	// 不显示该列表中类型的内存详细信息以及统计信息
	static std::set<std::string> mIgnoreClass;
	// 如果详细信息以及统计信息中的类型包含该列表中的关键字,则不显示
	static std::set<std::string> mIgnoreClassKeyword;
	// 只显示该列表中类型的内存详细信息,如果该列表为空,则全部显示
	static std::set<std::string> mShowOnlyDetailClass;
	// 只显示该列表中类型的内存统计信息,如果该列表为空,则全部显示
	static std::set<std::string> mShowOnlyStatisticsClass;
	// 是否显示总信息表的详细内容
	static bool mShowDetail;
	// 是否显示内存统计信息
	static bool mShowStatistics;
	// 是否显示内存总个数
	static bool mShowTotalCount;
	static int mInstanceCount;
	static bool mShowAll;
	static bool mLockWriteInfo;
	static bool mLockReadInfo;
	HANDLE mThread;

#ifdef _WRITE_FILE
	static std::map<std::string, int> mMemeryTypeIndex;
	static MemeryType mMemeryList[MAX_COUNT];
	static int mMemeryCount;
#endif

	static CFFMServer* mServer;
};

// 申请无参或者带参构造类的内存
#define TRACE_NEW(className, ptr, ...)  NULL;\
										ptr = new className(__VA_ARGS__);	\
										MemeryTrace::insertPtr(ptr, MemeryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name()));

// 申请无参构造的类或者基础数据类型数组内存
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}												\
MemeryTrace::insertPtr(ptr, MemeryInfo(sizeof(className) * count, __FILE__, __LINE__, typeid(className).name()));

// 释放TRACE_NEW申请的内存
#define TRACE_DELETE(ptr)			\
MemeryTrace::erasePtr((void*)ptr);	\
if(ptr != NULL)						\
{									\
	delete ptr;						\
	ptr = NULL;						\
}

// 释放TRACE_NEW_ARRAY申请的内存
#define TRACE_DELETE_ARRAY(ptr)		\
MemeryTrace::erasePtr((void*)ptr);	\
if(ptr != NULL)						\
{									\
	delete[] ptr;					\
	ptr = NULL;						\
}

#else

// 申请无参或者带参构造类的内存
#define TRACE_NEW(className, ptr, ...)  NULL;\
										ptr = new className(__VA_ARGS__);

// 申请无参构造的类或者基础数据类型数组内存
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}

// 释放TRACE_NEW申请的内存
#define TRACE_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

// 释放TRACE_NEW_ARRAY申请的内存
#define TRACE_DELETE_ARRAY(ptr) \
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}
#endif

// 无论什么平台都需要定义以下正常的内存申请宏
#define NORMAL_NEW(className, ptr, ...) NULL;\
	ptr = new className(__VA_ARGS__);

// 正常的申请数组内存
#define NORMAL_NEW_ARRAY(className, count, ptr) \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}												\

// 正常的释放内存
#define NORMAL_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

// 正常的释放数组内存
#define NORMAL_DELETE_ARRAY(ptr)	\
if (ptr != NULL)					\
{									\
	delete[] ptr;					\
	ptr = NULL;						\
}

#endif