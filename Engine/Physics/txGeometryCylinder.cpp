#include "txGeometryCylinder.h"
#include "txCamera.h"
#include "txRigidBody.h"
#include "txPhysicsUtility.h"
#include "txRenderUtility.h"

txGeometryCylinder::txGeometryCylinder(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mHeight(0.0f),
mRadius(0.0f)
{
	;
}

void txGeometryCylinder::create(const float& height, const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mHeight = height;
	mRadius = radius;

	// 开始创建可叠加的形状
	startCreateGeometry(pos, rot);
	// 创建封装的圆柱体形状
	dGeomID tempGeom = dCreateCylinder(NULL, mRadius, mHeight);
	// 结束创建可叠加的形状
	endCreateGeometry(tempGeom, mass);
}

void txGeometryCylinder::render(txCamera* camera)
{
	txRenderUtility::renderCylinder(camera, mHeight, mRadius, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometryCylinder::setMass(const float& mass)
{
	dMass cylinderMass;
	dMassSetCylinderTotal(&cylinderMass, mass, 3, mRadius, mHeight);
	// 变换质量
	transformMass(cylinderMass);
	// 刷新刚体质量
	mRigidBody->refreshMass();
}

void txGeometryCylinder::setHeight(const float& height)
{
	setCylinderParams(height, mRadius);
}

void txGeometryCylinder::setRadius(const float& radius)
{
	setCylinderParams(mHeight, radius);
}

void txGeometryCylinder::setCylinderParams(const float& height, const float& radius)
{
	mHeight = height;
	mRadius = radius;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	dGeomCylinderSetParams(geom, mRadius, mHeight);
	// 修改参数后,需要刷新质量
	setMass(mMass.mass);
}