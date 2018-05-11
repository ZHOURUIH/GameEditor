#ifndef _CAMERA_LINKER_ACCELERATION_H_
#define _CAMERA_LINKER_ACCELERATION_H_

#include "CameraLinker.h"

// 根据加速度改变距离 连接器
class txSpring;
class CameraLinkerAcceleration : public CameraLinker
{
public:
	CameraLinkerAcceleration(const std::string& type, const std::string& name);
	virtual ~CameraLinkerAcceleration(){ destroy(); }
	virtual void preUpdate(float elapsedTime);
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type) 
	{
		return CameraLinker::isType(type) || type == TOSTRING(CameraLinkerAcceleration);
	}
	void destroy();
	virtual void setRelativePosition(const VECTOR3& pos, const CAMERA_LINKER_SWITCH& switchType = CLS_LINEAR, const bool& useDefaultSwitchSpeed = true, const float& switchSpeed = 1.0f);
protected:
	txSpring* mSpringX;
	txSpring* mSpringY;
	txSpring* mSpringZ;
};

#endif