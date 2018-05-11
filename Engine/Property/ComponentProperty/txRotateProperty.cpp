#include "txRotateProperty.h"

txRotatePropertyTargetAngle::txRotatePropertyTargetAngle()
{
	mPropertyName = "TargetAngle";
	mDescription = "目标旋转角度";
}

void txRotatePropertyTargetAngle::setRealValue(const VECTOR3& value, txComponentRotate* component)
{
	component->setTargetAngle(value);
}

VECTOR3 txRotatePropertyTargetAngle::getRealValue(txComponentRotate* component)
{
	return component->getTargetAngle();
}

//----------------------------------------------------------------------------------------------------
txRotatePropertyStartAngle::txRotatePropertyStartAngle()
{
	mPropertyName = "StartAngle";
	mDescription = "起始旋转角度";
}
void txRotatePropertyStartAngle::setRealValue(const VECTOR3& value, txComponentRotate* component)
{
	component->setStartAngle(value);
}
VECTOR3 txRotatePropertyStartAngle::getRealValue(txComponentRotate* component)
{
	return component->getStartAngle();
}