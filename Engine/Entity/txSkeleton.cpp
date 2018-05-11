#include "txSkeleton.h"
#include "txBone.h"
#include "txEngineLog.h"
#include "txSceneManager.h"
#include "txEntity.h"
#include "txEngineRoot.h"

txSkeleton::txSkeleton(txEntity* parentEntity)
:
mParentEntity(parentEntity),
mRoot(NULL)
{
	mBoneList.clear();
}

txSkeleton::~txSkeleton()
{
	Clear();
}

void txSkeleton::Clear()
{
	DestroyBone(mRoot);
	mBoneList.clear();
}

void txSkeleton::DestroyBone(txBone* pBone)
{
	if(pBone == NULL)
	{
		return;
	}
	// 先销毁所有的子骨头
	txVector<txBone*>& boneList = pBone->getBoneList();
	int boneCount = boneList.size();
	FOR_STL(boneList, int i = 0; i < boneCount; ++i)
	{
		txBone* bone = boneList[i];
		DestroyBone(bone);
	}
	END_FOR_STL(boneList);
	// 再销毁骨头
	TRACE_DELETE(pBone);
}

txBoneHook* txSkeleton::CreateBoneHook(const std::string& boneName, const std::string& hookName)
{
	auto iter = mBoneList.find(boneName);
	if (iter == mBoneList.end())
	{
		ENGINE_ERROR("error : can not create hook : %s, bone named: %s is not exist!", hookName.c_str(), boneName.c_str());
		return NULL;
	}
	return iter->second->CreateHook(hookName);
}

txBoneHook* txSkeleton::GetBoneHook(const std::string& boneName, const std::string& hookName)
{
	auto iter = mBoneList.find(boneName);
	if (iter == mBoneList.end())
	{
		ENGINE_ERROR("error : can not get hook : %s, bone named: %s is not exist!", hookName.c_str(), boneName.c_str());
		return NULL;
	}
	return iter->second->GetHook(hookName);
}

bool txSkeleton::DestroyBoneHook(const std::string& boneName, const std::string& hookName)
{
	auto iter = mBoneList.find(boneName);
	if (iter == mBoneList.end())
	{
		ENGINE_ERROR("error : can not destroy hook : %s, bone named: %s is not exist!", hookName.c_str(), boneName.c_str());
		return false;
	}
	return iter->second->DestroyHook(hookName);
}

txBoneHook* txSkeleton::GetHookInAllBones(const std::string& hookName)
{
	auto iter = mBoneHookList.find(hookName);
	if(iter == mBoneHookList.end())
	{
		ENGINE_ERROR("error : can not get hook : %s in all bones", hookName.c_str());
		return NULL;
	}
	txBone* pBone = GetBone(iter->second.c_str());
	if(pBone == NULL)
	{
		return NULL;
	}
	return pBone->GetHook(hookName);
}

bool txSkeleton::DestroyHookInAllBones(const std::string& hookName)
{
	auto iter = mBoneHookList.find(hookName);
	if(iter == mBoneHookList.end())
	{
		ENGINE_ERROR("error : can not destroy hook : %s in all bones", hookName.c_str());
		return false;
	}
	txBone* pBone = GetBone(iter->second.c_str());
	if(pBone == NULL)
	{
		return false;
	}
	return pBone->DestroyHook(hookName);
}

// 返回是否在指定的骨头中有指定名字的挂点
bool txSkeleton::HasHook(const std::string& boneName, const std::string& hookName)
{
	txBone* pBone = GetBone(boneName.c_str());
	if(pBone == NULL)
	{
		return false;
	}
	return pBone->HasHook(hookName);
}

// 在所有挂点列表中查找是否有该名字的挂点
bool txSkeleton::HasHookInAllBones(const std::string& hookName)
{
	auto iter = mBoneHookList.find(hookName);
	if(iter == mBoneHookList.end())
	{
		return false;
	}
	txBone* pBone = GetBone(iter->second.c_str());
	if(pBone == NULL)
	{
		return false;
	}
	return pBone->HasHook(hookName);
}

txBone* txSkeleton::CreateBone(const std::string& name)
{
	if (mBoneList.find(name) != mBoneList.end())
	{
		ENGINE_ERROR("error : can not create bone : %s, the bone have already exsit!", name);
		return NULL;
	}

	txBone* bone = TRACE_NEW(txBone, bone, name, this);
	mBoneList.insert(name, bone);
	if (name == "root")
	{
		mRoot = bone;
	}
	return bone;
}

txBone* txSkeleton::GetBone(const std::string& name)
{
	auto iter = mBoneList.find(name);
	if (iter == mBoneList.end())
	{
		ENGINE_ERROR("error : can not find bone : %s, entity name : %s", name, mParentEntity->getName());
		return NULL;
	}
	return iter->second;
}

void txSkeleton::SetBonesParent()
{
	auto iter = mBoneList.begin();
	auto iterEnd = mBoneList.end();
	FOR_STL(mBoneList, ; iter != iterEnd; ++iter)
	{
		txBone* pBone = iter->second;
		if (pBone != NULL)
		{
			const std::string& boneName = pBone->GetParentName();
			if (boneName == "none")
			{
				pBone->SetParent(NULL);
				mRoot = pBone;
			}
			else
			{
				txBone* pParentBone = GetBone(boneName.c_str());
				if (pParentBone != NULL)
				{
					pParentBone->AddChildBone(pBone);
				}
			}
		}
	}
	END_FOR_STL(mBoneList);

	iter = mBoneList.begin();
	FOR_STL(mBoneList, ; iter != iterEnd; ++iter)
	{
		txBone* pBone = iter->second;
		if (pBone != NULL)
		{
			pBone->Refresh();
		}
	}
	END_FOR_STL(mBoneList);
}

void txSkeleton::AddHookNameToList(const std::string& hookName, const std::string& boneName)
{
	if(mBoneHookList.find(hookName) != mBoneHookList.end())
	{
		ENGINE_ERROR("error : hook named : %s have already in the list, insert failed!", hookName.c_str());
		return;
	}
	mBoneHookList.insert(hookName, boneName);
}

void txSkeleton::RemoveHookNameFromList(const std::string& hookName)
{
	auto iter = mBoneHookList.find(hookName);
	if(iter == mBoneHookList.end())
	{
		ENGINE_ERROR("error : hook named : %s is not in the list, remove failed!", hookName.c_str());
		return;
	}
	mBoneHookList.erase(iter);
}

void txSkeleton::CopySkeleton(txSkeleton* pSkeleton)
{
	// 如果骨骼数据为空,则直接返回
	if(mRoot == NULL)
	{
		return;
	}
	else
	{
		pSkeleton->mRoot = TRACE_NEW(txBone, pSkeleton->mRoot, mRoot->GetName(), pSkeleton);
		mRoot->CopyBone(pSkeleton->mRoot);
		// 然后将根骨头的所有子骨头和根骨头自己放入列表中,顺序可能会与原骨骼的骨头列表中的顺序不同
		// 不能直接拷贝原列表中的
		txBone::GetAllChildren(pSkeleton->mRoot, pSkeleton->mBoneList);
		// 挂点列表是创建挂点时生成的,不需要拷贝
	}
}

void txSkeleton::Refresh()
{
	if (mRoot != NULL)
	{
		mRoot->Refresh();
	}
}