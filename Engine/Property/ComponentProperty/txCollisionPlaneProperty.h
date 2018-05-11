#ifndef _TX_COLLISION_PLANE_PROPERTY_H_
#define _TX_COLLISION_PLANE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionPlane)

// 平面X方向上宽度
class txCollisionPlanePropertyPlaneWidthX : public txComponentCollisionPlaneProperty<txFloatProperty, float>
{
public:
	txCollisionPlanePropertyPlaneWidthX();
	virtual void setRealValue(const float& value, txComponentCollisionPlane* component);
	virtual float getRealValue(txComponentCollisionPlane* component);
};

//---------------------------------------------------------------------------------------------------------
// 平面Z方向上长度
class txCollisionPlanePropertyPlaneLengthZ : public txComponentCollisionPlaneProperty<txFloatProperty, float>
{
public:
	txCollisionPlanePropertyPlaneLengthZ();
	virtual void setRealValue(const float& value, txComponentCollisionPlane* component);
	virtual float getRealValue(txComponentCollisionPlane* component);
};

#endif