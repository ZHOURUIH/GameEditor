#ifndef _TX_GEOMETRY_SPHERE_H_
#define _TX_GEOMETRY_SPHERE_H_

#include "txGeometry.h"

class txGeometrySphere : public txGeometry
{
public:
	txGeometrySphere(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometrySphere(){}
	void create(const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot);
	virtual void render(txCamera* camera);
	virtual void setMass(const float& mass);
	void setRadius(const float& radius);
	const float& getRadius() { return mRadius; }
protected:
	float mRadius;
};

#endif