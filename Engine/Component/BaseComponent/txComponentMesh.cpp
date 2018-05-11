#include "txEntityManager.h"
#include "txNodeManager.h"
#include "txNode.h"
#include "txObjectBoneHook.h"
#include "txEntity.h"
#include "txComponentHeader.h"
#include "txEngineLog.h"
#include "txMeshProperty.h"

void txComponentMesh::init(txComponentOwner* owner)
{
	txComponent::init(owner);
	// 创建组件节点
	mNode = mNodeManager->CreateNode();

	// 如果不是独立的组件,则初始默认挂接到变换组件下
	if (mComponentOwner != NULL)
	{
		txComponentTransform* transformComponent = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transformComponent != NULL)
		{
			transformComponent->getNode()->AddChildNode(mNode);
		}
	}

	// 创建一个空的实体,并且分配节点
	txNode* entityNode = mNodeManager->CreateNode();
	std::string entityName;
	if (mComponentOwner != NULL)
	{
		entityName = mComponentOwner->getName() + "_" + mName;
	}
	else
	{
		entityName = entityNode->GetName() + "_" + mName;
	}
	mEntity = mEntityManager->CreateEmptyEntity(entityName);
	mEntity->SetParent(entityNode);
	mEntity->setVisible(true);
	// 需要将模型缩小100倍
	entityNode->SetScale(VECTOR3(0.01f, 0.01f, 0.01f));

	// 将实体节点挂接到组件节点上
	mNode->AddChildNode(entityNode);
}

void txComponentMesh::initProperty()
{
	txComponent::initProperty();
	addProperty<txMeshPropertyMeshPosition>();
	addProperty<txMeshPropertyMeshRotation>();
	addProperty<txMeshPropertyMeshScale>();
}

void txComponentMesh::update(float elapsedTime)
{
	// 先更新自己的节点,从挂点中获得相对变换,设置到节点中
	if (mHook != NULL)
	{
		const MATRIX4& relativeTransform = mHook->getWorldTransform();
		mNode->SetLocalTransform(relativeTransform);
	}
	txComponent::update(elapsedTime);
}

void txComponentMesh::destroyEntity()
{
	// 当实体不为空,并且不是外部创建的实体
	if (mEntity != NULL && !mIsOutEntity)
	{
		// 销毁实体的节点
		txNode* entityNode = mEntity->GetParent();
		if (entityNode != NULL)
		{
			mNodeManager->DestroyNode(entityNode->GetName());
		}
		// 销毁实体
		mEntityManager->DestroyEntity(mEntity->getName());
	}
	mEntity = NULL;
}

bool txComponentMesh::initEntityByFileName(const std::string& filePath, const bool& hasAnim, const bool& loadTexture)
{
	if (mIsOutEntity)
	{
		ENGINE_ERROR("error : component has an out entity! can not init again!");
		return false;
	}
	// 如果实体不为空,则先清空实体
	if (mEntity != NULL && mEntity->getSubEntityList().size() > 0)
	{
		mEntityManager->ClearEntity(mEntity->getName());
	}
	
	// 初始化实体
	// 如果有动作则使用动态模型的简单拷贝,静态模型就使用简单拷贝,暂时不使用深度拷贝
	ENTITY_COPY_TYPE copyType = hasAnim ? ECT_ANIM_SIMPLE : ECT_SIMPLE;
	mEntityManager->initEntity(mEntity, filePath, hasAnim, copyType, loadTexture);
	mIsOutEntity = false;
	mModelFileName = txStringUtility::getFileName(filePath);
	txNode* entityNode = mEntity->GetParent();
	entityNode->setDirty(true, false);
	return true;
}

bool txComponentMesh::initEntityByEntityName(const std::string& sourceEntityName, const bool& hasAnim, const bool& loadTexture)
{
	const std::string& filePath = mEntityManager->GetFileNameByEntityName(sourceEntityName);
	return initEntityByFileName(filePath, hasAnim, loadTexture);
}

// resetNode为是否重新设置父节点, 如果重新设置,则将实体节点的父节点设置为parentNode
bool txComponentMesh::initEntityByOutEntity(const std::string& sourceEntityName, const bool& resetNode)
{
	// 如果已经有了创建好的实体,则需要先将实体销毁
	if (!mIsOutEntity && mEntity != NULL)
	{
		destroyEntity();
	}
	mEntity = mEntityManager->GetEntity(sourceEntityName);
	mIsOutEntity = true;
	mModelFileName = EMPTY_STRING;
	if (mEntity == NULL)
	{
		return false;
	}
	if (resetNode)
	{
		if (mEntity->GetParent()->GetParent() != NULL)
		{
			ENGINE_ERROR("error : entity node has parent, can not set parent node!");
			return false;
		}
		mNode->AddChildNode(mEntity->GetParent());
	}
	return true;
}

void txComponentMesh::setActive(const bool& active)
{
	txComponent::setActive(active);
	if (mEntity != NULL)
	{
		mEntity->setVisible(active);
	}
}

void txComponentMesh::generateTexture()
{
	if (mEntity != NULL)
	{
		mEntity->GenerateAllSubEntitiesTextures();
	}
}

const VECTOR3& txComponentMesh::getPosition()
{
	if (mEntity == NULL)
	{
		return txMath::VEC3_ZERO;
	}
	return mEntity->GetParent()->GetPosition();
}

const VECTOR3& txComponentMesh::getScale()
{
	if (mEntity == NULL)
	{
		return txMath::VEC3_ZERO;
	}
	return mEntity->GetParent()->GetScale();
}

const MATRIX3& txComponentMesh::getRotation()
{
	if (mEntity == NULL)
	{
		return txMath::MAT3_IDENTITY;
	}
	return mEntity->GetParent()->GetRotationMatrix();
}

void txComponentMesh::setPosition(const VECTOR3& pos)
{
	if (mEntity == NULL)
	{
		return;
	}
	mEntity->GetParent()->SetPosition(pos);
}

void txComponentMesh::setScale(const VECTOR3& scale)
{
	if (mEntity == NULL)
	{
		return;
	}
	mEntity->GetParent()->SetScale(scale);
}

void txComponentMesh::setRotation(const MATRIX3& rot)
{
	if (mEntity == NULL)
	{
		return;
	}
	mEntity->GetParent()->SetRotationMatrix(rot);
}
void txComponentMesh::resetRotation()
{
	if (mEntity == NULL)
	{
		return;
	}
	mEntity->GetParent()->ResetRotation();
}

txAABox txComponentMesh::getWorldAABB()
{
	if (mEntity == NULL)
	{
		return txAABox();
	}
	const txAABox& aabb = mEntity->getAABB();
	const MATRIX4& finalMatrix = mEntity->GetParent()->GetFinalMatrix();
	return aabb.transform(finalMatrix);
}

txAABox txComponentMesh::getLocalAABB()
{
	if (mEntity == NULL)
	{
		return txAABox();
	}
	return mEntity->getAABB();
}

void txComponentMesh::setShowAABB(const bool& show)
{
	if (mEntity != NULL)
	{
		mEntity->setShowAABB(show);
	}
}

bool txComponentMesh::getShowAABB()
{
	if (mEntity != NULL)
	{
		return mEntity->getShowAABB();
	}
	return false;
}

void txComponentMesh::roll(const float& angle)
{
	if (mEntity != NULL)
	{
		mEntity->GetParent()->Roll(angle);
	}
}

void txComponentMesh::pitch(const float& angle)
{
	if (mEntity != NULL)
	{
		mEntity->GetParent()->Pitch(angle);
	}
}

void txComponentMesh::yaw(const float& angle)
{
	if (mEntity != NULL)
	{
		mEntity->GetParent()->Yaw(angle);
	}
}

void txComponentMesh::setHook(const std::string& hookName)
{
	// 父组件不为空,并且父组件是动态模型组件
	if (mParent == NULL || mParent->isType(TOSTRING(txComponentDynamicMesh)))
	{
		txComponentDynamicMesh* dynamicComponent = static_cast<txComponentDynamicMesh*>(mParent);
		txObjectBoneHook* hook = dynamicComponent->getHook(hookName);
		if (hook == NULL)
		{
			ENGINE_ERROR("error : can not find hook : %s", hookName.c_str());
			return;
		}
		mHook = hook;
	}
}