#ifndef _TX_GEOMETRY_CYLINDER_H_
#define _TX_GEOMETRY_CYLINDER_H_

#include "txGeometry.h"

class txGeometryCylinder : public txGeometry
{
public:
	txGeometryCylinder(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometryCylinder(){}
	void create(const float& height, const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot);
	virtual void render(txCamera* camera);
	virtual void setMass(const float& mass);
	void setHeight(const float& height);
	void setRadius(const float& radius);
	void setCylinderParams(const float& height, const float& radius);
	const float& getHeight() { return mHeight; }
	const float& getRadius() { return mRadius; }
protected:
	float mHeight;
	float mRadius;
};

#endif