#ifndef _TX_COLLISION_CAPSULE_PROPERTY_H_
#define _TX_COLLISION_CAPSULE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionCapsule);

// 中间圆柱体的高度
class txCollisionCapsulePropertyCenterCylinderHeight : public txComponentCollisionCapsuleProperty<txFloatProperty, float>
{
public:
	txCollisionCapsulePropertyCenterCylinderHeight();
	virtual void setRealValue(const float& value, txComponentCollisionCapsule* component);
	virtual float getRealValue(txComponentCollisionCapsule* component);
};

//---------------------------------------------------------------------------------------------------------
// 两端半球体半径
class txCollisionCapsulePropertyHalfSphereRadius : public txComponentCollisionCapsuleProperty<txFloatProperty, float>
{
public:
	txCollisionCapsulePropertyHalfSphereRadius();
	virtual void setRealValue(const float& value, txComponentCollisionCapsule* component);
	virtual float getRealValue(txComponentCollisionCapsule* component);
};

#endif