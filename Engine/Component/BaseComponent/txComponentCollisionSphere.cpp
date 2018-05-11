#include "txGeometrySphere.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txCollisionSphereProperty.h"

txGeometry* txComponentCollisionSphere::createGeometry(txComponentPhysics* physicalComponent)
{
	mSphereGeometry = physicalComponent->getRigidBody()->addSphere(1.0f, txMath::VEC3_ZERO, 1.0f, txMath::VEC3_ZERO);
	return mSphereGeometry;
}

void txComponentCollisionSphere::afterCreateGeometry()
{
	txComponentCollision::afterCreateGeometry();
	mRadius = mSphereGeometry->getRadius();
}

void txComponentCollisionSphere::initProperty()
{
	txComponentCollision::initProperty();
	addProperty<txCollisionSpherePropertySphereRadius>();
}

void txComponentCollisionSphere::setRadius(const float& radius)
{
	mRadius = radius;
	if (mSphereGeometry != NULL)
	{
		mSphereGeometry->setRadius(radius);
	}
}