#include "TimeLock.h"
#include "Utility.h"

TimeLock::TimeLock(const long& frameTimeMS)
{
	mFrameTimeMS = frameTimeMS;
	mForceSleep = 5;
	mLastTime = txUtility::getTimeMS();
	mCurTime = mLastTime;
}

long TimeLock::update()
{
	long endTime = txUtility::getTimeMS();
	long remainMS = mFrameTimeMS - (endTime - mCurTime);
	if (remainMS > 0)
	{
		txUtility::sleep(remainMS);
	}
	else if (mForceSleep > 0)
	{
		txUtility::sleep(mForceSleep);
	}
	mCurTime = txUtility::getTimeMS();
	long frameTime = mCurTime - mLastTime;
	mLastTime = mCurTime;
	return frameTime;
}