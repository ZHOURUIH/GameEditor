#include "txNode.h"
#include "txObject.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"

txNode::txNode(const std::string& name)
:
mParent(NULL),
mName(name),
mScale(txMath::VEC3_ONE),
mDirty(true),
mAlwaysUpdate(false),
mObject(NULL)
{
	mChildren.clear();
}

txNode::~txNode()
{
	// 移除挂接的所有物体
	DetachObject();
	auto iter = mChildren.begin();
	auto iterEnd = mChildren.end();
	FOR_STL(mChildren, ; iter != iterEnd; ++iter)
	{
		(*iter)->SetParent(NULL);
	}
	END_FOR_STL(mChildren);
	mChildren.clear();
	if (mParent != NULL)
	{
		mParent->RemoveChildNode(this);
	}
}

void txNode::Update(float fElaspedTime)
{
	// 当节点或者父节点的变换矩阵发生改变,则更新自己的平移旋转缩放
	if(mDirty)
	{
		Refresh();
	}
	// 节点变换矩阵发生改变需要被更新或者挂接的物体始终都要更新
	if(mDirty || mAlwaysUpdate)
	{
		// 更新挂接到自己身上的物体
		if (mObject != NULL)
		{
			mObject->Update(fElaspedTime);
		}
	}

	//update the childNode
	auto iter = mChildren.begin();
	auto iterEnd = mChildren.end();
	FOR_STL(mChildren, ; iter != iterEnd; ++iter)
	{
		(*iter)->Update(fElaspedTime);
	}
	END_FOR_STL(mChildren);
	// 已经更新,修改更新标记
	setDirty(false, false);
}

void txNode::AddChildNode(txNode* node)
{
	mChildren.insert(node);
	node->SetParent(this);
}

void txNode::RemoveChildNode(txNode* node)
{
	if (node != NULL)
	{
		auto iter = mChildren.find(node);
		if (iter != mChildren.end())
		{
			mChildren.erase(iter);
			node->SetParent(NULL);
		}
	}
}

void txNode::AttachObject(txObject* obj)
{
	// 如果当前有挂接物体,则先取下物体
	if (mObject != NULL)
	{
		DetachObject();
	}
	// 保存物体,并且设置物体的节点为当前节点
	mObject = obj;
	if (mObject->GetParent() != this)
	{
		mObject->SetParent(this);
	}
}

void txNode::DetachObject()
{
	if (mObject != NULL)
	{
		// 如果已经清空了,则不用再次清空
		if (mObject->GetParent() != NULL)
		{
			mObject->SetParent(NULL);
		}
		mObject = NULL;
	}
}

void txNode::SetWorldPosition(const VECTOR3& pos, const bool& refreshNow)
{
	if(mParent != NULL)
	{
		SetPosition(pos - mParent->GetWorldPosition());
	}
	else
	{
		SetPosition(pos);
	}
	setDirty(true, refreshNow);
}

void txNode::SetWorldRotationMatrix(const MATRIX3& rotation, const bool& refreshNow)
{
	if(mParent != NULL)
	{
		// 父节点的旋转矩阵的逆矩阵
		MATRIX3 inversMat4 = txMath::inverse(mParent->GetWorldRotationMatrix());
		mRotMat = inversMat4 * rotation;
	}
	else
	{
		mRotMat = rotation;
	}
	setDirty(true, refreshNow);
}

float txNode::GetRadianPitch()
{
	QUATERNION quat = GetRotateQuaternion();
	float pitchRadian = txMath::getPitch(quat);
	// 当pitch大于90°时,用180°减去pitch
	if(pitchRadian > 3.14f / 2.0f)
	{
		pitchRadian = 3.14f - pitchRadian;
	}
	// 当pitch小于-90°时,则用-180°减去pitch
	else if(pitchRadian > 3.14f / 2.0f)
	{
		pitchRadian = -3.14f - pitchRadian;
	}
	return pitchRadian;
}

float txNode::GetWorldRadianPitch()
{
	QUATERNION worldQuat = GetWorldRotateQuaternion();
	float pitchRadian = txMath::getPitch(worldQuat);
	// 当pitch大于90°时,用180°减去pitch
	if(pitchRadian > 3.14f / 2.0f)
	{
		pitchRadian = 3.14f - pitchRadian;
	}
	// 当pitch小于-90°时,则用-180°减去pitch
	else if(pitchRadian > 3.14f / 2.0f)
	{
		pitchRadian = -3.14f - pitchRadian;
	}
	return pitchRadian;
}

void txNode::Refresh()
{
	if (mParent == NULL)
	{
		mWorldPos = mPos;
		mWorldRotMat = mRotMat;

		MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, mScale);
		MATRIX4 transMat = txMath::translate(txMath::MAT4_IDENTITY, mPos + mPosOffset);
		mLocalMatrix = MATRIX4(mRotMat) * scaleMat;
		mLocalMatrix = transMat * mLocalMatrix;
		mFinalMatrix = mLocalMatrix;
	}
	else
	{
		mWorldPos = txMath::transformVector3(mPos, mParent->GetFinalMatrix());
		mWorldRotMat = mParent->GetWorldRotationMatrix() * mRotMat;

		MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, mScale);
		MATRIX4 transMat = txMath::translate(txMath::MAT4_IDENTITY, mPos + mPosOffset);

		mLocalMatrix = MATRIX4(mRotMat) * scaleMat;
		mLocalMatrix = transMat * mLocalMatrix;

		mFinalMatrix = mParent->GetFinalMatrix() * mLocalMatrix;
	}

	// 通知节点上挂接的所有物体,父节点重新刷新了
	if (mObject != NULL)
	{
		mObject->NotifyParentUpdated();
	}
}

void txNode::ForceRefresh()
{
	// 先判断父节点是否需要刷新,如果需要刷新,则强制刷新父节点
	if (mParent != NULL && mParent->getDirty())
	{
		mParent->ForceRefresh();
	}
	Refresh();
	setDirty(false, false);
}

void txNode::SetLocalTransform(const MATRIX4& local, const bool& refreshNow)
{
	// 计算其他的值,并且不要修改更新标记,虽然节点本地的数据已经更新,但是父节点可能需要更新而引起子节点需要更新
	mPos = txMath::getMatrixPosition(local);
	mWorldPos = txMath::transformVector3(mPos, mParent->GetFinalMatrix());
	mScale = txMath::getMatrixScale(local);
	mRotMat = txMath::getMatrixRotation(local);
	mWorldRotMat = mParent->GetWorldRotationMatrix() * mRotMat;
	mLocalMatrix = local;
	mFinalMatrix = mParent->GetFinalMatrix() * mLocalMatrix;

	// 设置所有子节点需要更新
	auto ite = mChildren.begin();
	auto iteEnd = mChildren.end();
	FOR_STL(mChildren, ; ite != iteEnd; ++ite)
	{
		txNode* pNode = *ite;
		if(pNode != NULL)
		{
			pNode->setDirty(true, refreshNow);
		}
		else
		{
			ENGINE_ERROR("error : child node is NULL!");
		}
	}
}

void txNode::copyNode(txNode* that)
{
	mDirty = true;
	mAlwaysUpdate = that->mAlwaysUpdate;
	mPos = that->mPos;
	mWorldPos = that->mWorldPos;
	mRotMat = that->mRotMat;
	mWorldRotMat = that->mWorldRotMat;
	mScale = that->mScale;
	mFinalMatrix = that->mFinalMatrix;
	mLocalMatrix = that->mLocalMatrix;
	mNodeAABox = that->mNodeAABox;
}

void txNode::setDirty(const bool& dirty, const bool& refreshNow)
{
	mDirty = dirty;
	// 设置子节点需要刷新
	if(mDirty)
	{
		// 如果需要强制刷新,则先强制刷新自己
		if (refreshNow)
		{
			ForceRefresh();
		}
		auto ite = mChildren.begin();
		auto iteEnd = mChildren.end();
		FOR_STL(mChildren, ; ite != iteEnd; ++ite)
		{
			txNode* pNode = *ite;
			if(pNode != NULL)
			{
				// 此处使用dirty,不能使用mDirty,因为在ForceRefresh中会改变mDirty
				pNode->setDirty(dirty, refreshNow);
			}
			else
			{
				ENGINE_ERROR("error : child node is NULL!");
			}
		}
		END_FOR_STL(mChildren);
	}
}