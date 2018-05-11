#ifndef _TX_RIGID_BODY_H_
#define _TX_RIGID_BODY_H_

#include "ode.h"
#include "txEngineDefine.h"
#include "txEngineBase.h"

// 刚体,是对ODE中body的封装
const int MAX_CONTACT_COUNT = 40;

class txGeometry;
class txGeometryBox;
class txGeometrySphere;
class txGeometryCapsule;
class txGeometryCylinder;
class txGeometryPlane;
class txCamera;
class txODEPhysics;
class txRigidBody : public txEngineBase
{
public:
	txRigidBody(const bool& showCollideShape = true);
	virtual ~txRigidBody();
	virtual void init();
	virtual void destroy();
	// 更新获取刚体当前的位置
	virtual void update();
	virtual void render(txCamera* camera);
	// 设置刚体的位置
	virtual void setBodyPosition(const VECTOR3& pos);
	// 设置刚体的旋转
	virtual void setBodyRotation(const VECTOR3& eulerAngle);
	virtual void setBodyRotation(const QUATERNION& quaternion);
	virtual void setBodyRotation(const MATRIX3& rotMat);
	// 根据刚体中的所有形状,刷新质量
	void refreshMass();
	// 为刚体添加盒子,mass是该盒子的质量,必须大于0
	// pos是在body的坐标系中的位置,也是盒子中心所处的位置
	// x,y,z分别是盒子在三个方向上的长度
	// eulerAngle是欧拉角的三个分量,航向角,俯仰角,滚动角
	txGeometryBox* addBox(const float& mass, const VECTOR3& pos, const float& x, const float& y, const float& z, const VECTOR3& eulerAngle, const bool& showGeom = true);
	// 为刚体添加球体,mass是该球体的质量,必须大于0
	// pos是在body的坐标系中的位置,也是球体球心所处的位置
	// radius是球体的半径
	txGeometrySphere* addSphere(const float& mass, const VECTOR3& pos, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom = true);
	// 为刚体添加胶囊,mass是该胶囊的质量,必须大于0
	// pos是在body的坐标系中的位置,也是胶囊中心所处的位置,height是中间圆柱体的高度,radius是两端半球体的半径
	// eulerAngle是欧拉角的三个分量,航向角,俯仰角,滚动角
	txGeometryCapsule* addCapsule(const float& mass, const VECTOR3& pos, const float& height, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom = true);
	// 为刚体添加圆柱体,mass是该圆柱体的质量,必须大于0
	// pos是在body的坐标系中的位置,也是圆柱体中心所处的位置,height是圆柱体的高, radius是底面半径
	txGeometryCylinder* addCylinder(const float& mass, const VECTOR3& pos, const float& height, const float& radius, const VECTOR3& eulerAngle, const bool& showGeom = true);
	// 为刚体添加平面,mass是该平面的质量,必须大于0
	// pos是在body的坐标系中的位置,也是平面中心所处的位置,x是x方向上的长度,z是z方向上的长度
	txGeometryPlane* addPlane(const float& mass, const VECTOR3& pos, const float& x, const float& z, const VECTOR3& eulerAngle, const bool& showGeom = true);
	void deleteGeometry(txGeometry* geom);
	void setShowCollideShape(const bool& show) { mShowCollideShape = show; }
	const bool& getShowCollideShape() { return mShowCollideShape; }
	const MATRIX4& getWorldTransform() { return mWorldTransform; }
	const VECTOR3& getPosition() { return mPosition; }
	const MATRIX3& getRotation() { return mRotation; }
	const dBodyID& getBodyID() { return mODEBody; }
	void setKinematic(const bool& kinematic);
	bool getKinematic();
	void setEnable(const bool& enable);
	bool getEnable();
	void setGravityMode(const bool& gravity);
	bool getGravityMode();
	VECTOR3 getForce();
	void setForce(const VECTOR3& force);
	void addForce(const VECTOR3& force);
	VECTOR3 getTorque();
	void setTorque(const VECTOR3& torque);
	void addTorque(const VECTOR3& torque);
	void setLinearVelocity(const VECTOR3& linearVelocity);
	const VECTOR3& getLinearVelocity() {return mLinearVelocity;}
	void setAngularVelocity(const VECTOR3& angularVelocity);
	const VECTOR3& getAngularVelocity() {return mAngularVelocity;}
	void setConstantForce(const VECTOR3& force) { mConstantForce = force; }
	const VECTOR3& getConstantForce() { return mConstantForce; }
	void setConstantTorque(const VECTOR3& torque) { mConstantTorque = torque; }
	const VECTOR3& getConstantTorque() { return mConstantTorque; }
	// 碰撞检测回调函数
	static void nearCallback(void* data, dGeomID geom1, dGeomID geom2);
protected:
	void refreshTransform();
protected:
	dBodyID mODEBody;
	txSet<txGeometry*> mGeometryList;
	bool mShowCollideShape;					// 是否渲染该刚体中的所有形状
	VECTOR3 mPosition;					// 世界空间中的坐标
	MATRIX3 mRotation;					// 世界空间中的旋转
	MATRIX4 mWorldTransform;				// 世界空间中的变换
	VECTOR3 mLinearVelocity;				// 物体的线速度
	VECTOR3 mAngularVelocity;				// 物体的角速度
	VECTOR3 mConstantForce;				// 物体的恒定外力
	VECTOR3 mConstantTorque;				// 物体的恒定扭矩
};

#endif