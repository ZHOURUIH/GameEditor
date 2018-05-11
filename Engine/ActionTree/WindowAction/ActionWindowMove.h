#ifndef _ACTION_WINDOW_MOVE_H_
#define _ACTION_WINDOW_MOVE_H_

#include "txPoint.h"
#include "ActionWindow.h"

class ActionWindowMove : public ActionWindow
{
public:
	ActionWindowMove(const std::string& typeName, const std::string& name)
		:
		ActionWindow(typeName, name),
		mDoingCallback(NULL),
		mDoingUserData(NULL),
		mInverseDoingCallback(NULL),
		mInverseDoingUserData(NULL)
	{}
	virtual ~ActionWindowMove(){ destroy(); }
	virtual void initProperty();
	virtual void execute(const float& timeOffset, const bool& direction = true);
	void destroy(){}
	virtual void setDefaultParam()
	{
		setParameter(txPoint::ZERO, txPoint::ZERO, 1.0f);
	}
	void setParameter(const txPoint& start, const txPoint& target, const float& moveTime)
	{
		mStartPosition = start;
		mTargetPosition = target;
		setExecuteTime(moveTime);
	}
	void setWindowCallback(KeyFrameCallback callback, void* userData)
	{
		mDoingCallback = callback;
		mDoingUserData = userData;
	}
	void setInverseWindowCallback(KeyFrameCallback callback, void* userData)
	{
		mInverseDoingCallback = callback;
		mInverseDoingUserData = userData;
	}
	void setStartPos(const txPoint& pos) { mStartPosition = pos; }
	void setTargetPos(const txPoint& pos) { mTargetPosition = pos; }
	const txPoint& getStartPos() { return mStartPosition; }
	const txPoint& getTargetPos() { return mTargetPosition; }
protected:
	static void moveDone(txComponentKeyFrame* moveComponent, void* userdata, const bool& breakMove, const bool& done);
	static void inverseMoveDone(txComponentKeyFrame* moveComponent, void* userdata, const bool& breakMove, const bool& done);
	virtual void recoverAction();
	virtual void pauseAction();
	virtual void resumeAction(const bool& direction = true);
	virtual void stopAction();
protected:
	txPoint mStartPosition;
	txPoint mTargetPosition;
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mInverseDoingCallback;
	void* mInverseDoingUserData;
};

#endif