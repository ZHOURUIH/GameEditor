#ifndef _TX_COLLISION_SPHERE_PROPERTY_H_
#define _TX_COLLISION_SPHERE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionSphere)

// ÇòÌå°ë¾¶
class txCollisionSpherePropertySphereRadius : public txComponentCollisionSphereProperty<txFloatProperty, float>
{
public:
	txCollisionSpherePropertySphereRadius();
	virtual void setRealValue(const float& value, txComponentCollisionSphere* component);
	virtual float getRealValue(txComponentCollisionSphere* component);
};

#endif