#include "txMemoryCheck.h"
#include "Utility.h"

txSet<void*> txMemoryCheck::mUsedPtrs;
ThreadLock txMemoryCheck::mLock;

void txMemoryCheck::usePtr(void* ptr)
{
	LOCK(mLock);
	if (!mUsedPtrs.insert(ptr).second)
	{
		//LOG_ERROR("ptr is in use!");
	}
	UNLOCK(mLock);
}

void txMemoryCheck::unusePtr(void* ptr)
{
	LOCK(mLock);
	if (!mUsedPtrs.tryErase(ptr))
	{
		//LOG_ERROR("not find ptr! can not unuse it!");
	}
	UNLOCK(mLock);
}

bool txMemoryCheck::canAccess(void* ptr)
{
	bool ret = false;
	LOCK(mLock);
	ret = mUsedPtrs.contains(ptr);
	UNLOCK(mLock);
	return ret;
}