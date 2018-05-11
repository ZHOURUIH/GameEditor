#ifndef _TX_COMPONENT_COLLISION_PLANE_H_
#define _TX_COMPONENT_COLLISION_PLANE_H_

#include "txComponentCollision.h"

class txGeometryPlane;
class txComponentCollisionPlane : public txComponentCollision
{
public:
	txComponentCollisionPlane(const std::string& type, const std::string& name)
		:
		txComponentCollision(type, name),
		mPlaneGeometry(NULL),
		mWidthX(0.0f),
		mLengthZ(0.0f)
	{}
	virtual ~txComponentCollisionPlane(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		bool isCollision = txComponentCollision::isType(type);
		return isCollision || type == TOSTRING(txComponentCollisionPlane);
	}
	void destroy(){ mPlaneGeometry = NULL; }
	// X方向上宽度
	void setWidthX(const float& widthX);
	const float& getWidthX(){ return mWidthX; }
	// Z方向上长度
	void setLengthZ(const float& lengthZ);
	const float& getLengthZ(){ return mLengthZ; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent);
	virtual void afterCreateGeometry();
protected:
	txGeometryPlane* mPlaneGeometry;
	float mWidthX;
	float mLengthZ;
};

#endif