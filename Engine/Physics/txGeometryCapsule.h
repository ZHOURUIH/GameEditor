#ifndef _TX_GEOMETRY_CAPSULE_H_
#define _TX_GEOMETRY_CAPSULE_H_

#include "txGeometry.h"

class txGeometryCapsule : public txGeometry
{
public:
	txGeometryCapsule(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometryCapsule(){}
	void create(const float& cylinderHeight, const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot);
	virtual void render(txCamera* camera);
	virtual void setMass(const float& mass);
	void setCylinderHeight(const float& cylinderHeight);
	void setRadius(const float& radius);
	void setCapsuleParams(const float& cylinderHeight, const float& radius);
	const float& getCylinderHeight() { return mCylinderHeight; }
	const float& getRadius() { return mRadius; }
protected:
	float mCylinderHeight;	// 中间圆柱体的高度
	float mRadius;	// 胶囊两端半球体的半径
};

#endif