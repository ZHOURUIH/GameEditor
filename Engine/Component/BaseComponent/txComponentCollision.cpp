#include "txGeometry.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txCollisionProperty.h"

void txComponentCollision::init(txComponentOwner* owner)
{
	txComponent::init(owner);
	if (mComponentOwner != NULL)
	{
		// 找到物理组件中的刚体,添加形状
		txComponentPhysics* physicalComponent = static_cast<txComponentPhysics*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentPhysics)));
		if (physicalComponent == NULL)
		{
			ENGINE_ERROR("error : can not find active physical component!");
			return;
		}
		mGeometry = createGeometry(physicalComponent);
		afterCreateGeometry();
	}
}

void txComponentCollision::initProperty()
{
	txComponent::initProperty();
	addProperty<txCollisionPropertyGeometryMass>();
	addProperty<txCollisionPropertyGeometryShowGeometry>();
	addProperty<txCollisionPropertyGeometryRelativePosition>();
	addProperty<txCollisionPropertyGeometryRelativeRotation>();
}

void txComponentCollision::afterCreateGeometry()
{
	mMass = mGeometry->getMass().mass;
	mPosition = mGeometry->getRelativePosition();
	mRotation = mGeometry->getRelativeRotation();
	mVisible = mGeometry->getVisible();
	mWireframe = mGeometry->getWireframe();
	mColour = mGeometry->getColour();
}

void txComponentCollision::setParentComponent(txComponent* component)
{
	if (component->getBaseType() != TOSTRING(txComponentPhysics))
	{
		ENGINE_ERROR("error : collision component must attach to physical component!");
		return;
	}
	txComponent::setParentComponent(component);
}

void txComponentCollision::setActive(const bool& active)
{
	txComponent::setActive(active);
	if (mGeometry != NULL)
	{
		mGeometry->setVisible(active);
		mGeometry->setEnable(active);
	}
}

void txComponentCollision::destroy()
{
	if (mComponentOwner != NULL)
	{
		// 找到物理组件中的刚体,添加盒子形状
		txComponentPhysics* physicalComponent = static_cast<txComponentPhysics*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentPhysics)));
		if (physicalComponent == NULL)
		{
			ENGINE_ERROR("error : can not find active physical component!");
			return;
		}
		physicalComponent->getRigidBody()->deleteGeometry(mGeometry);
		mGeometry = NULL;
	}
}

void txComponentCollision::setMass(const float& mass)
{
	mMass = mass;
	if (mGeometry != NULL)
	{
		mGeometry->setMass(mass);
	}
}

void txComponentCollision::setColour(const VECTOR4& colour)
{
	mColour = colour;
	if (mGeometry != NULL)
	{
		mGeometry->setColour(colour);
	}
}

void txComponentCollision::setWireframe(const bool& wireframe)
{
	mWireframe = wireframe;
	if (mGeometry != NULL)
	{
		mGeometry->setWireframe(wireframe);
	}
}

void txComponentCollision::setShowGeometry(const bool& show)
{
	mVisible = show;
	if (mGeometry != NULL)
	{
		mGeometry->setVisible(show);
	}
}

void txComponentCollision::setRelativePosition(const VECTOR3& pos)
{
	mPosition = pos;
	if (mGeometry != NULL)
	{
		mGeometry->setRelativePosition(pos);
	}
}

void txComponentCollision::setRelativeRotation(const MATRIX3& rot)
{
	mRotation = rot;
	if (mGeometry != NULL)
	{
		mGeometry->setRelativeRotation(rot);
	}
}