#ifndef _TX_COMPONENT_COLLISION_H_
#define _TX_COMPONENT_COLLISION_H_

#include "txComponent.h"

class txGeometry;
class txComponentPhysics;
class txComponentCollision : public txComponent
{
public:
	txComponentCollision(const std::string& type, const std::string& name)
	:
	txComponent(type, name),
	mGeometry(NULL),
	mMass(0.0f),
	mVisible(true),
	mWireframe(true)
	{}
	virtual ~txComponentCollision(){ destroy(); }
	virtual void init(txComponentOwner* owner);
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentCollision); }
	virtual void setParentComponent(txComponent* component);
	virtual void setActive(const bool& active);
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentCollision); }
	void destroy();
	
	void setMass(const float& mass);
	void setColour(const VECTOR4& colour);
	void setWireframe(const bool& wireframe);
	void setShowGeometry(const bool& show);
	void setRelativePosition(const VECTOR3& pos);
	void setRelativeRotation(const MATRIX3& rot);

	const float& getMass()					{ return mMass; }
	const VECTOR3& getRelativePosition()	{ return mPosition; }
	const MATRIX3& getRelativeRotation()	{ return mRotation; }
	const bool& getShowGeometry()			{ return mVisible; }
	const bool& getWireframe()				{ return mWireframe; }
	const VECTOR4& getColour()				{ return mColour; }
protected:
	virtual txGeometry* createGeometry(txComponentPhysics* physicalComponent) = 0;
	virtual void afterCreateGeometry();
protected:
	txGeometry* mGeometry;
	float mMass;			// 之所以加上这些已经在Geometry中存在的变量,是为了当无法创建Geometry时仍然可以正常设置和获得属性
	VECTOR3 mPosition;
	MATRIX3 mRotation;
	bool mVisible;
	bool mWireframe;
	VECTOR4 mColour;
};

#endif