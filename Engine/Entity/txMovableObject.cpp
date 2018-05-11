#include "txRigidBody.h"
#include "txProperty.h"
#include "txMovableObject.h"
#include "txComponentHeader.h"
#include "txTransformProperty.h"

txMovableObject::txMovableObject(const std::string& name)
:
txComponentOwner(name)
{
	// 添加变换组件和刚体组件,这样才能成为一个可移动物体
	mTransformComponent = static_cast<txComponentTransform*>(addComponent("transform", TOSTRING(txComponentTransform)));
	mPhysicalComponent = static_cast<txComponentPhysics*>(addComponent("physics", TOSTRING(txComponentPhysics)));
	// 默认为动力学状态
	mPhysicalComponent->setKinematic(true);
}

void txMovableObject::setPosition(const VECTOR3& position, const bool& refreshNow)
{
	// 需要调用设置组件属性
	mTransformComponent->setPosition(position, refreshNow);
	// 还需要同时设置物理组件中刚体的位置
	mPhysicalComponent->setBodyPosition(position);
}

void txMovableObject::setRotation(const MATRIX3& rotation, const bool& refreshNow)
{
	mTransformComponent->setRotation(rotation, refreshNow);
	// 还需要同时设置物理组件中刚体的旋转
	mPhysicalComponent->setBodyRotation(rotation);
}

void txMovableObject::setScale(const VECTOR3& scale, const bool& refreshNow)
{
	mTransformComponent->setScale(scale, refreshNow);
}

void txMovableObject::setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue)
{
	txComponentOwner::setComponentProperty(component, prop, propertyValue);
	// 可移动物体的设置变换信息有两个途径,调用物体的setPosition等函数,或者是通过setComponentProperty设置组件属性
	// setPosition中已经同步了物理组件的变换属性,所以还需要在setComponentProperty中同步物理组件的变换属性
	if (component->getBaseType() == TOSTRING(txComponentTransform))
	{
		txComponentTransform* transformComponent = static_cast<txComponentTransform*>(component);
		if (prop->getName() == txProperty::getPropertyName<txTransformPropertyPosition>())
		{
			mPhysicalComponent->setBodyPosition(transformComponent->getPosition());
		}
		else if (prop->getName() == txProperty::getPropertyName<txTransformPropertyRotation>())
		{
			mPhysicalComponent->setBodyRotation(transformComponent->getRotation());
		}
	}
}

const VECTOR3& txMovableObject::getPosition()
{
	return mTransformComponent->getPosition();
}

const MATRIX3& txMovableObject::getRotation()
{
	return mTransformComponent->getRotation();
}

const VECTOR3& txMovableObject::getScale()
{
	return mTransformComponent->getScale();
}

void txMovableObject::rotate(const MATRIX3& rotation, const bool& refreshNow)
{
	setRotation(rotation * getRotation(), refreshNow);
}

void txMovableObject::yaw(const float& angle, const bool& refreshNow)
{
	rotate(txMath::getYawMatrix3(angle));
}

void txMovableObject::pitch(const float& angle, const bool& refreshNow)
{
	rotate(txMath::getPitchMatrix3(angle));
}

void txMovableObject::roll(const float& angle, const bool& refreshNow)
{
	rotate(txMath::getRollMatrix3(angle));
}

txNode* txMovableObject::getNode()
{
	return mTransformComponent->getNode();
}

void txMovableObject::setLinearVelocity(const VECTOR3& speed)
{
	mPhysicalComponent->setLinearVelocity(speed);
}

const VECTOR3& txMovableObject::getLinearVelocity()
{
	return mPhysicalComponent->getLinearVelocity();
}