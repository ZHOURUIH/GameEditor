#ifndef _TX_GEOMETRY_PLANE_H_
#define _TX_GEOMETRY_PLANE_H_

#include "txGeometry.h"

class txGeometryPlane : public txGeometry
{
public:
	txGeometryPlane(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometryPlane(){}
	void create(const float& x, const float& z, const float& mass, const VECTOR3& pos, const MATRIX3& rot);
	virtual void render(txCamera* camera);
	virtual void setMass(const float& mass);
	void setWidthX(const float& x);
	void setLengthZ(const float& z);
	void setSide(const float& x, const float& z);
	const float& getWidthX() { return mX; }
	const float& getLengthZ() { return mZ; }
protected:
	dGeomID createPlane();
	void fillPlaneVertices(const float& x, const float& z);
protected:
	float* mVertices;
	int mVertexCount;
	dTriIndex* mIndices;
	int mIndexCount;
	float mX;
	float mZ;
};

#endif