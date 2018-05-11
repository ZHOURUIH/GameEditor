#ifndef _CAMERA_LINKER_SWITCH_AROUND_TARGET_H_
#define _CAMERA_LINKER_SWITCH_AROUND_TARGET_H_

#include "CameraLinkerSwitch.h"

class CameraLinkerSwitchAroundTarget : public CameraLinkerSwitch
{
public:
	CameraLinkerSwitchAroundTarget(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker);
	virtual ~CameraLinkerSwitchAroundTarget(){ destroy(); }
	virtual void init(const VECTOR3& origin, const VECTOR3& target, const float& speed);
	virtual void update(float elapsedTime);
	virtual void destroy(){}
protected:
	float mTotalAngle;
	float mRotatedAngle;
	bool mClockwise;
	float mDistanceDelta;
	float mDistanceCurrent;
};

#endif