#include "txGeometry.h"
#include "txPhysicsUtility.h"
#include "txRigidBody.h"

txGeometry::txGeometry(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
mSpace(space),
mRigidBody(body),
mType(type),
mGeom(NULL),
mVisible(true),
mWireframe(true),
mColour(0.7f, 0.3f, 0.9f, 1.0f),
mRelativeRotation(txMath::MAT3_IDENTITY),
mRelativeTransform(txMath::MAT4_IDENTITY)
{}

void txGeometry::destroy()
{
	if (mGeom != NULL)
	{
		// 如果当前有挂在一个刚体上,则先从刚体中取下来
		if (dGeomGetBody(mGeom) != NULL)
		{
			dGeomSetBody(mGeom, NULL);
		}
		dGeomDestroy(mGeom);
		mGeom = NULL;
		// 销毁形状后需要通知刚体刷新质量
		if (mRigidBody != NULL)
		{
			mRigidBody->refreshMass();
		}
	}
}

void txGeometry::update()
{
	if (mGeom == NULL || dGeomGetClass(mGeom) != dGeomTransformClass)
	{
		return;
	}

	dGeomID g2 = dGeomTransformGetGeom(mGeom);
	mRelativePosition = txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dGeomGetPosition(g2));
	mRelativeRotation = txPhysicsUtility::odeMatrix3ToGLMMatrix3((dReal*)dGeomGetRotation(g2));
	refreshTransform();
}

void txGeometry::refreshTransform()
{
	mRelativeTransform = txMath::translate(txMath::MAT4_IDENTITY, mRelativePosition) * MATRIX4(mRelativeRotation);
}

void txGeometry::startCreateGeometry(const VECTOR3& pos, const MATRIX3& rot)
{
	mRelativePosition = pos;
	mRelativeRotation = rot;
	// 刷新自身的相对变换矩阵
	refreshTransform();
	// 创建一个变换形状
	mGeom = dCreateGeomTransform(mSpace);
	dGeomTransformSetCleanup(mGeom, 1);
}

void txGeometry::endCreateGeometry(dGeomID geom, float mass)
{
	// 将被变换的形状设置到变换形状中
	dGeomTransformSetGeom(mGeom, geom);

	// 设置形状相对于空形状的位置和旋转,并且一起调整质量分布
	dGeomSetPosition(geom, mRelativePosition.x, mRelativePosition.y, mRelativePosition.z);
	dMatrix3 Rtx;
	txPhysicsUtility::glmMatrix3ToODEMatrix3(mRelativeRotation, Rtx);
	dGeomSetRotation(geom, Rtx);

	// 设置质量
	setMass(mass);
}

void txGeometry::transformMass(dMass& mass)
{
	// 对质量进行变换
	dMassTranslate(&mass, mRelativePosition.x, mRelativePosition.y, mRelativePosition.z);
	dMatrix3 Rtx;
	txPhysicsUtility::glmMatrix3ToODEMatrix3(mRelativeRotation, Rtx);
	dMassRotate(&mass, Rtx);
	// 保存形状的质量
	mMass = mass;
}

void txGeometry::setRelativePosition(const VECTOR3& pos)
{
	// 设置相对位置
	mRelativePosition = pos;

	// 设置形状相对于空形状的位置
	dGeomID transformedGeom = dGeomTransformGetGeom(mGeom);
	dGeomSetPosition(transformedGeom, mRelativePosition.x, mRelativePosition.y, mRelativePosition.z);

	// 刷新质量分布
	setMass(mMass.mass);
}

void txGeometry::setRelativeRotation(const MATRIX3& rot)
{
	// 设置相对旋转
	mRelativeRotation = rot;

	// 设置形状相对于空形状的旋转
	dGeomID transformedGeom = dGeomTransformGetGeom(mGeom);
	dMatrix3 Rtx;
	txPhysicsUtility::glmMatrix3ToODEMatrix3(mRelativeRotation, Rtx);
	dGeomSetRotation(transformedGeom, Rtx);

	// 刷新质量分布
	setMass(mMass.mass);
}

void txGeometry::setEnable(const bool& enable)
{
	if (mGeom != NULL)
	{
		enable ? dGeomEnable(mGeom) : dGeomDisable(mGeom);
	}
}

bool txGeometry::isEnable()
{
	if (mGeom != NULL)
	{
		return dGeomIsEnabled(mGeom) != 0;
	}
	return false;
}