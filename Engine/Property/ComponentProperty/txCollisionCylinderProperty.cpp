#include "txCollisionCylinderProperty.h"

txCollisionCylinderPropertyCylinderHeight::txCollisionCylinderPropertyCylinderHeight()
{
	mReceiverType = TOSTRING(txComponentCollisionCylinder);
	mPropertyName = "CylinderHeight";
	mDescription = "圆柱体高度";
}
void txCollisionCylinderPropertyCylinderHeight::setRealValue(const float& value, txComponentCollisionCylinder* component)
{
	component->setCylinderHeight(value);
}
float txCollisionCylinderPropertyCylinderHeight::getRealValue(txComponentCollisionCylinder* component)
{
	return component->getCylinderHeight();
}

//----------------------------------------------------------------------------------------------------------------
txCollisionCylinderPropertyBottomRadius::txCollisionCylinderPropertyBottomRadius()
{
	mReceiverType = TOSTRING(txComponentCollisionCylinder);
	mPropertyName = "BottomRadius";
	mDescription = "底面半径";
}
void txCollisionCylinderPropertyBottomRadius::setRealValue(const float& value, txComponentCollisionCylinder* component)
{
	component->setRadius(value);
}
float txCollisionCylinderPropertyBottomRadius::getRealValue(txComponentCollisionCylinder* component)
{
	return component->getRadius();
}