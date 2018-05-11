#ifndef _TX_MOVABLE_OBJECT_H_
#define _TX_MOVABLE_OBJECT_H_

#include "txComponentOwner.h"

class txComponentTransform;
class txComponentPhysics;
class txNode;
class txMovableObject : public txComponentOwner
{
public:
	txMovableObject(const std::string& name);
	virtual ~txMovableObject(){}
	virtual void init() { initComponents(); }
	virtual void update(float elapsedTime){ txComponentOwner::updateComponents(elapsedTime); }
	virtual void setPosition(const VECTOR3& position, const bool& refreshNow = false);
	virtual void setRotation(const MATRIX3& rotation, const bool& refreshNow = false);
	virtual void setScale(const VECTOR3& scale, const bool& refreshNow = false);
	// 截获给组件设置属性的调用
	virtual void setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue);
	const VECTOR3& getPosition();
	const MATRIX3& getRotation();
	const VECTOR3& getScale();
	void rotate(const MATRIX3& rotation, const bool& refreshNow = false);
	void yaw(const float& angle, const bool& refreshNow = false);
	void pitch(const float& angle, const bool& refreshNow = false);
	void roll(const float& angle, const bool& refreshNow = false);
	txNode* getNode();
	void setLinearVelocity(const VECTOR3& velocity);
	const VECTOR3& getLinearVelocity();
	// 因为在摄像机中需要重写setPosition等虚函数来实现功能,所以不提供直接通过变换组件来设置位置等属性
	//txComponentTransform* getTransformComponent() { return mTransformComponent; }
	txComponentPhysics* getPhysicalComponent() { return mPhysicalComponent; }
	const VECTOR3& getSpeed() { return mSpeed; }
	void setSpeed(const VECTOR3& speed) { mSpeed = speed; }
protected:
	txComponentTransform* mTransformComponent;
	txComponentPhysics* mPhysicalComponent;
	VECTOR3 mSpeed;
};

#endif