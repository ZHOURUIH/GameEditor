#ifndef _TIME_LOCK_H_
#define _TIME_LOCK_H_

#include "txEngineDefine.h"

class TimeLock
{
public:
	TimeLock(const long& frameTimeMS)
	{
		mFrameTimeMS = frameTimeMS;
		mLastTime = 0;
		mForceSleep = 0;
	}
	void update();
	void setForceSleep(const long& forceTimeMS)
	{
		mForceSleep = forceTimeMS;
	}
protected:
	long mFrameTimeMS;
	long mLastTime;
	long mForceSleep;          // 每帧无暂停时间时强制暂停的毫秒数,避免线程单帧任务繁重时,导致单帧消耗时间大于设定的固定单帧时间时,CPU占用过高的问题
};

#endif