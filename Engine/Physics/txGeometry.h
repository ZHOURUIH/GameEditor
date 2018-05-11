#ifndef _TX_GEOMETRY_H_
#define _TX_GEOMETRY_H_

#include "ode.h"
#include "txEngineDefine.h"

class txCamera;
class txRigidBody;
class txGeometry
{
public:
	txGeometry(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type);
	virtual ~txGeometry(){ destroy(); }
	// 更新获取当前碰撞体的位置和旋转
	virtual void update();
	virtual void render(txCamera* camera) = 0;
	virtual void destroy();
	void setRelativePosition(const VECTOR3& pos);
	void setRelativeRotation(const MATRIX3& rot);
	void setEnable(const bool& enable);
	bool isEnable();
	virtual void setMass(const float& mass) = 0;

	// 设置成员变量
	void setColour(const VECTOR4& colour)	{ mColour = colour; }
	void setWireframe(const bool& wireframe){ mWireframe = wireframe; }
	void setVisible(const bool& visible)	{ mVisible = visible; }

	// 获得成员变量
	dGeomID getGeomID()						{ return mGeom; }
	const bool& getVisible()				{ return mVisible; }
	const VECTOR4& getColour()				{ return mColour; }
	const bool& getWireframe()				{ return mWireframe; }
	const GEOM_TYPE& getGeomType()			{ return mType; }
	const dMass& getMass()					{ return mMass; }
	const VECTOR3& getRelativePosition()	{ return mRelativePosition; }
	const MATRIX3& getRelativeRotation()	{ return mRelativeRotation; }
	const MATRIX4& getRelativeTransform()	{ return mRelativeTransform; }
protected:
	void refreshTransform();
	void startCreateGeometry(const VECTOR3& pos, const MATRIX3& rot);
	void endCreateGeometry(dGeomID geom, float mass);
	void transformMass(dMass& mass);
protected:
	txRigidBody* mRigidBody;
	dSpaceID mSpace;
	dGeomID mGeom;
	dMass mMass;
	bool mVisible;
	VECTOR4 mColour;
	bool mWireframe;
	GEOM_TYPE mType;
	VECTOR3 mRelativePosition;	// 相对于刚体的位置
	MATRIX3 mRelativeRotation;	// 相对于刚体的旋转
	MATRIX4 mRelativeTransform;	// 相对于刚体的变换
};

#endif