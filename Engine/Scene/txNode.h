#ifndef _TX_NODE_H_
#define _TX_NODE_H_

#include "txAABox.h"
#include "txMathUtility.h"

class txObject;

// 场景节点必须通过节点管理器来创建,也必须由场景管理器来销毁
class txNode
{
public:
	txNode(const std::string& name);
	virtual ~txNode();

	// 获得成员变量
	const std::string& GetName()			{ return mName; }
	txNode* GetParent()						{ return mParent; }
	const VECTOR3& GetPositionOffset()		{ return mPosOffset; }
	const VECTOR3& GetPosition()			{ return mPos; }
	const VECTOR3& GetWorldPosition()		{ return mWorldPos; }	// 得到节点的世界绝对坐标
	const bool& GetAlwaysUpdate()			{ return mAlwaysUpdate; }
	const MATRIX3& GetRotationMatrix()		{ return mRotMat; }
	const MATRIX3& GetWorldRotationMatrix()	{ return mWorldRotMat; }
	const VECTOR3& GetScale()				{ return mScale; }
	const MATRIX4& GetFinalMatrix()			{ return mFinalMatrix; }
	txSet<txNode*>& getChildren()	{ return mChildren; }
	txObject* getObject()					{ return mObject; }
	const bool& getDirty()					{ return mDirty; }

	void Update(float fElaspedTime);
	void SetAlwaysUpdate(const bool& update){ mAlwaysUpdate = update; }
	void AddChildNode(txNode* node);
	void RemoveChildNode(txNode* node);

	//object
	void AttachObject(txObject* obj);
	void DetachObject();

	void setPositionOffset(const VECTOR3& offset, const bool& refreshNow = false)
	{
		mPosOffset = offset;
		setDirty(true, refreshNow);
	}

	//position
	void SetPosition(const VECTOR3& pos, const bool& refreshNow = false)
	{
		mPos = pos;
		setDirty(true, refreshNow);
	}
	
	void SetWorldPosition(const VECTOR3& pos, const bool& refreshNow = false);
	void MoveLocal(const VECTOR3& delta, const bool& refreshNow = false)
	{
		mPos += delta;
		setDirty(true, refreshNow);
	}

	//rotation
	void ResetRotation(const bool& refreshNow = false)
	{
		mRotMat = txMath::MAT3_IDENTITY;
		setDirty(true, refreshNow);
	}
	void SetWorldRotationMatrix(const MATRIX3& rotation, const bool& refreshNow = false); // 设置绝对旋转值
	void Pitch(const float& Radian, const bool& refreshNow = false)
	{
		mRotMat = txMath::getPitchMatrix3(Radian) * mRotMat;
		setDirty(true, refreshNow);
	}

	void Roll(const float& Radian, const bool& refreshNow = false)
	{
		mRotMat = txMath::getRollMatrix3(Radian) * mRotMat;
		setDirty(true, refreshNow);
	}

	void Yaw(const float& Radian, const bool& refreshNow = false)
	{
		mRotMat = txMath::getYawMatrix3(Radian) * mRotMat;
		setDirty(true, refreshNow);
	}
	void Rotate(const MATRIX3& rotMat, const bool& refreshNow = false)
	{
		mRotMat = rotMat * mRotMat;
		setDirty(true, refreshNow);
	}
	void SetRotationMatrix(const MATRIX3& mat, const bool& refreshNow = false)
	{
		mRotMat = mat;
		setDirty(true, refreshNow);
	}
	
	// 得到弧度制的本地航向角
	float GetRadianYaw()
	{
		return txMath::getYaw(GetRotateQuaternion());
	}
	float GetRadianPitch(); // 得到弧度制的本地俯仰角,pitch的取值范围是-90°到90°,当超过这个范围时,会调整到该范围
	// 得到弧度制的本地俯仰角
	float GetRadianRoll()
	{
		return txMath::getRoll(GetRotateQuaternion());
	}
	void SetRadianYaw(const float& yaw, const bool& refreshNow = false)
	{
		Yaw(yaw - GetRadianYaw(), refreshNow);
	}
	void SetRadianPitch(const float& pitch, const bool& refreshNow = false)
	{
		Pitch(pitch - GetRadianPitch(), refreshNow);
	}
	void SetRadianRoll(const float& roll, const bool& refreshNow = false)
	{
		Roll(roll - GetRadianRoll(), refreshNow);
	}
	// 得到弧度制的世界航向角
	float GetWorldRadianYaw()
	{
		QUATERNION worldQuat = GetWorldRotateQuaternion();
		float yawRadian = txMath::getYaw(worldQuat);
		return yawRadian;
	}
	// 得到弧度制的世界航向角,pitch的取值范围是-90°到90°,当超过这个范围时,会调整到该范围
	float GetWorldRadianPitch();
	// 得到弧度制的世界航向角
	float GetWorldRadianRoll()
	{
		QUATERNION worldQuat = GetWorldRotateQuaternion();
		float rollRadian = txMath::getRoll(worldQuat);
		return rollRadian;
	}
	void SetWorldRadianYaw(const float& yaw, const bool& refreshNow = false)
	{
		Yaw(yaw - GetWorldRadianYaw(), refreshNow);
	}
	void SetWorldRadianPitch(const float& pitch, const bool& refreshNow = false)
	{
		Pitch(pitch - GetWorldRadianPitch(), refreshNow);
	}
	void SetWorldRadianRoll(const float& roll, const bool& refreshNow = false)
	{
		Roll(roll - GetWorldRadianRoll(), refreshNow);
	}
	// 得到本地旋转的四元数
	QUATERNION GetRotateQuaternion(){ return txMath::matrix3ToQuat(mRotMat); }
	// 得到世界旋转的四元数
	QUATERNION GetWorldRotateQuaternion(){ return txMath::matrix3ToQuat(mWorldRotMat); }

	void SetScale(const VECTOR3& v, const bool& refreshNow = false)
	{
		mScale = v;
		setDirty(true, refreshNow);
	}
	
	VECTOR3 GetDirection(const VECTOR3& front)
	{
		VECTOR3 dirVector = txMath::rotateVector3(front, mRotMat);
		dirVector = txMath::normalize(dirVector);
		return dirVector;
	}
	VECTOR3 GetWorldDirection(const VECTOR3& front)
	{
		VECTOR3 dirVector = txMath::rotateVector3(front, mWorldRotMat);
		dirVector = txMath::normalize(dirVector);
		return dirVector;
	}

	void Refresh();
	void ForceRefresh(); // 强制刷新

	void SetLocalTransform(const MATRIX4& local, const bool& refreshNow = false);

	// 从另一个节点拷贝数据,name,parent,children,objects除外
	void copyNode(txNode* that);

	// 设置更新标记时,如果需要更新,则修改所有的子节点的更新标记为需要更新
	void setDirty(const bool& dirty, const bool& refreshNow);
protected:
	// 不能直接调用设置父节点
	void SetParent(txNode* node)
	{
		mParent = node;
		setDirty(true, false);
	}
protected:
	std::string mName;
	bool mDirty; // 数据是否需要更新
	bool mAlwaysUpdate; // 是否始终都更新,不会影响子节点的是否更新
	txNode* mParent;
	txSet<txNode*> mChildren;
	txObject* mObject;
	//节点的包围盒（ShadowMap时用）
	txAABox mNodeAABox;
	//position
	VECTOR3 mPos;
	VECTOR3 mWorldPos;	// 世界绝对坐标
	VECTOR3 mPosOffset;	// 位置偏移,用于震动

	//rotation
	MATRIX3 mRotMat;
	MATRIX3 mWorldRotMat;

	//scale
	VECTOR3 mScale;

	//final
	MATRIX4 mFinalMatrix;
	MATRIX4 mLocalMatrix;
};

#endif
