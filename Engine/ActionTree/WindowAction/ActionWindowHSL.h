#ifndef _ACTION_WINDOW_HSL_H_
#define _ACTION_WINDOW_HSL_H_

#include "ActionWindow.h"

class ActionWindowHSL : public ActionWindow
{
public:
	ActionWindowHSL(const std::string& typeName, const std::string& name)
		:
		ActionWindow(typeName, name),
		mDoingCallback(NULL),
		mDoingUserData(NULL),
		mInverseDoingCallback(NULL),
		mInverseDoingUserData(NULL)
	{}
	virtual ~ActionWindowHSL(){ destroy(); }
	virtual void initProperty();
	virtual void execute(const float& timeOffset, const bool& direction = true);
	void destroy(){}
	virtual void setDefaultParam()
	{
		setParameter(1.0f, txMath::VEC3_ZERO, txMath::VEC3_ZERO);
	}
	void setParameter(const float& fadeTime, const VECTOR3& startHSL, const VECTOR3& targetHSL)
	{
		mStartHSL = startHSL;
		mTargetHSL = targetHSL;
		setExecuteTime(fadeTime);
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
	void setStartHSL(const VECTOR3& hsl) { mStartHSL = hsl; }
	void setTargetHSL(const VECTOR3 hsl) { mTargetHSL = hsl; }
	const VECTOR3& getStartHSL() { return mStartHSL; }
	const VECTOR3& getTargetHSL() { return mTargetHSL; }
protected:
	static void hslDone(txComponentKeyFrame* component, void* userData, const bool& breakHSL, const bool& done);
	static void inverseHSLDone(txComponentKeyFrame* component, void* userData, const bool& breakHSL, const bool& done);
	virtual void recoverAction();
	virtual void pauseAction();
	virtual void resumeAction(const bool& direction = true);
	virtual void stopAction();
protected:
	VECTOR3 mStartHSL;				// 窗口起始HSL偏移
	VECTOR3 mTargetHSL;				// 窗口目标HSL偏移
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mInverseDoingCallback;
	void* mInverseDoingUserData;
};

#endif