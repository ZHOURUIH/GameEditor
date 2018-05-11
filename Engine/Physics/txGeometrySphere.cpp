#include "txGeometrySphere.h"
#include "txCamera.h"
#include "txPhysicsUtility.h"
#include "txRigidBody.h"
#include "txRenderUtility.h"

txGeometrySphere::txGeometrySphere(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mRadius(0.0f)
{
	;
}

void txGeometrySphere::create(const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mRadius = radius;

	// 开始创建可叠加的形状
	startCreateGeometry(pos, rot);
	// 创建被变换的球体形状
	dGeomID tempGeom = dCreateSphere(NULL, mRadius);
	// 结束创建可叠加的形状
	endCreateGeometry(tempGeom, mass);
}

void txGeometrySphere::render(txCamera* camera)
{
	txRenderUtility::renderSphere(camera, mRadius, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometrySphere::setMass(const float& mass)
{
	dMass sphereMass;
	dMassSetSphereTotal(&sphereMass, mass, mRadius);
	// 变换质量
	transformMass(sphereMass);
	// 刷新刚体质量
	mRigidBody->refreshMass();
}

void txGeometrySphere::setRadius(const float& radius)
{
	mRadius = radius;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	dGeomSphereSetRadius(geom, mRadius);
	// 修改参数后,需要刷新质量
	setMass(mMass.mass);
}