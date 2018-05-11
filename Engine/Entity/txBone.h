#ifndef _TX_BONE_H_
#define _TX_BONE_H_

#include "txMathUtility.h"

class txBoneHook;
class txSkeleton;
class txBone
{
public:
	txBone(const std::string& name, txSkeleton* pParentSkeleton);
	virtual ~txBone();

	const std::string& GetName() { return mName; }
	txSkeleton* GetParentSkeleton() { return mParentSkeleton; }
	void SetParentSkeleton(txSkeleton* pSkeleton) { mParentSkeleton = pSkeleton; }

	txBoneHook* CreateHook(const std::string& hookname);
	txBoneHook* GetHook(const std::string& hookname);
	bool DestroyHook(const std::string& hookname);
	void DestroyAllHook();
	bool HasHook(const std::string& hookname){ return mHookList.find(hookname) != mHookList.end(); }

	void SetParent(txBone* bone)
	{
		mParent = bone;
		setDirty(true);
	}
	txBone* GetParent(){ return mParent; }

	void AddChildBone(txBone* bone)
	{
		mChildren.push_back(bone);
		bone->SetParent(this);
	}
	void RemoveChildBone(txBone* bone);

	//position
	void SetPosition(const VECTOR3& pos)
	{
		mPos = pos;
		setDirty(true);
	}
	const VECTOR3& GetPosition(){ return mPos; }
	const VECTOR3& GetDerivedPosition(){ return mDerivedPos; }

	//rotation
	void ResetRotation()
	{
		mRotMat = txMath::MAT3_IDENTITY;
		setDirty(true);
	}
	void Pitch(const float Radian)
	{
		mRotMat = txMath::getPitchMatrix3(Radian) * mRotMat;
		setDirty(true);
	}
	void Roll(const float Radian)
	{
		mRotMat = txMath::getRollMatrix3(Radian) * mRotMat;
		setDirty(true);
	}
	void Yaw(const float Radian)
	{
		mRotMat = txMath::getYawMatrix3(Radian) * mRotMat;
		setDirty(true);
	}

	void SetRotationMatrix(const MATRIX3& mat)
	{
		mRotMat = mat;
		setDirty(true);
	}
	const MATRIX3& GetRotationMatrix(){ return mRotMat; }
	const MATRIX3& GetDerivedRotationMatrix(){ return mDerivedRotMat; }

	//scale
	void SetScale(const VECTOR3& v)
	{
		mScale = v;
		setDirty(true);
	}
	const VECTOR3& GetScale(){ return mScale; }

	//direction
	VECTOR3 GetDirection(const VECTOR3& front)
	{
		VECTOR3 dirVector = front * MATRIX3(mLocalMatrix);
		return txMath::normalize(dirVector);
	}

	void Refresh();

	//for
	const MATRIX4& GetFinalMatrix(){ return mFinalMatrix; }
	const MATRIX4& GetLocalMatrix(){ return mLocalMatrix; }

	//ParentIndex
	const std::string& GetParentName() { return mParentName; }
	void SetParentName(const char* parentName){ mParentName = parentName; }

	txVector<txBone*>& getBoneList() { return mChildren; }

	void setDirty(const bool& dirty);

	// 拷贝骨头数据,里面会递归将所有的子骨头也一起拷贝
	void CopyBone(txBone* pBone);
	// 递归得到所有的子骨头,包括pBone自己也会放入列表
	static void GetAllChildren(txBone* pBone, txMap<std::string, txBone*>& boneList);

protected:
	txSkeleton* mParentSkeleton;
	std::string mName;
	txBone* mParent;
	std::string mParentName;
	txVector<txBone*> mChildren;

	//position
	VECTOR3 mPos;
	VECTOR3 mDerivedPos;

	//rotation
	MATRIX3 mRotMat;
	MATRIX3 mDerivedRotMat;

	//scale
	VECTOR3 mScale;

	//final
	MATRIX4 mFinalMatrix;
	MATRIX4 mLocalMatrix;

	// 是否需要更新
	bool mDirty;

	// 该骨头上的挂点列表,first是挂点名
	txMap<std::string, txBoneHook*> mHookList;
};

#endif
