#include "MemeryViewer.h"
#include "txSerializer.h"

#pragma comment(lib, "winmm.lib")

bool MemeryViewer::mTracing = true;
std::map<void*, MemeryInfo> MemeryViewer::mMemeryInfo;
std::map<void*, MemeryInfo> MemeryViewer::mMemeryInfoCopy;
std::map<std::string, int> MemeryViewer::mMemeryTypeIndex;
MemeryType MemeryViewer::mMemeryList[MAX_COUNT];
MemeryType MemeryViewer::mMemeryListCopy[MAX_COUNT];
int MemeryViewer::mMaxIndex = 0;
bool MemeryViewer::mLockRead = false;
bool MemeryViewer::mLockWrite = false;
CFFMClient* MemeryViewer::mClient = NULL;

DWORD WINAPI MemeryViewer::debugMemeryTrace(LPVOID lpParameter)
{
	while (mTracing)
	{
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		if (curTime - beginTime < 500)
		{
			Sleep(500 - (curTime - beginTime));
		}
		curTime = timeGetTime();

		DWORD filesize = 0;
		mClient->GetCmdDataSize(&filesize);
		char* buffer = new char[filesize];
		mClient->ReadCmdData(1, filesize, buffer);

		txSerializer serializer(buffer, filesize);

		std::map<void*, MemeryInfo> tempInfoList = mMemeryInfo;
		// 读取详细信息数量
		int infoCount = 0;
		serializer.read(infoCount);
		for (int i = 0; i < infoCount; ++i)
		{
			if (!mTracing)
			{
				return 0;
			}
			void* ptr = NULL;
			serializer.read(ptr);
			int s;
			serializer.read(s);
			char f[128];
			serializer.readString(f, 128);
			int l;
			serializer.read(l);
			char t[128];
			serializer.readString(t, 128);
			// 如果之前的列表中没有,则插入
			std::map<void*, MemeryInfo>::iterator iterInfo = mMemeryInfo.find(ptr);
			if (iterInfo == mMemeryInfo.end())
			{
				mMemeryInfo.insert(std::make_pair(ptr, MemeryInfo(s, f, l, t)));
			}
			// 如果已经有了,则修改,并且在临时列表中将该地址删除
			else
			{
				iterInfo->second = MemeryInfo(s, f, l, t);
				std::map<void*, MemeryInfo>::iterator iterTemp = tempInfoList.find(ptr);
				iterTemp->second.size = -1;
			}
		}

		// 临时列表中剩余的就是需要删除的
		std::map<void*, MemeryInfo>::iterator iterTemp = tempInfoList.begin();
		std::map<void*, MemeryInfo>::iterator iterTempEnd = tempInfoList.end();
		for (; iterTemp != iterTempEnd; ++iterTemp)
		{
			if (iterTemp->second.size > 0)
			{
				std::map<void*, MemeryInfo>::iterator iterInfo = mMemeryInfo.find(iterTemp->first);
				if (iterInfo != mMemeryInfo.end())
				{
					mMemeryInfo.erase(iterInfo);
				}
			}
		}

		// 首先将所有的类型信息清空,只保留类型名
		for (int i = 0; i < mMaxIndex; ++i)
		{
			mMemeryList[i] = MemeryType(mMemeryList[i].type);
		}
		// 读取统计信息数量
		int typeCount = 0;
		serializer.read(typeCount);
		for (int i = 0; i < typeCount; ++i)
		{
			if (!mTracing)
			{
				return 0;
			}
			char typeName[128];
			serializer.readString(typeName, 128);
			int count;
			serializer.read(count);
			int s;
			serializer.read(s);
			std::map<std::string, int>::iterator iterIndex = mMemeryTypeIndex.find(typeName);
			if (iterIndex == mMemeryTypeIndex.end())
			{
				if (mMaxIndex < MAX_COUNT)
				{
					mMemeryList[mMaxIndex] = MemeryType(typeName, count, s);
					mMemeryTypeIndex.insert(std::make_pair(typeName, mMaxIndex));
					++mMaxIndex;
				}
			}
			else
			{
				mMemeryList[iterIndex->second] = MemeryType(typeName, count, s);
			}
		}
		delete[] buffer;
		
		// 等待解锁写入,锁定读取操作
		waitUnlockWrite();
		lockRead();

		mMemeryInfoCopy = mMemeryInfo;
		for (int i = 0; i < MAX_COUNT; ++i)
		{
			if (!mTracing)
			{
				unlockRead();
				return 0;
			}
			mMemeryListCopy[i] = mMemeryList[i];
		}
		// 解读取锁操作
		unlockRead();
		
		beginTime = curTime;
	}
	return 0;
}

void MemeryViewer::waitUnlockRead()
{
	while (mLockRead)
	{}
}

void MemeryViewer::waitUnlockWrite()
{
	while (mLockWrite)
	{}
}