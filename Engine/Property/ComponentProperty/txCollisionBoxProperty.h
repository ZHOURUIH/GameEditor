#ifndef _TX_COLLISION_BOX_PROPERTY_H_
#define _TX_COLLISION_BOX_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionBox);

//---------------------------------------------------------------------------------------------------------
// 盒子宽度
class txCollisionBoxPropertyBoxWidth : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxWidth();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

//---------------------------------------------------------------------------------------------------------
// 盒子高度
class txCollisionBoxPropertyBoxHeight : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxHeight();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

//---------------------------------------------------------------------------------------------------------
// 盒子长度
class txCollisionBoxPropertyBoxLength : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxLength();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

#endif