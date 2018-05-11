#ifndef _TX_COMPONENT_TRANSFORM_H_
#define _TX_COMPONENT_TRANSFORM_H_

#include "txComponent.h"

class txNode;
class txComponentTransform : public txComponent
{
public:
	txComponentTransform(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mPosition(txMath::VEC3_ZERO),
		mRotation(txMath::MAT3_IDENTITY),
		mScale(txMath::VEC3_ONE),
		mDirty(true)
	{}
	virtual ~txComponentTransform(){ destroy(); }
	virtual void init(txComponentOwner* owner);
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentTransform); }
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentTransform); }
	void destroy(){}
	virtual void setParentComponent(txComponent* component);

	const VECTOR3& getTrembeOffset(){ return mTrembleOffset; }
	const VECTOR3& getPosition(){ return mPosition; }
	const MATRIX3& getRotation(){ return mRotation; }
	const VECTOR3& getScale(){ return mScale; }

	void setTrembleOffset(const VECTOR3& offset, const bool& refreshNow = false);
	void setPosition(const VECTOR3& position, const bool& refreshNow = false);
	void setRotation(const MATRIX3& rotation, const bool& refreshNow = false);
	void setScale(const VECTOR3& scale, const bool& refreshNow = false);

	void applyToNode(const bool& refreshNow);

	void rotate(const MATRIX3& rotation, const bool& refreshNow = false)
	{
		setRotation(rotation * mRotation, refreshNow);
	}

	void yaw(const float& fYaw, const bool& refreshNow = false)
	{
		setRotation(txMath::getYawMatrix3(fYaw) * mRotation, refreshNow);
	}

	// 往上为正,往下为负
	void pitch(const float& fPitch, const bool& refreshNow = false)
	{
		setRotation(txMath::getPitchMatrix3(fPitch) * mRotation, refreshNow);
	}

	void roll(const float& fRoll, const bool& refreshNow = false)
	{
		setRotation(txMath::getRollMatrix3(fRoll) * mRotation, refreshNow);
	}

	void setDirty(const bool& dirty) { mDirty = dirty; }
	const bool& isDirty(){ return mDirty; }
protected:
	VECTOR3 mTrembleOffset;		// 位置震动偏移
	VECTOR3 mPosition;			// 当前位置
	MATRIX3 mRotation;			// 当前旋转
	VECTOR3 mScale;				// 当前缩放
	bool mDirty;				// 是否需要更新
};

#endif