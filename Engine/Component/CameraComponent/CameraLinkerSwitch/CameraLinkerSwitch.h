#ifndef _CAMERA_LINKER_SWITCH_H_
#define _CAMERA_LINKER_SWITCH_H_

#include "txEngineDefine.h"

// 摄像机连接器的转换器,用于处理摄像机连接器切换时的转换逻辑,目前仅仅能给固定连接器使用
// 实际是计算更新固定连接器的相对位置
class CameraLinker;
class CameraLinkerSwitch
{
public:
	CameraLinkerSwitch(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker)
		:
		mType(type),
		mParentLinker(parentLinker)
	{}
	virtual ~CameraLinkerSwitch(){ destroy(); }
	virtual void init(const VECTOR3& origin, const VECTOR3& target, const float& speed)
	{
		mOriginRelative = origin;
		mTargetRelative = target;
		mSpeed = speed;
	}
	virtual void update(float elapsedTime) = 0;
	virtual void destroy(){ mParentLinker = NULL; }
	void setOriginRelative(const VECTOR3& origin) { mOriginRelative = origin; }
	void setTargetRelative(const VECTOR3& target) { mTargetRelative = target; }
	void setSwitchSpeed(const float& speed) { mSpeed = speed; }
	const VECTOR3& getOriginRelative() { return mOriginRelative; }
	const VECTOR3& getTargetRelative() { return mTargetRelative; }
	const float& getSwitchSpeed() { return mSpeed; }
	const CAMERA_LINKER_SWITCH& getType() { return mType; }
	CameraLinker* getLinker() { return mParentLinker; }
protected:
	CAMERA_LINKER_SWITCH mType;
	CameraLinker* mParentLinker;
	VECTOR3 mOriginRelative;
	VECTOR3 mTargetRelative;
	// 转换器的速度,不同的转换器速度含义不同
	// 直线转换器是直线速度
	// 环形转换器是角速度
	// 绕目标旋转转换器是角速度
	float mSpeed;
};

#endif