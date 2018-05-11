#ifndef _COMMAND_WINDOW_HSL_H_
#define _COMMAND_WINDOW_HSL_H_

#include "txCommand.h"

class CommandWindowHSL : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLoop = false;
		mFullOnce = false;
		mOnceLength = 1.0f;
		mAmplitude = 1.0f;
		mTimeOffset = 0.0f;
		mStartHSL = txMath::VEC3_ZERO;
		mTargetHSL = txMath::VEC3_ONE;
		mDoingCallback = NULL;
		mDoingUserData = NULL;
		mDoneCallback = NULL;
		mDoneUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setDoingCallback(KeyFrameCallback callback, void* userData)
	{
		mDoingCallback = callback;
		mDoingUserData = userData;
	}
	void setDoneCallback(KeyFrameCallback callback, void* userData)
	{
		mDoneCallback = callback;
		mDoneUserData = userData;
	}
public:
	std::string mName;
	bool mLoop;
	bool mFullOnce;
	float mOnceLength;
	float mTimeOffset;
	float mAmplitude;
	VECTOR3 mStartHSL;
	VECTOR3 mTargetHSL;
protected:
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void* mDoneUserData;
};

#endif