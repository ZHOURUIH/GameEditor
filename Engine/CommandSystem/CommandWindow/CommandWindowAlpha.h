#ifndef _COMMAND_WINDOW_ALPHA_H_
#define _COMMAND_WINDOW_ALPHA_H_

#include "txCommand.h"

class CommandWindowAlpha : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mOnceLength = 1.0f;
		mTimeOffset = 0.0f;
		mLoop = false;
		mAmplitude = 1.0f;
		mFullOnce = false;
		mAlphaChildren = true;
		mStartAlpha = 0.0f;
		mTargetAlpha = 1.0f;
		mDoingCallBack = NULL;
		mDoingUserData = NULL;
		mDoneCallBack = NULL;
		mDoneUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setDoingCallback(KeyFrameCallback callback, void* userData)
	{
		mDoingCallBack = callback;
		mDoingUserData = userData;
	}
	void setDoneCallback(KeyFrameCallback callback, void* userData)
	{
		mDoneCallBack = callback;
		mDoneUserData = userData;
	}
public:
	std::string mName;
	bool mLoop;
	bool mFullOnce;
	float mOnceLength;
	float mTimeOffset;
	float mAmplitude;
	bool mAlphaChildren;
	float mStartAlpha;
	float mTargetAlpha;
protected:
	KeyFrameCallback mDoingCallBack;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallBack;
	void* mDoneUserData;
};

#endif