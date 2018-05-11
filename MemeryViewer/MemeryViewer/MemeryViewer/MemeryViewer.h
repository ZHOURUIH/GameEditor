#ifndef _MEMERY_VIEWER_H_
#define _MEMERY_VIEWER_H_

#include <map>
#include <set>
#include <string>

#include "ShareMemeryClient.h"

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

class MemeryViewer
{
public:
	MemeryViewer()
	{
		mClient = new CFFMClient();
		mClient->Open();

		mThread = CreateThread(NULL, 0, debugMemeryTrace, NULL, 0, NULL);
	}
	~MemeryViewer()
	{
		if (mThread != NULL)
		{
			TerminateThread(mThread, 0);
			mThread = NULL;
		}
	}
	static DWORD WINAPI debugMemeryTrace(LPVOID lpParameter);
	static bool isLockRead() { return mLockRead; }
	static bool isLockWrite() { return mLockWrite; }
	static void lockWrite() { mLockWrite = true; }
	static void unlockWrite() { mLockWrite = false; }
	static void lockRead() { mLockRead = true; }
	static void unlockRead() { mLockRead = false; }
	static void waitUnlockRead();
	static void waitUnlockWrite();
	static int getMaxIndex() { return mMaxIndex; }
	static std::map<void*, MemeryInfo>& getMemeryInfoList() { return mMemeryInfoCopy; }
	static MemeryType* getMemeryList() { return mMemeryListCopy; }
protected:
	static bool mTracing;
	static std::map<void*, MemeryInfo> mMemeryInfo;	// 外部不可访问
	static std::map<void*, MemeryInfo> mMemeryInfoCopy; // 用于外部获取的列表
	static std::map<std::string, int> mMemeryTypeIndex;
	static MemeryType mMemeryList[MAX_COUNT];	// 外部不可访问
	static MemeryType mMemeryListCopy[MAX_COUNT];	// 用于外部获取
	static int mMaxIndex;
	static bool mLockRead;
	static bool mLockWrite;
	static CFFMClient* mClient;
	HANDLE mThread;
};

#endif
