#ifndef _TX_COMPONENT_PHYSICS_H_
#define _TX_COMPONENT_PHYSICS_H_

#include "txComponent.h"

class txRigidBody;
class txComponentPhysics : public txComponent
{
public:
	txComponentPhysics(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mRigidBody(NULL)
	{}
	virtual ~txComponentPhysics(){ destroy(); }
	virtual void init(txComponentOwner* owner);
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentPhysics); }
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentPhysics); }
	void destroy();
	void setBodyPosition(const VECTOR3& pos);
	void setBodyRotation(const MATRIX3& rot);
	void setLinearVelocity(const VECTOR3& velocity);
	const VECTOR3& getLinearVelocity();
	void setAngularVelocity(const VECTOR3& velocity);
	const VECTOR3& getAngularVelocity();
	VECTOR3 getAcceleration();
	void setInstantForce(const VECTOR3& force);
	VECTOR3 getInstantForce();
	void setConstantForce(const VECTOR3& force);
	const VECTOR3& getConstantForce();
	void setInstantTorque(const VECTOR3& torque);
	VECTOR3 getInstantTorque();
	void setConstantTorque(const VECTOR3& torque);
	const VECTOR3& getConstantTorque();
	void setShowAllGeometry(const bool& show);
	bool getShowAllGeometry();
	void setKinematic(const bool& kinematic);
	bool getKinematic();
	txRigidBody* getRigidBody() { return mRigidBody; }
protected:
	txRigidBody* mRigidBody;
};

#endif