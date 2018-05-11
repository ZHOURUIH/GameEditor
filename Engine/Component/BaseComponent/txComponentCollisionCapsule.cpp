#include "txGeometryCapsule.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txCollisionCapsuleProperty.h"

txGeometry* txComponentCollisionCapsule::createGeometry(txComponentPhysics* physicalComponent)
{
	mCapsuleGeometry = physicalComponent->getRigidBody()->addCapsule(1.0f, txMath::VEC3_ZERO, 1.0f, 1.0f, txMath::VEC3_ZERO);
	return mCapsuleGeometry;
}

void txComponentCollisionCapsule::afterCreateGeometry()
{
	txComponentCollision::afterCreateGeometry();
	mCylinderHeight = mCapsuleGeometry->getCylinderHeight();
	mRadius = mCapsuleGeometry->getRadius();
}

void txComponentCollisionCapsule::initProperty()
{
	txComponentCollision::initProperty();
	addProperty<txCollisionCapsulePropertyCenterCylinderHeight>();
	addProperty<txCollisionCapsulePropertyHalfSphereRadius>();
}

void txComponentCollisionCapsule::setCylinderHeight(const float& cylinderHeight)
{
	mCylinderHeight = cylinderHeight;
	if (mCapsuleGeometry != NULL)
	{
		mCapsuleGeometry->setCylinderHeight(cylinderHeight);
	}
}

void txComponentCollisionCapsule::setRadius(const float& radius)
{
	mRadius = radius;
	if (mCapsuleGeometry != NULL)
	{
		mCapsuleGeometry->setRadius(radius);
	}
}