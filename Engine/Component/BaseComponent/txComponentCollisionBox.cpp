#include "txGeometryBox.h"
#include "txRigidBody.h"
#include "txComponentHeader.h"
#include "txCollisionBoxProperty.h"
#include "txEngineLog.h"

txGeometry* txComponentCollisionBox::createGeometry(txComponentPhysics* physicalComponent)
{
	mBoxGeometry = physicalComponent->getRigidBody()->addBox(1.0f, txMath::VEC3_ZERO, 1.0f, 1.0f, 1.0f, txMath::VEC3_ZERO);
	return mBoxGeometry;
}

void txComponentCollisionBox::afterCreateGeometry()
{
	txComponentCollision::afterCreateGeometry();
	mWidthX = mBoxGeometry->getWidth();
	mHeightY = mBoxGeometry->getHeight();
	mLengthZ = mBoxGeometry->getLength();
}

void txComponentCollisionBox::initProperty()
{
	txComponentCollision::initProperty();
	addProperty<txCollisionBoxPropertyBoxWidth>();
	addProperty<txCollisionBoxPropertyBoxHeight>();
	addProperty<txCollisionBoxPropertyBoxLength>();
}

void txComponentCollisionBox::setWidth(const float& widthX)
{
	mWidthX = widthX;
	if (mBoxGeometry != NULL)
	{
		mBoxGeometry->setWidth(widthX);
	}
}

void txComponentCollisionBox::setHeight(const float& heightY)
{
	mHeightY = heightY;
	if (mBoxGeometry != NULL)
	{
		mBoxGeometry->setHeight(heightY);
	}
}

void txComponentCollisionBox::setLength(const float& lengthZ)
{
	mLengthZ = lengthZ;
	if (mBoxGeometry != NULL)
	{
		mBoxGeometry->setLength(lengthZ);
	}
}