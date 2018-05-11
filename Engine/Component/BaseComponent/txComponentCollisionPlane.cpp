#include "txGeometryPlane.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txCollisionPlaneProperty.h"

txGeometry* txComponentCollisionPlane::createGeometry(txComponentPhysics* physicalComponent)
{
	mPlaneGeometry = physicalComponent->getRigidBody()->addPlane(1.0f, txMath::VEC3_ZERO, 1.0f, 1.0f, txMath::VEC3_ZERO);
	return mPlaneGeometry;
}

void txComponentCollisionPlane::afterCreateGeometry()
{
	txComponentCollision::afterCreateGeometry();
	mWidthX = mPlaneGeometry->getWidthX();
	mLengthZ = mPlaneGeometry->getLengthZ();
}

void txComponentCollisionPlane::initProperty()
{
	txComponentCollision::initProperty();
	addProperty<txCollisionPlanePropertyPlaneWidthX>();
	addProperty<txCollisionPlanePropertyPlaneLengthZ>();
}

void txComponentCollisionPlane::setWidthX(const float& widthX)
{
	mWidthX = widthX;
	if (mPlaneGeometry != NULL)
	{
		mPlaneGeometry->setWidthX(widthX);
	}
}

void txComponentCollisionPlane::setLengthZ(const float& lengthZ)
{
	mLengthZ = lengthZ;
	if (mPlaneGeometry != NULL)
	{
		mPlaneGeometry->setLengthZ(lengthZ);
	}
}