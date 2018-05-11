#include "txCollisionSphereProperty.h"

txCollisionSpherePropertySphereRadius::txCollisionSpherePropertySphereRadius()
{
	mReceiverType = TOSTRING(txComponentCollisionSphere);
	mPropertyName = "SphereRadius";
	mDescription = "ÇòÌå°ë¾¶";
}
void txCollisionSpherePropertySphereRadius::setRealValue(const float& value, txComponentCollisionSphere* component)
{
	component->setRadius(value);
}
float txCollisionSpherePropertySphereRadius::getRealValue(txComponentCollisionSphere* component)
{
	return component->getRadius();
}