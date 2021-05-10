#include "ArrayPool.h"

ThreadLock ArrayPool::mLock;
myMap<uint, ThreadArrayMemory*> ArrayPool::mThreadArrayMemoryList;

void ArrayPool::destroy()
{
	FOREACH(iter, mThreadArrayMemoryList)
	{
		DELETE(iter->second);
	}
	END(mThreadArrayMemoryList);
	mThreadArrayMemoryList.clear();
}

void ArrayPool::deleteArray(void* data)
{
	ThreadArrayMemory* threadArrayMemory = NULL;
	uint threadID = SystemUtility::getThreadID();
	LOCK(mLock);
	if (mThreadArrayMemoryList.contains(threadID))
	{
		threadArrayMemory = mThreadArrayMemoryList[threadID];
	}
	UNLOCK(mLock);
	if (threadArrayMemory != NULL)
	{
		threadArrayMemory->release(data);
	}
}