#include "txCollisionPlaneProperty.h"

txCollisionPlanePropertyPlaneWidthX::txCollisionPlanePropertyPlaneWidthX()
{
	mReceiverType = TOSTRING(txComponentCollisionPlane);
	mPropertyName = "PlaneWidthX";
	mDescription = "平面X方向上宽度";
}
void txCollisionPlanePropertyPlaneWidthX::setRealValue(const float& value, txComponentCollisionPlane* component)
{
	component->setWidthX(value);
}
float txCollisionPlanePropertyPlaneWidthX::getRealValue(txComponentCollisionPlane* component)
{
	return component->getWidthX();
}

//------------------------------------------------------------------------------------------------------------
txCollisionPlanePropertyPlaneLengthZ::txCollisionPlanePropertyPlaneLengthZ()
{
	mReceiverType = TOSTRING(txComponentCollisionPlane);
	mPropertyName = "PlaneLengthZ";
	mDescription = "平面Z方向上长度";
}
void txCollisionPlanePropertyPlaneLengthZ::setRealValue(const float& value, txComponentCollisionPlane* component)
{
	component->setLengthZ(value);
}
float txCollisionPlanePropertyPlaneLengthZ::getRealValue(txComponentCollisionPlane* component)
{
	return component->getLengthZ();
}