#ifndef _TX_SPRING_H_
#define _TX_SPRING_H_

#include "txEngineDefine.h"

// 弹簧
class txSpring
{
public:
	txSpring();
	virtual ~txSpring(){}
	void update(float fElaspedTime);
	// 计算拉力 如果为正则是压缩弹簧的方向,为负拉伸弹簧的方向
	float calculateElasticForce()
	{
		float elasticForce = (mCurLength - mNormalLength) * mSpringK;
		return elasticForce;
	}
	void setNormaLength(const float& length) { mNormalLength = length; }
	void setMass(const float& mass) { mObjectMass = mass; }
	void setSpringk(const float& k) { mSpringK = k; }
	void setSpeed(const float& speed) { mObjectSpeed = speed; }
	void setForce(const float& force) { mForce = force; }
	void setCurLength(const float& length) { mCurLength = length; }
	const float& getSpeed(){ return mObjectSpeed; }
	const float& getLength() { return mCurLength; }
	const float& getNomalLength(){ return mNormalLength; }
protected:
	float mNormalLength;
	float mCurLength;
	float mSpringK;
	float mObjectMass;
	float mMinLength;
	// 力和速度 只有正负没有方向,正的是沿着拉伸弹簧的方向,负值压缩弹簧的方向
	float mForce;
	float mObjectSpeed;
	float mPreAcce;
};

#endif