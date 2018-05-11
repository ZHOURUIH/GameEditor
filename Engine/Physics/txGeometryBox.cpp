#include "txGeometryBox.h"
#include "txPhysicsUtility.h"
#include "txCamera.h"
#include "txRigidBody.h"
#include "txRenderUtility.h"

txGeometryBox::txGeometryBox(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mX(0.0f),
mY(0.0f),
mZ(0.0f)
{
	;
}

void txGeometryBox::create(const float& x, const float& y, const float& z, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mX = x;
	mY = y;
	mZ = z;

	// 开始创建可叠加的形状
	startCreateGeometry(pos, rot);
	// 创建封装的盒子形状
	dGeomID tempGeom = dCreateBox(NULL, mX, mY, mZ);
	// 结束创建可叠加的形状
	endCreateGeometry(tempGeom, mass);
}

void txGeometryBox::render(txCamera* camera)
{
	txRenderUtility::renderBox(camera, mX, mY, mZ, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometryBox::setMass(const float& mass)
{
	// 设置封装盒子的质量
	dMass boxMass;
	dMassSetBoxTotal(&boxMass, mass, mX, mY, mZ);
	// 变换质量
	transformMass(boxMass);
	// 刷新刚体质量
	mRigidBody->refreshMass();
}

void txGeometryBox::setWidth(const float& widthX)
{
	setSide(widthX, mY, mZ);
}

void txGeometryBox::setHeight(const float& heightY)
{
	setSide(mX, heightY, mZ);
}

void txGeometryBox::setLength(const float& lengthZ)
{
	setSide(mX, mY, lengthZ);
}

void txGeometryBox::setSide(const float& x, const float& y, const float& z)
{
	mX = x;
	mY = y;
	mZ = z;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	dGeomBoxSetLengths(geom, mX, mY, mZ);
	// 修改参数后,需要刷新质量
	setMass(mMass.mass);
}