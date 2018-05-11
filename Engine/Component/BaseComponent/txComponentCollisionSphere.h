#ifndef _TX_COMPONENT_COLLISION_SPHERE_H_
#define _TX_COMPONENT_COLLISION_SPHERE_H_

#include "txComponentCollision.h"

class txGeometrySphere;
class txComponentCollisionSphere : public txComponentCollision
{
public:
	txComponentCollisionSphere(const std::string& type, const std::string& name)
		:
		txComponentCollision(type, name),
		mSphereGeometry(NULL),
		mRadius(0.0f)
	{}
	virtual ~txComponentCollisionSphere(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		bool isCollision = txComponentCollision::isType(type);
		return isCollision || type == TOSTRING(txComponentCollisionSphere);
	}
	void destroy(){ mSphereGeometry = NULL; }
	// µ×Ãæ°ë¾¶
	void setRadius(const float& radius);
	const float& getRadius(){ return mRadius; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent);
	virtual void afterCreateGeometry();
protected:
	txGeometrySphere* mSphereGeometry;
	float mRadius;
};

#endif