#include "txEntity.h"
#include "txBoneHook.h"
#include "txObjectBoneHook.h"
#include "txComponentTransform.h"
#include "txEngineLog.h"

txObjectBoneHook::txObjectBoneHook(const std::string& name)
:
txMovableObject(name),
mHookedBoneName(EMPTY_STRING),
mHook(NULL),
mHookedEntity(NULL)
{}

void txObjectBoneHook::update(float elapsedTime)
{
	txMovableObject::update(elapsedTime);
	// 如果是根据骨骼挂点创建的,则将挂点的世界坐标系下的位置旋转缩放属性设置到可移动物体基类中
	if (mHook != NULL && mHookedEntity != NULL)
	{
		const MATRIX4& entityMatrix = mHookedEntity->GetParent()->GetFinalMatrix();
		mTransformComponent->setPosition(txMath::transformVector3(mHook->GetWorldPosition(), entityMatrix), true);

		QUATERNION rotQuat = txMath::matrix3ToQuat(txMath::getMatrixRotation(entityMatrix) * mHook->GetWorldRotation());
		QUATERNION newQuat;
		newQuat.x = -rotQuat.x;
		newQuat.y = rotQuat.z;
		newQuat.z = rotQuat.y;
		newQuat.w = rotQuat.w;
		newQuat = txMath::rotate(newQuat, -txMath::MATH_PI / 4.0f * 3.0f, txMath::AXIS_Y);
		newQuat = txMath::rotate(newQuat, -txMath::MATH_PI / 2.0f, txMath::AXIS_Z);
		mTransformComponent->setRotation(txMath::quatToMatrix3(newQuat), true);

		VECTOR3 entityScale = txMath::getMatrixScale(entityMatrix);
		VECTOR3 hookScale = mHook->GetWorldScale();
		mTransformComponent->setScale(VECTOR3(entityScale.x * hookScale.x, entityScale.y * hookScale.y, entityScale.z * hookScale.z), true);
	}
}

void txObjectBoneHook::destroy()
{
	if (mHook != NULL && mHookedEntity != NULL)
	{
		mHookedEntity->DestroyBoneHook(mHookedBoneName, mHook->GetName());
		mHook = NULL;
		mHookedBoneName = EMPTY_STRING;
		mHookedEntity = NULL;
		mName = EMPTY_STRING;
	}
}

void txObjectBoneHook::createHook(const std::string& boneName, txEntity* entity)
{
	if (mHook != NULL)
	{
		ENGINE_ERROR("error : game hook has created! can not create again! name : %s", mName.c_str());
		return;
	}
	mHookedBoneName = boneName;
	mHookedEntity = entity;
	mHook = mHookedEntity->CreateBoneHook(mHookedBoneName, mName);
}

void txObjectBoneHook::setPosition(const VECTOR3& position, const bool& refreshNow)
{
	// 如果有骨骼挂点,则将位置设置到骨骼挂点上
	if (mHook != NULL)
	{
		mHook->SetPosition(position);
	}
	// 否则就直接设置到自己可移动物体的属性中
	else
	{
		txMovableObject::setPosition(position, refreshNow);
	}
}

void txObjectBoneHook::setRotation(const MATRIX3& rotation, const bool& refreshNow)
{
	// 如果有骨骼挂点,则将位置设置到骨骼挂点上
	if (mHook != NULL)
	{
		mHook->SetRotation(rotation);
	}
	// 否则就直接设置到自己可移动物体的属性中
	else
	{
		txMovableObject::setRotation(rotation, refreshNow);
	}
}

void txObjectBoneHook::setScale(const VECTOR3& scale, const bool& refreshNow)
{
	// 如果有骨骼挂点,则将位置设置到骨骼挂点上
	if (mHook != NULL)
	{
		mHook->SetScale(scale);
	}
	// 否则就直接设置到自己可移动物体的属性中
	else
	{
		txMovableObject::setScale(scale, refreshNow);
	}
}

const MATRIX4& txObjectBoneHook::getWorldTransform()
{
	if (mHook != NULL)
	{
		return mHook->GetWorldTransform();
	}
	return txMath::MAT4_IDENTITY;
}

VECTOR3 txObjectBoneHook::getWorldPosition()
{
	if (mHook != NULL)
	{
		return mHook->GetWorldPosition();
	}
	return txMath::VEC3_ZERO;
}

MATRIX3 txObjectBoneHook::getWorldRotation()
{
	if (mHook != NULL)
	{
		return mHook->GetWorldRotation();
	}
	return txMath::MAT3_IDENTITY;
}

VECTOR3 txObjectBoneHook::getWorldScale()
{
	if (mHook != NULL)
	{
		return mHook->GetWorldScale();
	}
	return txMath::VEC3_ONE;
}

MATRIX4 txObjectBoneHook::getRealWorldTransform()
{
	if (mHook != NULL && mHookedEntity != NULL)
	{
		return mHookedEntity->GetParent()->GetFinalMatrix() * mHook->GetWorldTransform();
	}
	return txMath::MAT4_IDENTITY;
}

VECTOR3 txObjectBoneHook::getRealWorldPosition()
{
	return txMath::getMatrixPosition(getRealWorldTransform());
}

MATRIX3 txObjectBoneHook::getRealWorldRotation()
{
	return txMath::getMatrixRotation(getRealWorldTransform());
}

VECTOR3 txObjectBoneHook::getRealWorldScale()
{
	return txMath::getMatrixScale(getRealWorldTransform());
}