#ifndef _COMMAND_WINDOW_MOVE_RELATIVE_H_
#define _COMMAND_WINDOW_MOVE_RELATIVE_H_

#include "txCommand.h"

class CommandWindowMoveRelative : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mOnceLength = 1.0f;
		mOffset = 0.0f;
		mLoop = false;
		mAmplitude = 1.0f;
		mFullOnce = false;
		mStartPosition = txPoint::ZERO;
		mDoingCallBack = NULL;
		mDoingUserData = NULL;
		mDoneCallBack = NULL;
		mDoneUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setTremblingCallback(KeyFrameCallback callback, void* userData)
	{
		mDoingCallBack = callback;
		mDoingUserData = userData;
	}
	void setTrembleDoneCallback(KeyFrameCallback callback, void* userData)
	{
		mDoneCallBack = callback;
		mDoneUserData = userData;
	}
public:
	std::string mName;
	float mOnceLength;
	float mOffset;
	bool mLoop;
	float mAmplitude;
	bool mFullOnce;
	txPoint mStartPosition;
protected:
	KeyFrameCallback mDoingCallBack;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallBack;
	void* mDoneUserData;
};

#endif