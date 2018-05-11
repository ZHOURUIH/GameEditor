#ifndef _TX_COMPONENT_DYNAMIC_MESH_H_
#define _TX_COMPONENT_DYNAMIC_MESH_H_

#include "txComponentMesh.h"

class txObjectBoneHook;
class txComponentDynamicMesh : public txComponentMesh
{
public:
	txComponentDynamicMesh(const std::string& type, const std::string& name)
		:
		txComponentMesh(type, name),
		mStartFrame(-1),
		mEndFrame(-1)
	{}
	virtual ~txComponentDynamicMesh(){ destroy(); }
	virtual void initProperty();
	virtual bool initEntityByFileName(const std::string& filePath, const bool& hasAnim, const bool& loadTexture);
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type)
	{
		bool isMesh = txComponentMesh::isType(type);
		return isMesh || type == TOSTRING(txComponentDynamicMesh);
	}
	void destroy();

	txObjectBoneHook* addHook(const std::string& name, const std::string& boneName, const VECTOR3& hookPos = txMath::VEC3_ZERO,
		const MATRIX3& hookRot = txMath::MAT3_IDENTITY, const VECTOR3& hookScale = txMath::VEC3_ONE);
	txObjectBoneHook* getHook(const std::string& name)
	{
		auto iter = mHookList.find(name);
		if (iter != mHookList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	// 动作播放速度
	void setCurAnimSpeed(const float& speed);
	float getCurAnimSpeed();
	//播放动作
	void playAnimation(const std::string& anim, const bool& loop = false, const float& speed = 1.0f, const float& timeOffset = 0.0f,
		const std::string& hookAnim = EMPTY_STRING, const bool& hookAnimLoop = false, const float& hookAnimSpeed = 1.0f, const float& hookAnimTimeOffset = 0.0f);
	void stopCurAnimation();
	const std::string& getCurAnimName();
	float getCurAnimLength();
	void setPlayState(const PLAY_STATE& state);
	PLAY_STATE getPlayState();
	bool getAnimNameByFullAnimName(const std::string& sourceModelName, const std::string& fullAnimName, std::string& animName)
	{
		std::string modelName = mModelFileName.substr(0, mModelFileName.find_last_of('.'));
		// 如果不是此模型的动作,则返回false
		if (sourceModelName != modelName)
		{
			return false;
		}
		// 去除模型名
		animName = fullAnimName.substr(modelName.length() + 1, fullAnimName.length() - modelName.length() - 1);
		// 去除固定动作后缀
		animName = animName.substr(0, animName.find_last_of('_'));
		return true;
	}
	std::string getFullAnimName(const std::string& animName)
	{
		std::string modelName = mModelFileName.substr(0, mModelFileName.find_last_of('.'));
		return modelName + "_" + animName + "_animation";
	}
	void setStartFrame(const int& frame) { mStartFrame = frame; }
	const int& getStartFrame() { return mStartFrame; }
	void setEndFrame(const int& frame) { mEndFrame = frame; }
	const int& getEndFrame() { return mEndFrame; }
	virtual txAABox getWorldAABB(const MATRIX4& exTransform = txMath::MAT4_IDENTITY);
	virtual txAABox getLocalAABB();
protected:
	txMap<std::string, txObjectBoneHook*> mHookList; // 挂点列表
	int mStartFrame;
	int mEndFrame;

};

#endif