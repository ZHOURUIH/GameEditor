#ifndef _TX_COMPONENT_COLLISION_CAPSULE_H_
#define _TX_COMPONENT_COLLISION_CAPSULE_H_

#include "txComponentCollision.h"

class txGeometryCapsule;
class txComponentCollisionCapsule : public txComponentCollision
{
public:
	txComponentCollisionCapsule(const std::string& type, const std::string& name)
		:
		txComponentCollision(type, name),
		mCapsuleGeometry(NULL), 
		mCylinderHeight(0.0f),
		mRadius(0.0f)
	{}
	virtual ~txComponentCollisionCapsule(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		bool isCollision = txComponentCollision::isType(type);
		return isCollision || type == TOSTRING(txComponentCollisionCapsule);
	}
	void destroy(){ mCapsuleGeometry = NULL; }
	// 中间圆柱体的高度
	void setCylinderHeight(const float& cylinderHeight);
	const float& getCylinderHeight(){ return mCylinderHeight; }
	// 两端半球体的半径
	void setRadius(const float& radius);
	const float& getRadius() { return mRadius; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent);
	virtual void afterCreateGeometry();
protected:
	txGeometryCapsule* mCapsuleGeometry;
	float mCylinderHeight;
	float mRadius;
};

#endif