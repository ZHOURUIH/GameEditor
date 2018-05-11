#include "txSceneManager.h"
#include "txNode.h"
#include "txNodeManager.h"
#include "txEngineRoot.h"

#include "txComponentHeader.h"
#include "txTransformProperty.h"

void txComponentTransform::init(txComponentOwner* owner)
{
	txComponent::init(owner);
	mNode = mNodeManager->CreateNode();
	mSceneManager->getRootNode()->AddChildNode(mNode);
}

void txComponentTransform::initProperty()
{
	txComponent::initProperty();
	addProperty<txTransformPropertyPosition>();
	addProperty<txTransformPropertyRotation>();
	addProperty<txTransformPropertyScale>();
}

void txComponentTransform::update(float elapsedTime)
{
	applyToNode(false);
	txComponent::update(elapsedTime);
}

void txComponentTransform::setParentComponent(txComponent* component)
{
	ENGINE_ERROR("error : can not set transform's parent! transform must be root component!");
}

void txComponentTransform::applyToNode(const bool& refreshNow)
{
	if (NULL == mNode || !mDirty)
	{
		return;
	}
	// 平移
	mNode->SetPosition(mPosition);
	// 震动偏移
	mNode->setPositionOffset(mTrembleOffset);
	// 旋转
	mNode->SetRotationMatrix(mRotation);
	// 缩放
	mNode->SetScale(mScale, refreshNow);
	setDirty(false);
}

void txComponentTransform::setTrembleOffset(const VECTOR3& offset, const bool& refreshNow)
{
	if (!txMath::isVector3Equal(mTrembleOffset, offset))
	{
		mTrembleOffset = offset;
		setDirty(true);
		if (refreshNow)
		{
			applyToNode(refreshNow);
		}
	}
}

void txComponentTransform::setPosition(const VECTOR3& position, const bool& refreshNow)
{
	if (!txMath::isVector3Equal(mPosition, position))
	{
		mPosition = position;
		setDirty(true);
		if (refreshNow)
		{
			applyToNode(refreshNow);
		}
	}
}

void txComponentTransform::setRotation(const MATRIX3& rotation, const bool& refreshNow)
{
	if (!txMath::isMatrix3Equal(mRotation, rotation))
	{
		mRotation = rotation;
		setDirty(true);
		if (refreshNow)
		{
			applyToNode(refreshNow);
		}
	}
}

void txComponentTransform::setScale(const VECTOR3& scale, const bool& refreshNow)
{
	if (!txMath::isVector3Equal(mScale, scale))
	{
		mScale = scale;
		setDirty(true);
		if (refreshNow)
		{
			applyToNode(refreshNow);
		}
	}
}