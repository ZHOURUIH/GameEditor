#ifndef _COMMAND_WINDOW_ROTATE_H_
#define _COMMAND_WINDOW_ROTATE_H_

#include "txCommand.h"

class CommandWindowRotate : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLoop = false;
		mFullOnce = false;
		mAmplitude = 1.0f;
		mStartAngle = txMath::VEC3_ZERO;
		mTargetAngle = txMath::VEC3_ONE;
		mOnceLength = 1.0f;
		mTimeOffset = 0.0f;
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
	float mAmplitude;
	VECTOR3 mStartAngle;
	VECTOR3 mTargetAngle;
	float mOnceLength;
	float mTimeOffset;
protected:
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void* mDoneUserData;
};

#endif