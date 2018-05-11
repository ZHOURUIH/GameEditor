#include "TimeLock.h"
#include "txUtility.h"

void TimeLock::update()
{
	long curTime = timeGetTime();
	long deltaMS = mFrameTimeMS - (curTime - mLastTime);
	if (deltaMS > 0)
	{
		txUtility::sleep(deltaMS);
	}
	else if (mForceSleep > 0)
	{
		txUtility::sleep(mForceSleep);
	}
	mLastTime = timeGetTime();
}