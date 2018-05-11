#ifndef _ACTION_WINDOW_ALPHA_H_
#define _ACTION_WINDOW_ALPHA_H_

#include "ActionWindow.h"

class ActionWindowAlpha : public ActionWindow
{
public:
	ActionWindowAlpha(const std::string& typeName, const std::string& name)
		:
		ActionWindow(typeName, name),
		mStartAlpha(0.0f),
		mTargetAlpha(1.0f),
		mChangingCallback(NULL),
		mInverseChangingCallback(NULL),
		mChangingData(NULL),
		mInverseChangingData(NULL)
	{}
	virtual ~ActionWindowAlpha(){}
	virtual void initProperty();
	virtual void execute(const float& timeOffset, const bool& direction = true);
	virtual void setDefaultParam()
	{
		setParameters(0.0f, 1.0f, 1.0f);
	}
	void setParameters(const float& startAlpha, const float& destAlpha, const float& changeTime)
	{
		mStartAlpha = startAlpha;
		mTargetAlpha = destAlpha;
		setExecuteTime(changeTime);
	}
	void setStartAlpha(const float& alpha)	{ mStartAlpha = alpha; }
	void setTargetAlpha(const float& alpha) { mTargetAlpha = alpha; }
	const float& getStartAlpha()	{ return mStartAlpha; }
	const float& getTargetAlpha()	{ return mTargetAlpha; }
protected:
	virtual void pauseAction();
	virtual void resumeAction(const bool& direction = true);
	virtual void recoverAction();
	virtual void stopAction();
	static void changeDone(txComponentKeyFrame* component, void* userData, const bool& breakLoop, const bool& done);
	static void inverserChangeDone(txComponentKeyFrame* component, void* userData, const bool& breakLoop, const bool& done);
protected:
	float mStartAlpha;
	float mTargetAlpha;
	KeyFrameCallback mChangingCallback;
	KeyFrameCallback mInverseChangingCallback;
	void* mChangingData;
	void* mInverseChangingData;
};

#endif