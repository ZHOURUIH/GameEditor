#include "txMemoryTrace.h"
#ifdef TRACE_MEMORY
#include "txLog.h"
#ifdef WRITE_FILE
#include "txSerializer.h"
#endif
#include "TimeLock.h"

txMap<void*, MemoryInfo> txMemoryTrace::mMemoryInfo;
txMap<std::string, MemoryType> txMemoryTrace::mMemoryType;
txSet<std::string> txMemoryTrace::mIgnoreClass;
txSet<std::string> txMemoryTrace::mIgnoreClassKeyword;
txSet<std::string> txMemoryTrace::mShowOnlyDetailClass;
txSet<std::string> txMemoryTrace::mShowOnlyStatisticsClass;
bool txMemoryTrace::mShowDetail = true;
bool txMemoryTrace::mShowStatistics = true;
bool txMemoryTrace::mShowTotalCount = true;
int txMemoryTrace::mInstanceCount = 0;
bool txMemoryTrace::mShowAll = true;
#ifdef WRITE_FILE
txMap<std::string, int> txMemoryTrace::mMemoryTypeIndex;
MemoryType txMemoryTrace::mMemoryList[MAX_COUNT];
int txMemoryTrace::mMemoryCount = 0;
#endif
txShareMemoryServer* txMemoryTrace::mShareMemoryServer = NULL;
ThreadLock txMemoryTrace::mInfoLock;

txMemoryTrace::txMemoryTrace()
{
	++mInstanceCount;
	assert(mInstanceCount <= 1);
	mShowDetail = true;
	mShowStatistics = true;
	mShowTotalCount = true;
	mShowAll = true;
	mShareMemoryServer = TRACE_NEW(txShareMemoryServer, mShareMemoryServer);
	mShareMemoryServer->Create();
	mThread = NULL;
}

txMemoryTrace::~txMemoryTrace()
{
	if (mThread != NULL)
	{
		TerminateThread(mThread, 0);
		mThread = NULL;
	}
	txUtility::sleep(100);
	TRACE_DELETE(mShareMemoryServer);
}

void txMemoryTrace::init(bool writeOrDebug)
{
#ifdef WRITE_FILE
	if (writeOrDebug)
	{
		mThread = CreateThread(NULL, 0, writeMemoryTrace, NULL, 0, NULL);
	}
	else
	{
		mThread = CreateThread(NULL, 0, debugMemoryTrace, NULL, 0, NULL);
	}
#else
	mThread = CreateThread(NULL, 0, debugMemoryTrace, NULL, 0, NULL);
#endif
}

DWORD WINAPI txMemoryTrace::debugMemoryTrace(LPVOID lpParameter)
{
	TimeLock timeLock(1000);
	while (true)
	{
		timeLock.update();
		int memoryCount = mMemoryInfo.size();
		int memorySize = 0;
		if (mShowAll)
		{
			// 首先检测是否可以读取,如果不可以,则等待解锁读取
			LOCK(mInfoLock);
			LOGI("\n\n---------------------------------------------memery info begin-----------------------------------------------------------\n");

			// 内存详细信息
			auto iter = mMemoryInfo.begin();
			auto iterEnd = mMemoryInfo.end();
			FOR_STL(mMemoryInfo, ; iter != iterEnd; ++iter)
			{
				memorySize += iter->second.size;
				if (!mShowDetail)
				{
					continue;
				}
				// 如果该类型已忽略,则不显示
				if (mIgnoreClass.find(iter->second.type) != mIgnoreClass.end())
				{
					continue;
				}

				// 如果仅显示的类型列表不为空,则只显示列表中的类型
				if (mShowOnlyDetailClass.size() > 0 && mShowOnlyDetailClass.find(iter->second.type) == mShowOnlyDetailClass.end())
				{
					continue;
				}

				// 如果类型包含关键字,则不显示
				bool show = true;
				auto iterKeyword = mIgnoreClassKeyword.begin();
				auto iterKeywordEnd = mIgnoreClassKeyword.end();
				FOR_STL (mIgnoreClassKeyword, ; iterKeyword != iterKeywordEnd; ++iterKeyword)
				{
					if (strstr(iter->second.type.c_str(), iterKeyword->c_str()) != NULL)
					{
						show = false;
						break;
					}
				}
				END_FOR_STL(mIgnoreClassKeyword);

				if (show)
				{
					LOGI("size : %d, file : %s, line : %d, class : %s\n", iter->second.size, iter->second.file.c_str(), iter->second.line, iter->second.type.c_str());
				}
			}
			END_FOR_STL(mMemoryInfo);
			UNLOCK(mInfoLock);

			if (mShowTotalCount)
			{
				LOGI("-------------------------------------------------memery count : %d, total size : %.3fKB\n", memoryCount, memorySize / 1000.0f);
			}
			// 显示统计数据
			if (mShowStatistics)
			{
				auto iterType = mMemoryType.begin();
				auto iterTypeEnd = mMemoryType.end();
				FOR_STL (mMemoryType, ; iterType != iterTypeEnd; ++iterType)
				{
					// 如果该类型已忽略,则不显示
					if (mIgnoreClass.find(iterType->first) != mIgnoreClass.end())
					{
						continue;
					}
					// 如果仅显示的类型列表不为空,则只显示列表中的类型
					if (mShowOnlyStatisticsClass.size() > 0 && mShowOnlyStatisticsClass.find(iterType->first) == mShowOnlyStatisticsClass.end())
					{
						continue;
					}
					// 如果类型包含关键字,则不显示
					bool show = true;
					auto iterKeyword = mIgnoreClassKeyword.begin();
					auto iterKeywordEnd = mIgnoreClassKeyword.end();
					FOR_STL (mIgnoreClassKeyword, ; iterKeyword != iterKeywordEnd; ++iterKeyword)
					{
						if (strstr(iterType->first.c_str(), iterKeyword->c_str()) != NULL)
						{
							show = false;
							break;
						}
					}
					END_FOR_STL(mIgnoreClassKeyword);
					if (show)
					{
						LOGI("%s : %d个, %.3fKB\n", iterType->first.c_str(), iterType->second.count, iterType->second.size / 1000.0f);
					}
				}
				END_FOR_STL(mMemoryType);
			}
			LOGI("---------------------------------------------memery info end-----------------------------------------------------------\n");
		}
		beginTime = curTime;
	}
	return 0;
}

#ifdef WRITE_FILE
DWORD WINAPI txMemoryTrace::writeMemoryTrace(LPVOID lpParameter)
{
	TimeLock timeLock(1000);
	while (true)
	{
		timeLock.update();
		// 这里就不能在对序列化的内存进行跟踪,否则会陷入死锁
		txSerializer serializer(false);

		// 锁定列表
		LOCK(mInfoLock);
		// 写入详细信息数量
		int infoCount = mMemoryInfo.size();
		serializer.write(infoCount);
		auto iterInfo = mMemoryInfo.begin();
		auto iterInfoEnd = mMemoryInfo.end();
		FOR_STL(mMemoryInfo, ; iterInfo != iterInfoEnd; ++iterInfo)
		{
			serializer.write(iterInfo->first);						// 地址
			serializer.write(iterInfo->second.size);				// 内存大小
			serializer.writeString(iterInfo->second.file.c_str());	// 文件名
			serializer.write(iterInfo->second.line);				// 行号
			serializer.writeString(iterInfo->second.type.c_str());	// 类型
		}
		END_FOR_STL(mMemoryInfo);

		// 写入类型数量
		int typeCount = mMemoryTypeIndex.size();
		serializer.write(typeCount);
		auto iterIndex = mMemoryTypeIndex.begin();
		auto iterIndexEnd = mMemoryTypeIndex.end();
		FOR_STL (mMemoryTypeIndex, ; iterIndex != iterIndexEnd; ++iterIndex)
		{
			serializer.writeString(mMemoryList[iterIndex->second].type.c_str());	// 类型名
			serializer.write(mMemoryList[iterIndex->second].count);					// 个数
			serializer.write(mMemoryList[iterIndex->second].size);					// 大小
		}
		END_FOR_STL(mMemoryTypeIndex);
		// 解锁列表
		UNLOCK(mInfoLock);
		mShareMemoryServer->WriteCmdData(1, serializer.getDataSize(), (void*)serializer.getBuffer());
		beginTime = curTime;
	}
	return 0;
}
#endif

void txMemoryTrace::insertPtr(void* ptr, MemoryInfo info)
{
	// 锁定列表
	LOCK(mInfoLock);
	int lastPos = info.file.find_last_of('\\');
	if (lastPos != -1)
	{
		info.file = info.file.substr(lastPos + 1, info.file.length() - lastPos - 1);
	}
	mMemoryInfo.insert(ptr, info);

	auto iterType = mMemoryType.find(info.type);
	if (iterType != mMemoryType.end())
	{
		++(iterType->second.count);
		iterType->second.size += info.size;
	}
	else
	{
		mMemoryType.insert(info.type, MemoryType(info.type, 1, info.size));
	}

#ifdef WRITE_FILE
	// 在类型下标列表中查找该类型,如果有,则更新类型信息
	auto iterIndex = mMemoryTypeIndex.find(info.type);
	if (iterIndex != mMemoryTypeIndex.end())
	{
		auto iterType = mMemoryType.find(info.type);
		mMemoryList[iterIndex->second] = iterType->second;
	}
	// 如果没有,则添加类型信息
	else
	{
		if (mMemoryCount < MAX_COUNT)
		{
			auto iterType = mMemoryType.find(info.type);
			mMemoryTypeIndex.insert(info.type, mMemoryCount);
			mMemoryList[mMemoryCount] = iterType->second;
			++mMemoryCount;
		}
		else
		{
			UNLOCK(mInfoLock);
			return;
		}
	}
#endif
	// 解锁列表
	UNLOCK(mInfoLock);
}

void txMemoryTrace::erasePtr(void* ptr)
{
	// 锁定列表
	LOCK(mInfoLock);
	// 加一层循环是为了方便解锁
	for (int i = 0; i < 1; ++i)
	{
		// 从内存信息列表中移除
		auto iterTrace = mMemoryInfo.find(ptr);
		std::string type;
		int size = 0;
		if (iterTrace == mMemoryInfo.end())
		{
			break;
		}
		type = iterTrace->second.type;
		size = iterTrace->second.size;
		mMemoryInfo.erase(iterTrace);
		// 从内存类型列表中移除
		auto iterType = mMemoryType.find(type);
		if (iterType == mMemoryType.end())
		{
			break;
		}
		--(iterType->second.count);
		iterType->second.size -= size;
#ifdef WRITE_FILE
		// 在下标列表中查找该类型的下标,如果有,则将类型信息中的信息清空
		auto iterIndex = mMemoryTypeIndex.find(type);
		if (iterIndex == mMemoryTypeIndex.end())
		{
			break;
		}
		std::string type = mMemoryList[iterIndex->second].type;
		--(mMemoryList[iterIndex->second].count);
		mMemoryList[iterIndex->second].size -= size;
#endif
	}
	// 解锁列表
	UNLOCK(mInfoLock);
}

#endif