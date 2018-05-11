#ifndef _TX_ROTATE_PROPERTY_H_
#define _TX_ROTATE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentRotate)

// 旋转状态
class txRotatePropertyTargetAngle : public txComponentRotateProperty<txVector3Property, VECTOR3>
{
public:
	txRotatePropertyTargetAngle();
	virtual void setRealValue(const VECTOR3& value, txComponentRotate* component);
	virtual VECTOR3 getRealValue(txComponentRotate* component);
};

//---------------------------------------------------------------------------------------------------------
// 起始旋转角度
class txRotatePropertyStartAngle : public txComponentRotateProperty<txVector3Property, VECTOR3>
{
public:
	txRotatePropertyStartAngle();
	virtual void setRealValue(const VECTOR3& value, txComponentRotate* component);
	virtual VECTOR3 getRealValue(txComponentRotate* component);
};

#endif