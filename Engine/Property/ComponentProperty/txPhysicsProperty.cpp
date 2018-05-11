#include "txPhysicsProperty.h"

//---------------------------------------------------------------------------------------------
txPhysicsPropertyShowAllGeometry::txPhysicsPropertyShowAllGeometry()
{
	mPropertyName = "ShowAllGeometry";
	mDescription = "是否显示所有的碰撞体";
}
void txPhysicsPropertyShowAllGeometry::setRealValue(const bool& value, txComponentPhysics* component)
{
	component->setShowAllGeometry(value);
}
bool txPhysicsPropertyShowAllGeometry::getRealValue(txComponentPhysics* component)
{
	return component->getShowAllGeometry();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyKinematic::txPhysicsPropertyKinematic()
{
	mPropertyName = "Kinematic";
	mDescription = "是否为动力学状态,动力学状态下不会进行物理模拟,但是会有碰撞检测";
}
void txPhysicsPropertyKinematic::setRealValue(const bool& value, txComponentPhysics* component)
{
	component->setKinematic(value);
}
bool txPhysicsPropertyKinematic::getRealValue(txComponentPhysics* component)
{
	return component->getKinematic();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyLinearVelocity::txPhysicsPropertyLinearVelocity()
{
	mPropertyName = "LinearVelocity";
	mDescription = "物体的线速度";
}
void txPhysicsPropertyLinearVelocity::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setLinearVelocity(value);
}
VECTOR3 txPhysicsPropertyLinearVelocity::getRealValue(txComponentPhysics* component)
{
	return component->getLinearVelocity();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyAngularVelocity::txPhysicsPropertyAngularVelocity()
{
	mPropertyName = "AngularVelocity";
	mDescription = "物体的角速度,分别是绕X轴,Y轴,Z轴转动的速度,弧度制";
}
void txPhysicsPropertyAngularVelocity::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setAngularVelocity(value);
}
VECTOR3 txPhysicsPropertyAngularVelocity::getRealValue(txComponentPhysics* component)
{
	return component->getAngularVelocity();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyConstantForce::txPhysicsPropertyConstantForce()
{
	mPropertyName = "ConstantForce";
	mDescription = "物体的恒定外力,会一直作用于物体";
}
void txPhysicsPropertyConstantForce::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setConstantForce(value);
}
VECTOR3 txPhysicsPropertyConstantForce::getRealValue(txComponentPhysics* component)
{
	return component->getConstantForce();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyInstantForce::txPhysicsPropertyInstantForce()
{
	mPropertyName = "InstantForce";
	mDescription = "物体的瞬间外力,只是给物体的一个瞬间的外力,不会一直作用于物体";
}
void txPhysicsPropertyInstantForce::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setInstantForce(value);
}
VECTOR3 txPhysicsPropertyInstantForce::getRealValue(txComponentPhysics* component)
{
	return component->getInstantForce();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyConstantTorque::txPhysicsPropertyConstantTorque()
{
	mPropertyName = "ConstantTorque";
	mDescription = "物体的恒定扭矩,会一直作用于物体";
}
void txPhysicsPropertyConstantTorque::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setConstantTorque(value);
}
VECTOR3 txPhysicsPropertyConstantTorque::getRealValue(txComponentPhysics* component)
{
	return component->getConstantTorque();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyInstantTorque::txPhysicsPropertyInstantTorque()
{
	mPropertyName = "InstantTorque";
	mDescription = "物体的瞬间扭矩,只是给物体一个瞬间的扭矩,不会一直作用于物体";
}
void txPhysicsPropertyInstantTorque::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setInstantTorque(value);
}
VECTOR3 txPhysicsPropertyInstantTorque::getRealValue(txComponentPhysics* component)
{
	return component->getInstantTorque();
}