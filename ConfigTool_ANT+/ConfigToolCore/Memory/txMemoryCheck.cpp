#include "txMemoryCheck.h"

txSet<void*> txMemoryCheck::mUsedPtrs;
txThreadLock txMemoryCheck::mLock;

void txMemoryCheck::usePtr(void* ptr)
{
	LOCK(mLock);
	if (!mUsedPtrs.insert(ptr).second)
	{
		//ENGINE_ERROR("error : ptr is in use!");
	}
	UNLOCK(mLock);
}

void txMemoryCheck::unusePtr(void* ptr)
{
	LOCK(mLock);
	auto iter = mUsedPtrs.find(ptr);
	if (iter != mUsedPtrs.end())
	{
		mUsedPtrs.erase(iter);
	}
	else
	{
		//ENGINE_ERROR("error : not find ptr! can not unuse it!");
	}
	UNLOCK(mLock);
}

bool txMemoryCheck::canAccess(void* ptr)
{
	LOCK(mLock);
	bool ret = mUsedPtrs.find(ptr) != mUsedPtrs.end();
	UNLOCK(mLock);
	return ret;
}