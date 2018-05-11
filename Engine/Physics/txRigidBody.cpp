#include "txRigidBody.h"
#include "txCamera.h"
#include "txODEPhysics.h"
#include "txGeometryBox.h"
#include "txGeometryPlane.h"
#include "txGeometrySphere.h"
#include "txGeometryCapsule.h"
#include "txGeometryCylinder.h"
#include "txPhysicsUtility.h"
#include "txEngineRoot.h"

txRigidBody::txRigidBody(const bool& showCollideShape)
:
mODEBody(NULL),
mShowCollideShape(showCollideShape)
{}

txRigidBody::~txRigidBody()
{
	destroy();
}

void txRigidBody::init()
{
	mODEBody = dBodyCreate(mODEPhysics->getPhysicsWorld());
}

void txRigidBody::destroy()
{
	// 先销毁刚体
	if (mODEBody != NULL)
	{
		dBodyDestroy(mODEBody);
		mODEBody = NULL;
	}

	// 然后销毁刚体上附加的所有的形状
	auto iter = mGeometryList.begin();
	auto iterEnd = mGeometryList.end();
	FOR_STL(mGeometryList, ; iter != iterEnd; ++iter)
	{
		txGeometry* geometry = *iter;
		TRACE_DELETE(geometry);
	}
	END_FOR_STL(mGeometryList);
	mGeometryList.clear();
}

void txRigidBody::update()
{
	auto iter = mGeometryList.begin();
	auto iterEnd = mGeometryList.end();
	FOR_STL(mGeometryList, ; iter != iterEnd; ++iter)
	{
		(*iter)->update();
	}
	END_FOR_STL(mGeometryList);
	// 设置物体的恒定外力和恒定扭矩,只是在原有
	dBodyAddForce(mODEBody, mConstantForce.x, mConstantForce.y, mConstantForce.z);
	dBodyAddTorque(mODEBody, mConstantTorque.x, mConstantTorque.y, mConstantTorque.z);
	// 从物理引擎中获得当前的位置的旋转
	mPosition = txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dBodyGetPosition(mODEBody));
	mRotation = txPhysicsUtility::odeMatrix3ToGLMMatrix3((dReal*)dBodyGetRotation(mODEBody));
	mLinearVelocity = txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dBodyGetLinearVel(mODEBody));
	mAngularVelocity = txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dBodyGetAngularVel(mODEBody));
	refreshTransform(); 
}

void txRigidBody::render(txCamera* camera)
{
	auto iter = mGeometryList.begin();
	auto iterEnd = mGeometryList.end();
	FOR_STL(mGeometryList, ; iter != iterEnd; ++iter)
	{
		if ((*iter)->getVisible())
		{
			(*iter)->render(camera);
		}
	}
	END_FOR_STL(mGeometryList);
}

void txRigidBody::refreshTransform()
{
	mWorldTransform = txMath::translate(txMath::MAT4_IDENTITY, mPosition) * MATRIX4(mRotation);
}

void txRigidBody::setBodyPosition(const VECTOR3& pos)
{
	dBodySetPosition(mODEBody, pos.x, pos.y, pos.z);
	mPosition = pos;
	refreshTransform();
}

void txRigidBody::setBodyRotation(const VECTOR3& eulerAngle)
{
	dMatrix3 dRot;
	txPhysicsUtility::eulerAngleToODEMatrix3(eulerAngle, dRot);
	dBodySetRotation(mODEBody, dRot);
	mRotation = txMath::eulerAngleToMatrix3(eulerAngle);
	refreshTransform();
}

void txRigidBody::setBodyRotation(const QUATERNION& quaternion)
{
	dQuaternion q;
	txPhysicsUtility::glmQuatToODEQuat(quaternion, q);
	dBodySetQuaternion(mODEBody, q);
	mRotation = txMath::quatToMatrix3(quaternion);
	refreshTransform();
}

void txRigidBody::setBodyRotation(const MATRIX3& rotMat)
{
	dMatrix3 dRot;
	txPhysicsUtility::glmMatrix3ToODEMatrix3(rotMat, dRot);
	dBodySetRotation(mODEBody, dRot);
	mRotation = rotMat;
	refreshTransform();
}

void txRigidBody::refreshMass()
{
	// 如果当前刚体已经销毁了,则不刷新,在销毁刚体时会出现这种情况
	if (mODEBody == NULL)
	{
		return;
	}
	// 没有碰撞体时不更新质量,避免报错
	if (mGeometryList.size() == 0)
	{
		return;
	}
	// 获得当前是否为运动学状态
	bool kinematic = dBodyIsKinematic(mODEBody) != 0;
	dMass bodymass;
	dMassSetZero(&bodymass);
	auto iter = mGeometryList.begin();
	auto iterEnd = mGeometryList.end();
	FOR_STL(mGeometryList, ; iter != iterEnd; ++iter)
	{
		dGeomID geom = (*iter)->getGeomID();
		if (dGeomGetClass(geom) == dGeomTransformClass)
		{
			dGeomID transformedGeom = dGeomTransformGetGeom(geom);
			// 合并形状的质量到刚体质量中
			dMassAdd(&bodymass, &((*iter)->getMass()));

			// 调整该形状质心位置,使之与整体的质心对齐
			const VECTOR3& gemoPosition = (*iter)->getRelativePosition();
			dGeomSetPosition(transformedGeom, gemoPosition.x - bodymass.c[0], gemoPosition.y - bodymass.c[1], gemoPosition.z - bodymass.c[2]);
			dMassTranslate(&bodymass, -bodymass.c[0], -bodymass.c[1], -bodymass.c[2]);

			// 将形状设置到刚体中,
			dGeomSetBody(geom, mODEBody);
		}
	}
	END_FOR_STL(mGeometryList);
	// 设置最终的质量到刚体中
	dBodySetMass(mODEBody, &bodymass);
	// 重新确认运动学状态
	if (kinematic)
	{
		dBodySetKinematic(mODEBody);
	}
	else
	{
		dBodySetDynamic(mODEBody);
	}
}

txGeometryBox* txRigidBody::addBox(const float& mass, const VECTOR3& pos, const float& x, const float& y, const float& z, const VECTOR3& eulerAngle, const bool& showGeom)
{
	txGeometryBox* boxGeom = TRACE_NEW(txGeometryBox, boxGeom, this, mODEPhysics->getPhysicsSpace(), GT_BOX);
	// 先加入列表,然后在设置属性
	mGeometryList.insert(boxGeom);
	boxGeom->create(x, y, z, mass, pos, txMath::eulerAngleToMatrix3(eulerAngle));
	boxGeom->setVisible(showGeom);
	return boxGeom;
}

txGeometrySphere* txRigidBody::addSphere(const float& mass, const VECTOR3& pos, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom)
{
	txGeometrySphere* sphereGeom = TRACE_NEW(txGeometrySphere, sphereGeom, this, mODEPhysics->getPhysicsSpace(), GT_SPHERE);
	// 先加入列表,然后在设置属性
	mGeometryList.insert(sphereGeom);
	sphereGeom->create(radius, mass, pos, txMath::eulerAngleToMatrix3(eulerAngle));
	sphereGeom->setVisible(showGeom);
	return sphereGeom;
}

txGeometryCapsule* txRigidBody::addCapsule(const float& mass, const VECTOR3& pos, const float& height, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom)
{
	txGeometryCapsule* capsuleGeom = TRACE_NEW(txGeometryCapsule, capsuleGeom, this, mODEPhysics->getPhysicsSpace(), GT_CAPSULE);
	// 先加入列表,然后在设置属性
	mGeometryList.insert(capsuleGeom);
	capsuleGeom->create(height, radius, mass, pos, txMath::eulerAngleToMatrix3(eulerAngle));
	capsuleGeom->setVisible(showGeom);
	return capsuleGeom;
}

txGeometryCylinder* txRigidBody::addCylinder(const float& mass, const VECTOR3& pos, const float& height, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom)
{
	txGeometryCylinder* cylinderGeom = TRACE_NEW(txGeometryCylinder, cylinderGeom, this, mODEPhysics->getPhysicsSpace(), GT_CYLINDER);
	// 先加入列表,然后在设置属性
	mGeometryList.insert(cylinderGeom);
	cylinderGeom->create(height, radius, mass, pos, txMath::eulerAngleToMatrix3(eulerAngle));
	cylinderGeom->setVisible(showGeom);
	return cylinderGeom;
}

txGeometryPlane* txRigidBody::addPlane(const float& mass, const VECTOR3& pos, const float& x, const float& z, const VECTOR3& eulerAngle, const bool& showGeom)
{
	txGeometryPlane* planeGeom = TRACE_NEW(txGeometryPlane, planeGeom, this, mODEPhysics->getPhysicsSpace(), GT_PLANE);
	// 先加入列表,然后在设置属性
	mGeometryList.insert(planeGeom);
	planeGeom->create(x, z, mass, pos, txMath::eulerAngleToMatrix3(eulerAngle));
	planeGeom->setVisible(showGeom);
	return planeGeom;
}

void txRigidBody::deleteGeometry(txGeometry* geom)
{
	auto iter = mGeometryList.find(geom);
	if (iter != mGeometryList.end())
	{
		txGeometry* geometry = *iter;
		// 先从列表中删除
		mGeometryList.erase(iter);
		// 再销毁碰撞体
		TRACE_DELETE(geometry);
	}
}

void txRigidBody::setKinematic(const bool& kinematic)
{
	if (kinematic)
	{
		dBodySetKinematic(mODEBody);
	}
	else
	{
		dBodySetDynamic(mODEBody);
	}
}

bool txRigidBody::getKinematic()
{
	return dBodyIsKinematic(mODEBody) != 0;
}

void txRigidBody::setEnable(const bool& enable)
{
	if (enable)
	{
		dBodyEnable(mODEBody);
	}
	else
	{
		dBodyDisable(mODEBody);
	}
}

bool txRigidBody::getEnable()
{
	return dBodyIsEnabled(mODEBody) != 0;
}

void txRigidBody::setGravityMode(const bool& gravity)
{
	dBodySetGravityMode(mODEBody, gravity ? 1 : 0);
}

bool txRigidBody::getGravityMode()
{
	return dBodyGetGravityMode(mODEBody) != 0;
}

VECTOR3 txRigidBody::getForce()
{
	return txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dBodyGetForce(mODEBody));
}

void txRigidBody::setForce(const VECTOR3& force)
{
	dBodySetForce(mODEBody, force.x, force.y, force.z);
}

void txRigidBody::addForce(const VECTOR3& force)
{
	dBodyAddForce(mODEBody, force.x, force.y, force.z);
}

VECTOR3 txRigidBody::getTorque()
{
	return txPhysicsUtility::odeVector3ToGLMVector3((dReal*)dBodyGetTorque(mODEBody));
}

void txRigidBody::setTorque(const VECTOR3& torque)
{
	dBodySetTorque(mODEBody, torque.x, torque.y, torque.z);
}

void txRigidBody::addTorque(const VECTOR3& torque)
{
	dBodyAddTorque(mODEBody, torque.x, torque.y, torque.z);
}

void txRigidBody::setLinearVelocity(const VECTOR3& linearVelocity)
{
	dBodySetLinearVel(mODEBody, linearVelocity.x, linearVelocity.y, linearVelocity.z);
}

void txRigidBody::setAngularVelocity(const VECTOR3& angularVelocity)
{
	dBodySetAngularVel(mODEBody, angularVelocity.x, angularVelocity.y, angularVelocity.z);
}

void txRigidBody::nearCallback(void* data, dGeomID geom1, dGeomID geom2)
{
	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(geom1);
	dBodyID b2 = dGeomGetBody(geom2);
	if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact))
	{
		return;
	}

	dContact contact[MAX_CONTACT_COUNT];   // up to MAX_CONTACTS contacts per box-box
	for (int i = 0; i < MAX_CONTACT_COUNT; ++i)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.1f;
		contact[i].surface.bounce_vel = 0.1f;
		contact[i].surface.soft_cfm = 0.01f;
	}
	if (int numc = dCollide(geom1, geom2, MAX_CONTACT_COUNT, &contact[0].geom, sizeof(dContact)))
	{
		for (int i = 0; i < numc; ++i)
		{
			dJointID c = dJointCreateContact(mODEPhysics->getPhysicsWorld(), mODEPhysics->getContactGroup(), contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}