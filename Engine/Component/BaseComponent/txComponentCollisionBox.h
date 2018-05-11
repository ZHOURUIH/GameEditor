#ifndef _TX_COMPONENT_COLLISION_BOX_H_
#define _TX_COMPONENT_COLLISION_BOX_H_

#include "txComponentCollision.h"

class txGeometryBox;
class txComponentCollisionBox : public txComponentCollision
{
public:
	txComponentCollisionBox(const std::string& type, const std::string& name)
		:
		txComponentCollision(type, name),
		mBoxGeometry(NULL),
		mWidthX(0.0f),
		mHeightY(0.0f),
		mLengthZ(0.0f)
	{}
	virtual ~txComponentCollisionBox(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		bool isCollision = txComponentCollision::isType(type);
		return isCollision || type == TOSTRING(txComponentCollisionBox);
	}
	void destroy(){ mBoxGeometry = NULL; }
	// x方向上的宽度
	void setWidth(const float& widthX);
	const float& getWidth() { return mWidthX; }
	// y方向上的高度
	void setHeight(const float& heightY);
	const float& getHeight() { return mHeightY; }
	// z方向上的长度
	void setLength(const float& lengthZ);
	const float& getLength() { return mLengthZ; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent);
	virtual void afterCreateGeometry();
protected:
	txGeometryBox* mBoxGeometry;
	float mWidthX;
	float mHeightY;
	float mLengthZ;
};

#endif