#ifndef _TX_COMPONENT_ROTATE_SPEED_H_
#define _TX_COMPONENT_ROTATE_SPEED_H_

#include "txComponent.h"

class txComponentRotateSpeed : public txComponent
{
public:
	txComponentRotateSpeed(const std::string& type, const std::string& name)
		:
		txComponent(type, name)
	{
		mPlayState = PS_STOP;
	}
	virtual ~txComponentRotateSpeed(){ destroy(); }
	virtual void initProperty();
	virtual void update(float elapsedTime);
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentRotateSpeed); }
	virtual bool isType(const std::string& type)
	{
		return type == TOSTRING(txComponentRotateSpeed);
	}
	virtual void setActive(const bool& active)
	{
		txComponent::setActive(active);
		if (!active)
		{
			setPlayState(PS_STOP);
		}
	}
	void destroy(){}
	const VECTOR3& getRotateSpeed() { return mRotateSpeed; }
	const VECTOR3& getRotateAcceleration() { return mRotateAcceleration; }
	const PLAY_STATE& getPlayState() { return mPlayState; }
	void setRotateSpeed(const VECTOR3& speed) { mRotateSpeed = speed; }
	void setRotateAcceleration(const VECTOR3& acceleration) { mRotateAcceleration = acceleration; }
	void setPlayState(const PLAY_STATE& state) { mPlayState = state; }
	void startRotateSpeed(const VECTOR3& startAngle, const VECTOR3& rotateSpeed, const VECTOR3& rotateAcceleration);
protected:
	virtual void applyRotation(const MATRIX3& rot);
protected:
	VECTOR3 mRotateSpeed;			// 欧拉角旋转速度
	VECTOR3 mRotateAcceleration;	// 旋转加速度
	VECTOR3 mCurRotation;
	PLAY_STATE mPlayState;
};

#endif