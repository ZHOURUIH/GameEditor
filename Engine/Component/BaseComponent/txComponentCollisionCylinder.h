#ifndef _TX_COMPONENT_COLLISION_CYLINDER_H_
#define _TX_COMPONENT_COLLISION_CYLINDER_H_

#include "txComponentCollision.h"

class txGeometryCylinder;
class txComponentCollisionCylinder : public txComponentCollision
{
public:
	txComponentCollisionCylinder(const std::string& type, const std::string& name)
		:
		txComponentCollision(type, name),
		mCylinderGeometry(NULL),
		mCylinderHeight(0.0f),
		mRadius(0.0f)
	{}
	virtual ~txComponentCollisionCylinder(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		bool isCollision = txComponentCollision::isType(type);
		return isCollision || type == TOSTRING(txComponentCollisionCylinder);
	}
	void destroy(){ mCylinderGeometry = NULL; }
	// 圆柱体的高度
	void setCylinderHeight(const float& cylinderHeight);
	const float& getCylinderHeight(){ return mCylinderHeight; }
	// 底面半径
	void setRadius(const float& radius);
	const float& getRadius() { return mRadius; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent);
	virtual void afterCreateGeometry();
protected:
	txGeometryCylinder* mCylinderGeometry;
	float mCylinderHeight;
	float mRadius;
};

#endif