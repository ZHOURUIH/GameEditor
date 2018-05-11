#include "txEngineRoot.h"
#include "txODEPhysics.h"
#include "txRigidBody.h"
#include "Utility.h"
#include "txComponentHeader.h"
#include "txPhysicsProperty.h"

void txComponentPhysics::init(txComponentOwner* owner)
{
	txComponent::init(owner);
	// 创建刚体
	mRigidBody = mODEPhysics->createBody();
}

void txComponentPhysics::initProperty()
{
	txComponent::initProperty();
	addProperty<txPhysicsPropertyShowAllGeometry>();
	addProperty<txPhysicsPropertyKinematic>();
	addProperty<txPhysicsPropertyLinearVelocity>();
	addProperty<txPhysicsPropertyAngularVelocity>();
	addProperty<txPhysicsPropertyConstantForce>();
	addProperty<txPhysicsPropertyInstantForce>();
	addProperty<txPhysicsPropertyConstantTorque>();
	addProperty<txPhysicsPropertyInstantTorque>();
}

void txComponentPhysics::update(float elapsedTime)
{
	if (mComponentOwner != NULL)
	{
		// 获得变换信息,设置到变换组件中
		txComponentTransform* transformComponent = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transformComponent != NULL)
		{
			transformComponent->setPosition(mRigidBody->getPosition());
			transformComponent->setRotation(mRigidBody->getRotation(), true);
		}
	}
	txComponent::update(elapsedTime);
}

void txComponentPhysics::destroy()
{
	// 销毁刚体
	mODEPhysics->destroyBody(mRigidBody);
	mRigidBody = NULL;
}

void txComponentPhysics::setBodyPosition(const VECTOR3& pos)
{
	mRigidBody->setBodyPosition(pos);
}

void txComponentPhysics::setBodyRotation(const MATRIX3& rot)
{
	mRigidBody->setBodyRotation(rot);
}

void txComponentPhysics::setLinearVelocity(const VECTOR3& velocity)
{
	mRigidBody->setLinearVelocity(velocity);
}

const VECTOR3& txComponentPhysics::getLinearVelocity()
{
	return mRigidBody->getLinearVelocity();
}

void txComponentPhysics::setAngularVelocity(const VECTOR3& velocity)
{
	mRigidBody->setAngularVelocity(velocity);
}

const VECTOR3& txComponentPhysics::getAngularVelocity()
{
	return mRigidBody->getAngularVelocity();
}

VECTOR3 txComponentPhysics::getAcceleration()
{
	return txMath::VEC3_ZERO;
}

void txComponentPhysics::setInstantForce(const VECTOR3& force)
{
	mRigidBody->addForce(force);
}

VECTOR3 txComponentPhysics::getInstantForce()
{
	return mRigidBody->getForce();
}

void txComponentPhysics::setConstantForce(const VECTOR3& force)
{
	mRigidBody->setConstantForce(force);
}

const VECTOR3& txComponentPhysics::getConstantForce()
{
	return mRigidBody->getConstantForce();
}

void txComponentPhysics::setInstantTorque(const VECTOR3& torque)
{
	mRigidBody->addTorque(torque);
}

VECTOR3 txComponentPhysics::getInstantTorque()
{
	return mRigidBody->getTorque();
}

void txComponentPhysics::setConstantTorque(const VECTOR3& torque)
{
	mRigidBody->setConstantTorque(torque);
}

const VECTOR3& txComponentPhysics::getConstantTorque()
{
	return mRigidBody->getConstantTorque();
}

void txComponentPhysics::setShowAllGeometry(const bool& show)
{
	mRigidBody->setShowCollideShape(show);
}

bool txComponentPhysics::getShowAllGeometry()
{
	return mRigidBody->getShowCollideShape();
}

void txComponentPhysics::setKinematic(const bool& kinematic)
{
	mRigidBody->setKinematic(kinematic);
}

bool txComponentPhysics::getKinematic()
{
	return mRigidBody->getKinematic();
}