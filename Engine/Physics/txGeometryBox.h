#ifndef _TX_GEOMETRY_BOX_H_
#define _TX_GEOMETRY_BOX_H_

#include "txGeometry.h"

class txGeometryBox : public txGeometry
{
public:
	txGeometryBox(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometryBox(){}
	void create(const float& x, const float& y, const float& z, const float& mass, const VECTOR3& pos, const MATRIX3& rot);
	virtual void render(txCamera* camera);
	virtual void setMass(const float& mass);
	void setWidth(const float& widthX);
	void setHeight(const float& heightY);
	void setLength(const float& lengthZ);
	void setSide(const float& x, const float& y, const float& z);
	const float& getWidth() { return mX; }
	const float& getHeight() { return mY; }
	const float& getLength() { return mZ; }
protected:
	float mX;
	float mY;
	float mZ;
};

#endif