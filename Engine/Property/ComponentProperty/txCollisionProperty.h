#ifndef _TX_COLLISION_PROPERTY_H_
#define _TX_COLLISION_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollision)

//---------------------------------------------------------------------------------------------------------
// 碰撞体质量
class txCollisionPropertyGeometryMass : public txComponentCollisionProperty<txFloatProperty, float>
{
public:
	txCollisionPropertyGeometryMass();
	virtual void setRealValue(const float& value, txComponentCollision* component);
	virtual float getRealValue(txComponentCollision* component);
};

//---------------------------------------------------------------------------------------------------------
// 是否显示碰撞体
class txCollisionPropertyGeometryShowGeometry : public txComponentCollisionProperty<txBoolProperty, bool>
{
public:
	txCollisionPropertyGeometryShowGeometry();
	virtual void setRealValue(const bool& value, txComponentCollision* component);
	virtual bool getRealValue(txComponentCollision* component);
};

//---------------------------------------------------------------------------------------------------------
// 碰撞体相对于刚体的位置
class txCollisionPropertyGeometryRelativePosition : public txComponentCollisionProperty<txVector3Property, VECTOR3>
{
public:
	txCollisionPropertyGeometryRelativePosition();
	virtual void setRealValue(const VECTOR3& value, txComponentCollision* component);
	virtual VECTOR3 getRealValue(txComponentCollision* component);
};

//---------------------------------------------------------------------------------------------------------
// 碰撞体相对于刚体的旋转
class txCollisionPropertyGeometryRelativeRotation : public txComponentCollisionProperty<txVector3Property, VECTOR3>
{
public:
	txCollisionPropertyGeometryRelativeRotation();
	virtual void setRealValue(const VECTOR3& value, txComponentCollision* component);
	virtual VECTOR3 getRealValue(txComponentCollision* component);
};

#endif