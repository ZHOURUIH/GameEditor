#include "txNode.h"
#include "txEntity.h"
#include "txAnimation.h"

#include "txComponentHeader.h"
#include "txObjectBoneHook.h"
#include "txEngineLog.h"
#include "txDynamicMeshProperty.h"

void txComponentDynamicMesh::destroy()
{
	auto iter = mHookList.begin();
	auto iterEnd = mHookList.end();
	FOR_STL(mHookList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mHookList);
	mHookList.clear();
}

void txComponentDynamicMesh::initProperty()
{
	txComponentMesh::initProperty();
	addProperty<txDynamicMeshPropertyDynamicMesh>();
	addProperty<txDynamicMeshPropertyAnimation>();
	addProperty<txDynamicMeshPropertyAnimationState>();
	addProperty<txDynamicMeshPropertyAnimationLength>();
	addProperty<txDynamicMeshPropertyAnimationSpeed>();
	addProperty<txDynamicMeshPropertyAnimationClip>();

	// 添加动态模型下拉列表
	txDynamicMeshPropertyDynamicMesh* dynamicMeshProperty = getProperty<txDynamicMeshPropertyDynamicMesh>();
	if (dynamicMeshProperty->getChoices() != NULL && dynamicMeshProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		txFileUtility::findFiles(P_MODEL_DYNAMIC_PATH, fileList, ".skinmesh");
		txVector<std::string> fileOnlyList;
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			fileOnlyList.push_back(txStringUtility::getFileName(fileList[i]));
		}
		END_FOR_STL(fileList);
		dynamicMeshProperty->setChoices(fileOnlyList);
	}
}

bool txComponentDynamicMesh::initEntityByFileName(const std::string& filePath, const bool& hasAnim, const bool& loadTexture)
{
	bool ret = txComponentMesh::initEntityByFileName(filePath, hasAnim, loadTexture);
	if (ret)
	{
		mEntity->GetParent()->SetAlwaysUpdate(true);
	}

	// 创建模型后添加动作下拉列表
	txDynamicMeshPropertyAnimation* dynamicMeshProperty = getProperty<txDynamicMeshPropertyAnimation>();
	if (dynamicMeshProperty->getChoices() != NULL)
	{
		txVector<std::string> animList;
		auto& animMap = mEntity->getAnimationSet();
		auto iterAnim = animMap.begin();
		auto iterAnimEnd = animMap.end();
		FOR_STL(animMap, ; iterAnim != iterAnimEnd; ++iterAnim)
		{
			animList.push_back(iterAnim->first);
		}
		END_FOR_STL(animMap);
		dynamicMeshProperty->setChoices(animList);
	}
	return ret;
}

void txComponentDynamicMesh::update(float elapsedTime)
{
	// 如果有挂点,则先更新骨骼,然后再更新挂点
	if (mHookList.size() > 0)
	{
		mEntity->refreshSkeleton();
		auto iter = mHookList.begin();
		auto iterEnd = mHookList.end();
		FOR_STL(mHookList, ; iter != iterEnd; ++iter)
		{
			iter->second->update(elapsedTime);
		}
		END_FOR_STL(mHookList);
	}
	txComponentMesh::update(elapsedTime);
}

txObjectBoneHook* txComponentDynamicMesh::addHook(const std::string& name, const std::string& boneName, const VECTOR3& hookPos, const MATRIX3& hookRot, const VECTOR3& hookScale)
{
	if (mEntity == NULL)
	{
		return NULL;
	}
	auto iter = mHookList.find(name);
	if (iter != mHookList.end())
	{
		ENGINE_ERROR("error : there is a game hook named : %s", name.c_str());
		return NULL;
	}
	txObjectBoneHook* hook = TRACE_NEW(txObjectBoneHook, hook, name);
	hook->createHook(boneName, mEntity);
	hook->setPosition(hookPos);
	hook->setRotation(hookRot);
	hook->setScale(hookScale);
	mHookList.insert(hook->getName(), hook);
	return hook;
}

void txComponentDynamicMesh::setCurAnimSpeed(const float& speed)
{
	if (mEntity != NULL)
	{
		mEntity->setAnimSpeed(speed);
	}
}

float txComponentDynamicMesh::getCurAnimSpeed()
{
	if (mEntity != NULL)
	{
		return mEntity->getCurAnimSpeed();
	}
	return 1.0f;
}

void txComponentDynamicMesh::playAnimation(const std::string& anim, const bool& loop, const float& speed, const float& timeOffset,
	const std::string& hookAnim, const bool& hookAnimLoop, const float& hookAnimSpeed, const float& hookAnimTimeOffset)
{
	if (mEntity != NULL)
	{
		mEntity->playClip(anim, mStartFrame, mEndFrame, loop, speed, timeOffset, hookAnim, -1, -1, hookAnimLoop, hookAnimSpeed, hookAnimTimeOffset);
	}
}

void txComponentDynamicMesh::stopCurAnimation()
{
	if (mEntity != NULL)
	{
		mEntity->stop();
	}
}

const std::string& txComponentDynamicMesh::getCurAnimName()
{
	if (mEntity != NULL)
	{
		return mEntity->getCurAnimName();
	}
	return EMPTY_STRING;
}

float txComponentDynamicMesh::getCurAnimLength()
{
	if (mEntity != NULL && mEntity->getCurAnim() != NULL)
	{
		return mEntity->getCurAnim()->getLength();
	}
	return 0.0f;
}

void txComponentDynamicMesh::setPlayState(const PLAY_STATE& state)
{
	if (mEntity != NULL)
	{
		mEntity->setAnimPlayState(state);
	}
}

PLAY_STATE txComponentDynamicMesh::getPlayState()
{
	if (mEntity != NULL)
	{
		return mEntity->getAnimPlayState();
	}
	return PS_STOP;
}

txAABox txComponentDynamicMesh::getWorldAABB(const MATRIX4& exTransform)
{
	if (mEntity == NULL)
	{
		return txAABox();
	}
	return mEntity->getAABB().transform(mEntity->GetParent()->GetFinalMatrix() * exTransform);
}

txAABox txComponentDynamicMesh::getLocalAABB()
{
	if (mEntity == NULL)
	{
		return txAABox();
	}
	return mEntity->getAABB();
}