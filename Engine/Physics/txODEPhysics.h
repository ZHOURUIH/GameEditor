#ifndef _TX_ODE_PHYSICS_H_
#define _TX_ODE_PHYSICS_H_

#include "ode.h"
#include "txEngineDefine.h"

class txCamera;
class txRigidBody;
class txODEPhysics
{
public:
	txODEPhysics();
	virtual ~txODEPhysics();
	virtual void init();
	virtual void update(float elapsedTime);
	virtual void render(txCamera* camera);
	virtual void destroy();
	const dWorldID& getPhysicsWorld() { return mPhysicsWorld; }
	const dSpaceID& getPhysicsSpace() { return mPhysicsSpace; }
	const dJointGroupID& getContactGroup() { return mContactGroup; }
	// 创建一个空的刚体,没有质量,没有形状
	txRigidBody* createBody(const bool& showCollideShape = true);
	// 销毁一个刚体,会连同刚体中的所有形状也一起销毁
	void destroyBody(txRigidBody* body);
protected:
	dWorldID mPhysicsWorld;									// 物理世界
	dSpaceID mPhysicsSpace;									// 物理空间
	dJointGroupID mContactGroup;							// 接触点的集合
	dThreadingImplementationID mThreadingImplementation;	// 线程实现
	dThreadingThreadPoolID mThreadingPool;					// 线程池
	txSet<txRigidBody*> mBodyList;
};

#endif