#ifndef _TX_PHYSICS_PROPERTY_H_
#define _TX_PHYSICS_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentPhysics)

// 是否显示所有的碰撞体
class txPhysicsPropertyShowAllGeometry : public txComponentPhysicsProperty<txBoolProperty, bool>
{
public:
	txPhysicsPropertyShowAllGeometry();
	virtual void setRealValue(const bool& value, txComponentPhysics* component);
	virtual bool getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 是否为动力学状态
class txPhysicsPropertyKinematic : public txComponentPhysicsProperty<txBoolProperty, bool>
{
public:
	txPhysicsPropertyKinematic();
	virtual void setRealValue(const bool& value, txComponentPhysics* component);
	virtual bool getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的线速度
class txPhysicsPropertyLinearVelocity : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyLinearVelocity();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的角速度
class txPhysicsPropertyAngularVelocity : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyAngularVelocity();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的恒定外力
class txPhysicsPropertyConstantForce : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyConstantForce();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的瞬间外力
class txPhysicsPropertyInstantForce : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyInstantForce();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的恒定扭矩
class txPhysicsPropertyConstantTorque : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyConstantTorque();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// 物体的瞬间扭矩
class txPhysicsPropertyInstantTorque : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyInstantTorque();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

#endif