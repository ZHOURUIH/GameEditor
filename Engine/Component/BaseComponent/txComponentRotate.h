#ifndef _TX_COMPONENT_ROTATE_H_
#define _TX_COMPONENT_ROTATE_H_

#include "txComponentKeyFrame.h"

class txComponentRotate : public txComponentKeyFrame
{
public:
	txComponentRotate(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~txComponentRotate(){ }
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentRotate); }
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentRotate); }
	void setStartAngle(const VECTOR3& startAngle) { mStartAngle = startAngle; }
	void setTargetAngle(const VECTOR3& targetAngle) { mTargetAngle = targetAngle; }
	const VECTOR3& getStartAngle() { return mStartAngle; }
	const VECTOR3& getTargetAngle() { return mTargetAngle; }
protected:
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	VECTOR3 mStartAngle;
	VECTOR3 mTargetAngle;
};

#endif