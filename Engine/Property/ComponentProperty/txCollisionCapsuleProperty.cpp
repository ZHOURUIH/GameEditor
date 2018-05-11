#include "txCollisionCapsuleProperty.h"

txCollisionCapsulePropertyCenterCylinderHeight::txCollisionCapsulePropertyCenterCylinderHeight()
{
	mReceiverType = TOSTRING(txComponentCollisionCapsule);
	mPropertyName = "CenterCylinderHeight";
	mDescription = "中间圆柱体高度";
}
void txCollisionCapsulePropertyCenterCylinderHeight::setRealValue(const float& value, txComponentCollisionCapsule* component)
{
	component->setCylinderHeight(value);
}
float txCollisionCapsulePropertyCenterCylinderHeight::getRealValue(txComponentCollisionCapsule* component)
{
	return component->getCylinderHeight();
}

//----------------------------------------------------------------------------------------------------------------
txCollisionCapsulePropertyHalfSphereRadius::txCollisionCapsulePropertyHalfSphereRadius()
{
	mReceiverType = TOSTRING(txComponentCollisionCapsule);
	mPropertyName = "HalfSphereRadius";
	mDescription = "两端半球体半径";
}
void txCollisionCapsulePropertyHalfSphereRadius::setRealValue(const float& value, txComponentCollisionCapsule* component)
{
	component->setRadius(value);
}
float txCollisionCapsulePropertyHalfSphereRadius::getRealValue(txComponentCollisionCapsule* component)
{
	return component->getRadius();
}