#include "txCollisionProperty.h"

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryMass::txCollisionPropertyGeometryMass()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryMass";
	mDescription = "碰撞体质量";
}
void txCollisionPropertyGeometryMass::setRealValue(const float& value, txComponentCollision* component)
{
	component->setMass(value);
}
float txCollisionPropertyGeometryMass::getRealValue(txComponentCollision* component)
{
	return component->getMass();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryShowGeometry::txCollisionPropertyGeometryShowGeometry()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "ShowGeometry";
	mDescription = "是否显示碰撞体";
}
void txCollisionPropertyGeometryShowGeometry::setRealValue(const bool& value, txComponentCollision* component)
{
	component->setShowGeometry(value);
}
bool txCollisionPropertyGeometryShowGeometry::getRealValue(txComponentCollision* component)
{
	return component->getShowGeometry();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryRelativePosition::txCollisionPropertyGeometryRelativePosition()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryRelativePosition";
	mDescription = "碰撞体相对于刚体的位置";
}
void txCollisionPropertyGeometryRelativePosition::setRealValue(const VECTOR3& value, txComponentCollision* component)
{
	component->setRelativePosition(value);
}
VECTOR3 txCollisionPropertyGeometryRelativePosition::getRealValue(txComponentCollision* component)
{
	return component->getRelativePosition();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryRelativeRotation::txCollisionPropertyGeometryRelativeRotation()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryRelativeRotation";
	mDescription = "碰撞体相对于刚体的旋转欧拉角";
}
void txCollisionPropertyGeometryRelativeRotation::setRealValue(const VECTOR3& value, txComponentCollision* component)
{
	component->setRelativeRotation(txMath::eulerAngleToMatrix3(value));
}
VECTOR3 txCollisionPropertyGeometryRelativeRotation::getRealValue(txComponentCollision* component)
{
	return txMath::matrix3ToEulerAngle(component->getRelativeRotation());
}