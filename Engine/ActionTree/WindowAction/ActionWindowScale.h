#ifndef _ACTION_WINDOW_SCALE_H_
#define _ACTION_WINDOW_SCALE_H_

#include "ActionWindow.h"

class ActionWindowScale : public ActionWindow
{
public:
	ActionWindowScale(const std::string& typeName, const std::string& name)
		:
		ActionWindow(typeName, name),
		mDoingCallback(NULL),
		mDoingUserData(NULL),
		mInverseDoingCallback(NULL),
		mInverseDoingUserData(NULL)
	{}
	virtual ~ActionWindowScale(){ destroy(); }
	virtual void initProperty();
	virtual void execute(const float& timeOffset, const bool& direction = true);
	void destroy(){}
	virtual void setDefaultParam()
	{
		setParameter(txMath::VEC2_ONE, txMath::VEC2_ONE, 1.0f);
	}
	void setParameter(const VECTOR2& start, const VECTOR2& target, const float& scaleTime)
	{
		mStartScale = start;
		mTargetScale = target;
		setExecuteTime(scaleTime);
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
	void setStartScale(const VECTOR2& scale) { mStartScale = scale; }
	void setTargetScale(const VECTOR2& scale) { mTargetScale = scale; }
	const VECTOR2& getStartScale() { return mStartScale; }
	const VECTOR2& getTargetScale() { return mTargetScale; }
protected:
	static void scaleDone(txComponentKeyFrame* component, void* userData, const bool& breakScale, const bool& done);
	static void inverseScaleDone(txComponentKeyFrame* component, void* userData, const bool& breakScale, const bool& done);
	virtual void recoverAction();
	virtual void pauseAction();
	virtual void resumeAction(const bool& direction = true);
	virtual void stopAction();
protected:
	VECTOR2 mStartScale;				// 窗口起始缩放值
	VECTOR2 mTargetScale;				// 窗口的目标缩放值
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mInverseDoingCallback;
	void* mInverseDoingUserData;
};

#endif