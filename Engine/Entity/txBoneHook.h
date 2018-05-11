#ifndef _BONE_HOOK_H_
#define _BONE_HOOK_H_

#include "txMathUtility.h"

class txBone;
class txBoneHook
{
public:
	txBoneHook(txBone* pBone, const std::string& name);
	virtual ~txBoneHook();
	const std::string& GetName() { return mName; }
	// 设置本地的变换矩阵
	void SetTransform(const MATRIX4& transform);
	// 设置本地位移
	void SetPosition(const VECTOR3& pos)
	{
		mPosition = pos;
		SetDirty(true);
	}
	// 设置本地旋转
	void SetRotation(const MATRIX3& rotate)
	{
		mRotation = rotate;
		SetDirty(true);
	}
	// 设置本地缩放
	void SetScale(const VECTOR3& scale)
	{
		mScale = scale;
		SetDirty(true);
	}
	// 得到本地位移
	const VECTOR3& GetPosition(){ return mPosition; }
	// 得到本地旋转
	const MATRIX3& GetRotation(){ return mRotation; }
	// 得到本地缩放
	const VECTOR3& GetScale(){ return mScale; }
	// 得到世界坐标,实际是相对于模型的位置
	VECTOR3 GetWorldPosition(){ return txMath::getMatrixPosition(mWorldTransform); }
	// 得到世界旋转,实际是相对于模型的旋转
	MATRIX3 GetWorldRotation(){ return txMath::getMatrixRotation(mWorldTransform); }
	// 得到世界缩放,实际是相对于模型的缩放
	VECTOR3 GetWorldScale(){ return txMath::getMatrixScale(mWorldTransform); }
	// 更新挂点
	void Update();
	const MATRIX4& GetWorldTransform() { return mWorldTransform; }
	const MATRIX4& GetLocalTransform() { return mTransform; }
	// 设置更新标记,为true则需要更新,为false则不需要更新
	void SetDirty(const bool& dirty){ mDirty = dirty; }
	// 拷贝挂点的所有数据
	void CopyHook(txBoneHook* hook);
protected:
	// 根据相对位置,相对旋转,相对缩放计算本地变换矩阵
	void UpdateTransform();
protected:
	std::string mName;
	txBone* mHookBone; // 挂接的骨头
	MATRIX4 mTransform; // 相对于骨头的变换
	MATRIX4 mWorldTransform; // 计算出的世界的变换,实际是模型本地空间下的变换
	VECTOR3 mPosition; // 相对与骨头的位置
	MATRIX3 mRotation; // 相对与骨头的旋转
	VECTOR3 mScale; // 相对与骨头的缩放
	bool mDirty;
};

#endif
