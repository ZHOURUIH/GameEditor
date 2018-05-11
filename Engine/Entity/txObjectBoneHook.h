#ifndef _TX_OBJECT_BONE_HOOK_H_
#define _TX_OBJECT_BONE_HOOK_H_

#include "txMovableObject.h"

class txEntity;
class txBoneHook;
class txObjectBoneHook : public txMovableObject
{
public:
	txObjectBoneHook(const std::string& name);
	virtual ~txObjectBoneHook(){ destroy(); }
	virtual void initComponents(){}
	virtual void update(float elapsedTime);
	void destroy();
	void createHook(const std::string& boneName, txEntity* entity);
	virtual void setPosition(const VECTOR3& position, const bool& refreshNow = false);
	virtual void setRotation(const MATRIX3& rotation, const bool& refreshNow = false);
	virtual void setScale(const VECTOR3& scale, const bool& refreshNow = false);
	// 获得世界变换,实际是挂点本地变换和骨头相对于骨骼的变换,也就是模型本地空间下的变换
	const MATRIX4& getWorldTransform();
	VECTOR3 getWorldPosition();
	MATRIX3 getWorldRotation();
	VECTOR3 getWorldScale();
	// 得到实际的世界变换,挂点本地变换和骨头相对于骨骼的变换和模型的世界变换,是世界坐标系下的变换
	MATRIX4 getRealWorldTransform();
	VECTOR3 getRealWorldPosition();
	MATRIX3 getRealWorldRotation();
	VECTOR3 getRealWorldScale();
	txEntity* getEntity() { return mHookedEntity; }
	txBoneHook* getHook() { return mHook; }
protected:
	std::string mHookedBoneName;
	txBoneHook* mHook;
	txEntity* mHookedEntity;
};

#endif