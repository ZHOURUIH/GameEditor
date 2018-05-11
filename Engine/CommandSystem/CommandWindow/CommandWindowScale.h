#ifndef _COMMAND_WINDOW_SCALE_H_
#define _COMMAND_WINDOW_SCALE_H_

#include "txCommand.h"

class CommandWindowScale : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLoop = false;
		mOnceLength = 1.0f;
		mTimeOffset = 0.0f;
		mFullOnce = false;
		mAmplitude = 1.0f;
		mStartScale = txMath::VEC2_ZERO;
		mTargetScale = txMath::VEC2_ONE;
		mDoingCallback = NULL;
		mDoingUserData = NULL;
		mDoneCallback = NULL;
		mDoneUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setDoingCallback(KeyFrameCallback scalCallback, void* userData)
	{
		mDoingCallback = scalCallback;
		mDoingUserData = userData;
	}
	void setDoneCallback(KeyFrameCallback scalCallback, void* userData)
	{
		mDoneCallback = scalCallback;
		mDoneUserData = userData;
	}
public:
	std::string mName;
	bool mLoop;
	float mOnceLength;					// 缩放的总时间
	float mTimeOffset;					// 缩放的起始时间偏移
	bool mFullOnce;
	float mAmplitude;
	VECTOR2 mStartScale;				// 窗口起始缩放值
	VECTOR2 mTargetScale;				// 窗口的目标缩放值
protected:
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void* mDoneUserData;
};

#endif