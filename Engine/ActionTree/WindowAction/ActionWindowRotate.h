#ifndef _ACTION_WINDOW_ROTATE_H_
#define _ACTION_WINDOW_ROTATE_H_

#include "ActionWindow.h"

class ActionWindowRotate : public ActionWindow
{
public:
	ActionWindowRotate(const std::string& typeName, const std::string& name)
		:
		ActionWindow(typeName, name),
		mDoingCallback(NULL),
		mDoingUserData(NULL),
		mInverseDoingCallback(NULL),
		mInverseDoingUserData(NULL)
	{}
	virtual ~ActionWindowRotate(){ destroy(); }
	virtual void initProperty();
	virtual void execute(const float& timeOffset, const bool& direction = true);
	void destroy(){}
	virtual void setDefaultParam()
	{
		setParameter(txMath::VEC3_ZERO, txMath::VEC3_ZERO, 1.0f);
	}
	// 此处强制需要外部设置起始的旋转值,不允许在实际执行时实时获取起始旋转值(因为那样做比较麻烦)
	void setParameter(const VECTOR3& start, const VECTOR3& target, const float& rotateTime)
	{
		mStartAngle = start;
		mTargetAngle = target;
		setExecuteTime(rotateTime);
	}
	void setRotateCallback(KeyFrameCallback callback, void* userData)
	{
		mDoingCallback = callback;
		mDoingUserData = userData;
	}
	void setInverseRotateCallback(KeyFrameCallback callback, void* userData)
	{
		mInverseDoingCallback = callback;
		mInverseDoingUserData = userData;
	}
	void setStartRotation(const VECTOR3& rot) { mStartAngle = rot; }
	void setTargetRotation(const VECTOR3& rot) { mTargetAngle = rot; }
	const VECTOR3& getStartRotation() { return mStartAngle; }
	const VECTOR3& getTargetRotation() { return mTargetAngle; }
protected:
	static void rotateDone(txComponentKeyFrame* component, void* userData, const bool& breakRotate, const bool& done);
	static void inverseRotateDone(txComponentKeyFrame* component, void* userData, const bool& breakRotate, const bool& done);
	virtual void recoverAction();
	virtual void pauseAction();
	virtual void resumeAction(const bool& direction = true);
	virtual void stopAction();
protected:
	VECTOR3 mTargetAngle;
	VECTOR3 mStartAngle;
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mInverseDoingCallback;
	void* mInverseDoingUserData;
};

#endif