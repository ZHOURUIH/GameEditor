#include "txBone.h"
#include "txBoneHook.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "Utility.h"

txBone::txBone(const std::string& name, txSkeleton* pParentSkeleton)
:
mName(name),
mParentSkeleton(pParentSkeleton),
mParent(NULL),
mScale(txMath::VEC3_ONE),
mDirty(true)
{
	;
}

txBone::~txBone()
{
	mChildren.clear();
	mParent = NULL;
	DestroyAllHook();
}

txBoneHook* txBone::CreateHook(const std::string& hookname)
{
	auto iter = mHookList.find(hookname);
	if(iter != mHookList.end())
	{
		ENGINE_ERROR("error : can not create hook, hook : %s have already exist!", hookname.c_str());
		return NULL;
	}
	txBoneHook* pHook = TRACE_NEW(txBoneHook, pHook, this, hookname);
	mHookList.insert(hookname, pHook);
	// 因为需要更新挂点,所以设置为true
	setDirty(true);
	return pHook;
}

txBoneHook* txBone::GetHook(const std::string& hookname)
{
	auto iter = mHookList.find(hookname);
	if(iter == mHookList.end())
	{
		ENGINE_ERROR("error : can not find hook named : %s", hookname.c_str());
		return NULL;
	}
	return iter->second;
}

bool txBone::DestroyHook(const std::string& hookname)
{
	auto iter = mHookList.find(hookname);
	if(iter == mHookList.end())
	{
		ENGINE_ERROR("error : can not destroy hook named : %s", hookname.c_str());
		return false;
	}
	TRACE_DELETE(iter->second);
	mHookList.erase(iter);
	return true;
}

void txBone::DestroyAllHook()
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

void txBone::RemoveChildBone(txBone* bone)
{
	auto iter = mChildren.begin();
	auto iterEnd = mChildren.end();
	FOR_STL(mChildren, ; iter != iterEnd; ++iter)
	{
		if (*iter == bone)
		{
			mChildren.erase(iter);
			bone->SetParent(NULL);
			break;
		}
	}
}

void txBone::Refresh()
{
	if (mDirty)
	{
		// 更新自己的矩阵
		if (mParent == NULL)
		{
			mDerivedPos = mPos;
			mDerivedRotMat = mRotMat;

			MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, mScale);
			MATRIX4 transMat = txMath::translate(txMath::MAT4_IDENTITY, mPos);

			mLocalMatrix = MATRIX4(mRotMat) * scaleMat;
			mLocalMatrix = transMat * mLocalMatrix;
			mFinalMatrix = mLocalMatrix;
		}
		else
		{
			const MATRIX4& parentFinalMatrix = mParent->GetFinalMatrix();
			mDerivedPos = txMath::transformVector3(mPos, parentFinalMatrix);
			mDerivedRotMat = mRotMat * txMath::getMatrixRotation(parentFinalMatrix);

			MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, mScale);
			MATRIX4 transMat = txMath::translate(txMath::MAT4_IDENTITY, mPos);

			mLocalMatrix = MATRIX4(mRotMat) * scaleMat;
			mLocalMatrix = transMat * mLocalMatrix;
			mFinalMatrix = parentFinalMatrix * mLocalMatrix;
		}

		// 更新这个骨头上绑定的所有挂点
		auto iter = mHookList.begin();
		auto iterEnd = mHookList.end();
		FOR_STL(mHookList, ; iter != iterEnd; ++iter)
		{
			if (iter->second != NULL)
			{
				iter->second->Update();
			}
		}
		END_FOR_STL(mHookList);
		setDirty(false);
	}

	// 更新子骨骼
	int boneCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < boneCount; ++i)
	{
		mChildren[i]->Refresh();
	}
	END_FOR_STL(mChildren);
}

void txBone::CopyBone(txBone* pBone)
{
	// 先拷贝所有的子骨头
	int childrenCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < childrenCount; ++i)
	{
		txBone* pChildBone = TRACE_NEW(txBone, pChildBone, mChildren[i]->GetName(), pBone->GetParentSkeleton());
		mChildren[i]->CopyBone(pChildBone);
		pChildBone->SetParent(pBone);
		pChildBone->SetParentName(pBone->GetName().c_str());
		pBone->mChildren.push_back(pChildBone);
	}
	END_FOR_STL(mChildren);

	// 然后再拷贝自身数据,parent不能直接拷贝
	//position
	pBone->mPos = mPos;
	pBone->mDerivedPos = mDerivedPos;

	//rotation
	pBone->mRotMat = mRotMat;
	pBone->mDerivedRotMat = mDerivedRotMat;

	//scale
	pBone->mScale = mScale;

	//final
	pBone->mFinalMatrix = mFinalMatrix;
	pBone->mLocalMatrix = mLocalMatrix;

	// 挂点
	auto iter = mHookList.begin();
	auto iterEnd = mHookList.end();
	FOR_STL(mHookList, ; iter != iterEnd; ++iter)
	{
		txBoneHook* pHook = pBone->CreateHook(iter->first);
		iter->second->CopyHook(pHook);
	}
	END_FOR_STL(mHookList);
}

void txBone::GetAllChildren(txBone* pBone, txMap<std::string, txBone*>& boneList)
{
	// 如果有子骨头就先将子骨头放入列表
	int childrenCount = pBone->mChildren.size();
	FOR_STL(pBone->mChildren, int i = 0; i < childrenCount; ++i)
	{
		GetAllChildren(pBone->mChildren[i], boneList);
	}
	END_FOR_STL(pBone->mChildren);

	// 再将自己放入列表
	boneList.insert(pBone->GetName(), pBone);
}

void txBone::setDirty(const bool& dirty)
{
	// 如果修改标记相同,则直接返回
	if (mDirty == dirty)
	{
		return;
	}
	mDirty = dirty;
	// 设置子节点需要刷新
	if (mDirty)
	{
		int boneCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < boneCount; ++i)
		{
			txBone* pBone = mChildren[i];
			if (pBone != NULL)
			{
				pBone->setDirty(dirty);
			}
		}
		END_FOR_STL(mChildren);
	}
}