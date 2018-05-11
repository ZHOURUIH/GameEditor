#ifndef _COMMAND_WINDOW_MOVE_H_
#define _COMMAND_WINDOW_MOVE_H_

#include "txPoint.h"
#include "txCommand.h"

class CommandWindowMove : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLoop = false;
		mTimeOffset = 0.0f;
		mOnceLength = 1.0f;
		mFullOnce = false;
		mAmplitude = 1.0f;
		mStartPosition = txPoint::ZERO;
		mTargetPosition = txPoint::ZERO;
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
	float mOnceLength;
	float mTimeOffset;
	bool mFullOnce;
	float mAmplitude;
	txPoint mStartPosition;
	txPoint mTargetPosition;
protected:
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void* mDoneUserData;
};

#endif