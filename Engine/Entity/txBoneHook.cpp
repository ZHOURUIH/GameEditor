#include "txBoneHook.h"
#include "txSkeleton.h"
#include "txBone.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"
#include "Utility.h"

txBoneHook::txBoneHook(txBone* pBone, const std::string& name)
{
	mName = name;
	mHookBone = pBone;
	if(mHookBone == NULL)
	{
		ENGINE_ERROR("error : hook bone is NULL!");
	}
	mTransform = txMath::MAT4_IDENTITY;
	if(mHookBone != NULL)
	{
		mWorldTransform = mHookBone->GetFinalMatrix();
		mHookBone->GetParentSkeleton()->AddHookNameToList(mName, mHookBone->GetName());
	}
	mPosition = txMath::VEC3_ZERO;
	mRotation = txMath::MAT3_IDENTITY;
	mScale = txMath::VEC3_ONE;
	mDirty = true;
}

txBoneHook::~txBoneHook()
{
	if(mHookBone != NULL)
	{
		mHookBone->GetParentSkeleton()->RemoveHookNameFromList(mName);
		mHookBone = NULL;
	}
}

void txBoneHook::SetTransform(const MATRIX4& transform)
{
	// 设置本地变换后,根据设置的矩阵,从中获取本地位移,旋转,缩放
	mTransform = transform;
	mPosition = txMath::getMatrixPosition(mTransform);
	mScale = txMath::getMatrixScale(mTransform);
	mRotation = txMath::getMatrixRotation(mTransform);
	SetDirty(false);
}

void txBoneHook::Update()
{
	UpdateTransform();
	mWorldTransform = mHookBone->GetFinalMatrix() * mTransform;
}

void txBoneHook::UpdateTransform()
{
	if(mDirty && mHookBone != NULL)
	{
		MATRIX4 posMat = txMath::translate(txMath::MAT4_IDENTITY, mPosition);
		MATRIX4 rotMat = MATRIX4(mRotation);
		MATRIX4 sclMat = txMath::scale(txMath::MAT4_IDENTITY, mScale);
		mTransform = sclMat;
		mTransform = rotMat * mTransform;
		mTransform = posMat * mTransform;
		SetDirty(false);
	}
}

void txBoneHook::CopyHook(txBoneHook* hook)
{
	// 名字和挂接的骨头是构造时确定的,所以不需要拷贝
	hook->mTransform = mTransform;
	hook->mWorldTransform = mWorldTransform;
	hook->mPosition = mPosition;
	hook->mRotation = mRotation;
	hook->mScale = mScale;
	hook->mDirty = mDirty;
}
