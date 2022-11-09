#include "TimeLock.h"

TimeLock::TimeLock(const llong frameTimeMS, const llong forceSleep):
	mFrameTimeMS(frameTimeMS),
	mForceSleep(forceSleep),
	mLastTime(getRealTimeMS()),
	mCurTime(mLastTime)
{
	clampMin(mForceSleep, (llong)1);
}

long TimeLock::update()
{
	const llong endTime = getRealTimeMS();
	llong remainMS = (llong)mFrameTimeMS - (endTime - mLastTime);
	clampMin(remainMS, mForceSleep);
	sleep((ulong)remainMS);
	mCurTime = getRealTimeMS();
	const long frameTime = (long)(mCurTime - mLastTime);
	mLastTime = mCurTime;
	return frameTime;
}