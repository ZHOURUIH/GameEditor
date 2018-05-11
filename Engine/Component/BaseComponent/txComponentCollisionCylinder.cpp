#include "txGeometryCylinder.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txCollisionCylinderProperty.h"

txGeometry* txComponentCollisionCylinder::createGeometry(txComponentPhysics* physicalComponent)
{
	mCylinderGeometry = physicalComponent->getRigidBody()->addCylinder(1.0f, txMath::VEC3_ZERO, 1.0f, 1.0f, txMath::VEC3_ZERO);
	return mCylinderGeometry;
}

void txComponentCollisionCylinder::afterCreateGeometry()
{
	txComponentCollision::afterCreateGeometry();
	mCylinderHeight = mCylinderGeometry->getHeight();
	mRadius = mCylinderGeometry->getRadius();
}

void txComponentCollisionCylinder::initProperty()
{
	txComponentCollision::initProperty();
	addProperty<txCollisionCylinderPropertyCylinderHeight>();
	addProperty<txCollisionCylinderPropertyBottomRadius>();
}

void txComponentCollisionCylinder::setCylinderHeight(const float& cylinderHeight)
{
	mCylinderHeight = cylinderHeight;
	if (mCylinderGeometry != NULL)
	{
		mCylinderGeometry->setHeight(cylinderHeight);
	}
}

void txComponentCollisionCylinder::setRadius(const float& radius)
{
	mRadius = radius;
	if (mCylinderGeometry != NULL)
	{
		mCylinderGeometry->setRadius(radius);
	}
}