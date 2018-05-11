#ifndef _CAMERA_LINKER_SWITCH_CIRCLE_H_
#define _CAMERA_LINKER_SWITCH_CIRCLE_H_

#include "CameraLinkerSwitch.h"

class CameraLinkerSwitchCircle : public CameraLinkerSwitch
{
public:
	CameraLinkerSwitchCircle(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker)
		:
		CameraLinkerSwitch(type, parentLinker),
		mRotatedAngle(0.0f),
		mTotalAngle(txMath::MATH_PI)
	{
		mSpeed = txMath::MATH_PI;
	}
	virtual ~CameraLinkerSwitchCircle(){ destroy(); }
	virtual void init(const VECTOR3& origin, const VECTOR3& target, const float& speed);
	virtual void update(float elapsedTime);
	virtual void destroy()
	{
		mRotatedAngle = 0.0f;
	}
protected:
	float mRotatedAngle; // 已经旋转过的角度
	VECTOR3 mRotateCenter;	// 高度忽略的旋转圆心
	float mTotalAngle;
};

#endif