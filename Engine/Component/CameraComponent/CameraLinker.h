#ifndef _CAMERA_LINKER_H_
#define _CAMERA_LINKER_H_

#include "txComponent.h"

// 摄像机连接器
class txMovableObject;
class CameraLinkerSwitch;
class CameraLinker : public txComponent
{
public:
	CameraLinker(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mCurSwitch(NULL),
		mLookatOffset(0.0f, 2.0f, 0.0f),
		mLookatTarget(false),
		mLinkObject(NULL),
		mUseTargetRotation(false)
	{}
	virtual ~CameraLinker(){ destroy(); }
	virtual void init(txComponentOwner* owner)
	{
		txComponent::init(owner);
		initSwitch();
	}
	virtual void setBaseType(){ mBaseType = TOSTRING(CameraLinker); }
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type) { return type == TOSTRING(CameraLinker); }
	void destroy(){destroySwitch();}
	// 补充更新逻辑,是在update之后调用
	virtual void lateUpdate(float elapsedTime);
	virtual void setRelativePosition(const VECTOR3& pos, const CAMERA_LINKER_SWITCH& switchType = CLS_NONE, const bool& useDefaultSwitchSpeed = true, const float& switchSpeed = 1.0f);
	virtual void notifyFinishSwitching(CameraLinkerSwitch* fixedSwitch){ mCurSwitch = NULL; } // 由转换器调用,通知连接器转换已经完成
	void setLookatOffset(const VECTOR3& offset) { mLookatOffset = offset; }
	void setLookatTarget(const bool& lookat) { mLookatTarget = lookat; }
	void setUseTargetRotation(const bool& use) { mUseTargetRotation = use; }
	void setLinkObject(txMovableObject* object){ mLinkObject = object; }
	const VECTOR3& getLookatOffset(){ return mLookatOffset; }
	const bool& getLookatTarget() { return mLookatTarget; }
	const bool& getUseTargetRotation() { return mUseTargetRotation; }
	txMovableObject* getLinkObject(){ return mLinkObject; }
	const VECTOR3& getRelativePosition() { return mRelativePosition; }
	CameraLinkerSwitch* getSwitch(const CAMERA_LINKER_SWITCH& type);
protected:
	void initSwitch();
	void destroySwitch();
protected:
	txMovableObject* mLinkObject;
	VECTOR3 mRelativePosition; //相对位置
	bool mLookatTarget; // 是否在摄像机运动过程中一直看向目标位置
	bool mUseTargetRotation; // 是否使用目标物体的旋转来旋转摄像机,旋转不会影响到摄像机的位置
	VECTOR3 mLookatOffset;	// 焦点的偏移,实际摄像机的焦点是物体的位置加上偏移
	txMap<CAMERA_LINKER_SWITCH, CameraLinkerSwitch*> mSwitchList; // 转换器列表
	CameraLinkerSwitch* mCurSwitch;
};

#endif