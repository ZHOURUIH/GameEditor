#include "txEntityManager.h"
#include "txEntity.h"
#include "txMesh.h"
#include "txMaterial.h"
#include "txLoadUtility.h"
#include "txSkeleton.h"
#include "txBone.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"

txVector<std::string> txEntityManager::EMPTY_STRING_VECTOR;

txEntityManager::txEntityManager()
{
	mEntityList.clear();
}

txEntityManager::~txEntityManager()
{
	DestroyAllEntity();
}

txEntity* txEntityManager::CreateEmptyEntity(const std::string& name)
{
	if (GetEntity(name) != NULL)
	{
		ENGINE_ERROR("error : entity named : %s have already exist! can not create again!", name.c_str());
		return NULL;
	}

	txEntity* pEntity = TRACE_NEW(txEntity, pEntity, name);
	mEntityList.insert(name, pEntity);
	return pEntity;
}

txEntity* txEntityManager::CreateEntity(const std::string& name, const std::string& filename, const bool& hasAnimation, const ENTITY_COPY_TYPE& copyType, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	if(GetEntity(name) != NULL)
	{
		ENGINE_ERROR("error : entity named : %s have already exist! can not create again!", name.c_str());
		return NULL;
	}
	// 如果已经由该文件创建了模型,则从已创建的实体来创建
	auto iterFileName = mFileName_EntityName_List.find(filename);
	if(iterFileName != mFileName_EntityName_List.end())
	{
		if(iterFileName->second.size() != 0)
		{
			return CreateWithExistEntity(name, GetEntity((iterFileName->second)[0]), copyType);
		}
	}

	// 创建空实体,然后初始化
	txEntity* pEntity = CreateEmptyEntity(name);
	if(!initEntity(pEntity, filename, hasAnimation, copyType, loadTexture, textureList))
	{
		ENGINE_ERROR("error : init entity failed!");
	}
	return pEntity;
}

bool txEntityManager::initEntity(txEntity* entity, const std::string& filename, const bool& hasAnimation, const ENTITY_COPY_TYPE& copyType, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	if (entity->getSubEntityList().size() != 0)
	{
		ENGINE_ERROR("error : entity is not empty! can not init it! entity : %s", entity->getName());
		return false;
	}

	// 如果已经由该文件创建了模型,则从已创建的实体来创建
	auto iterFileName = mFileName_EntityName_List.find(filename);
	if (iterFileName != mFileName_EntityName_List.end())
	{
		if (iterFileName->second.size() != 0)
		{
			return initWithExistEntity(entity, GetEntity(iterFileName->second[0]), copyType);
		}
	}

	// 加载模型数据
	bool ret = false;
	if (hasAnimation)
	{
		txVector<SkinMeshPair> skinMeshList;
		ret = txLoadUtility::loadSkeletonMeshFromFile(filename, skinMeshList, entity, loadTexture, textureList);
		int meshSize = skinMeshList.size();
		FOR_STL(skinMeshList, int i = 0; i < meshSize; ++i)
		{
			const SkinMeshPair& pskeletonMesh = skinMeshList[i];
			entity->CreateSubEntity(pskeletonMesh.mMesh, pskeletonMesh.mSkin);
		}
		END_FOR_STL(skinMeshList);
	}
	else
	{
		txMesh* pMesh = TRACE_NEW(txMesh, pMesh);
		ret = txLoadUtility::loadMeshFromFile(filename, pMesh, loadTexture, textureList);
		entity->CreateSubEntity(pMesh);
	}

	if (!ret)
	{
		ENGINE_ERROR("error : load mesh failed!");
	}

	// 如果是动态模型,创建完以后需要再次刷新实体的骨骼
	if (entity->GetSkeleton()->GetRootBone() != NULL)
	{
		entity->GetSkeleton()->GetRootBone()->setDirty(true);
	}
	mEntityName_FileName_List.insert(entity->getName(), filename);
	// 已经确定之前没有创建该文件的实体,所以需要新建一个列表再插入
	txVector<std::string> entityNameList;
	entityNameList.push_back(entity->getName());
	mFileName_EntityName_List.insert(filename, entityNameList);
	return true;
}

bool txEntityManager::initWithExistEntity(txEntity* entity, txEntity* sourceEntity, const ENTITY_COPY_TYPE& copyType)
{
	// 查找源实体的文件名
	auto iterFileName = mEntityName_FileName_List.find(sourceEntity->getName());
	if (iterFileName == mEntityName_FileName_List.end())
	{
		ENGINE_ERROR("error : can not find sourceName in namelist, sourceName : %s", sourceEntity->getName());
		return NULL;
	}
	sourceEntity->CopyEntity(entity, copyType);
	// 创建完以后需要再次刷新实体的骨骼
	if (entity->GetSkeleton()->GetRootBone() != NULL)
	{
		entity->GetSkeleton()->GetRootBone()->setDirty(true);
	}

	const std::string& filename = iterFileName->second;
	mEntityName_FileName_List.insert(entity->getName(), filename);
	// 已经确定使用该文件创建过了实体,直接插入实体名就可以了
	auto iterFile = mFileName_EntityName_List.find(filename);
	if (iterFile != mFileName_EntityName_List.end())
	{
		iterFile->second.push_back(entity->getName());
	}
	else
	{
		ENGINE_ERROR("error : sourceEntity have been created, but the filename is not in the list, filename : %s, entity name : %s", filename.c_str(), sourceEntity->getName());
	}
	return true;
}

txEntity* txEntityManager::CreateWithExistEntity(const std::string& name, txEntity* sourceEntity, const ENTITY_COPY_TYPE& copyType)
{
	// 该实体是否已经创建,如果已经创建,则不能再创建相同名字的实体
	if(GetEntity(name) != NULL)
	{
		ENGINE_ERROR("error : entity named : %s have already exist! can not create again!", name.c_str());
		return NULL;
	}
	
	txEntity* pEntity = CreateEmptyEntity(name);
	if (!initWithExistEntity(pEntity, sourceEntity, copyType))
	{
		ENGINE_ERROR("error : init entity failed!");
	}
	return pEntity;
}

txEntity* txEntityManager::CreateManualEntity(const std::string& name, txMesh* pMesh)
{
	txEntity* pEntity = CreateEmptyEntity(name);
	if (pEntity == NULL)
	{
		return NULL;
	}
	pEntity->CreateSubEntity(pMesh);
	// 创建完以后需要再次刷新实体的骨骼
	if (pEntity->GetSkeleton()->GetRootBone() != NULL)
	{
		pEntity->GetSkeleton()->GetRootBone()->setDirty(true);
	}
	return pEntity;
}

txEntity* txEntityManager::GetEntity(const std::string& name)
{
	auto iter = mEntityList.find(name);
	if(iter == mEntityList.end())
	{
		return NULL;
	}
	return iter->second;
}

txEntity* txEntityManager::GetFirstEntityByFileName(const std::string& filename)
{
	auto iter = mFileName_EntityName_List.find(filename);
	if(iter == mFileName_EntityName_List.end())
	{
		return NULL;
	}
	if(iter->second.size() == 0)
	{
		return NULL;
	}
	txEntity* entity = GetEntity((iter->second)[0]);
	if (entity == NULL)
	{
		ENGINE_ERROR("error : can not find entity named : %s in entity list", (iter->second)[0].c_str());
		return NULL;
	}
	return entity;
}

const txVector<std::string>& txEntityManager::GetEntityNamesByFileName(const std::string& filename)
{
	auto iter = mFileName_EntityName_List.find(filename);
	if(iter == mFileName_EntityName_List.end())
	{
		return EMPTY_STRING_VECTOR;
	}
	return iter->second;
}

const std::string& txEntityManager::GetFileNameByEntityName(const std::string& entityName)
{
	auto iter = mEntityName_FileName_List.find(entityName);
	if(iter == mEntityName_FileName_List.end())
	{
		ENGINE_ERROR("error : can not find entity : %s, maybe it is a manual created entity!", entityName.c_str());
		return EMPTY_STRING;
	}
	return iter->second;
}

bool txEntityManager::DestroyEntity(const std::string& name)
{
	auto iter = mEntityList.find(name);
	if(iter == mEntityList.end())
	{
		ENGINE_ERROR("error : can not find Entity : %s, can not destroy the entity!", name.c_str());
		return false;
	}
	TRACE_DELETE(iter->second);
	mEntityList.erase(iter);
	removeFromNameList(name);
	return true;
}

bool txEntityManager::ClearEntity(const std::string& name)
{
	txEntity* entity = GetEntity(name);
	if (entity == NULL)
	{
		return false;
	}
	entity->clear();
	removeFromNameList(name);
	return true;
}

void txEntityManager::DestroyAllEntity()
{
	// 销毁所有实体
	auto iter = mEntityList.begin();
	auto iterEnd = mEntityList.end();
	FOR_STL(mEntityList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mEntityList);
	mEntityList.clear();

	// 清空名称列表
	mFileName_EntityName_List.clear();
	mEntityName_FileName_List.clear();
}

void txEntityManager::removeFromNameList(const std::string& name)
{
	// 从名称列表中删除,如果找不到就不删除
	auto iterEntityName = mEntityName_FileName_List.find(name);
	if (iterEntityName != mEntityName_FileName_List.end())
	{
		const std::string& filename = iterEntityName->second;
		auto iterFileName = mFileName_EntityName_List.find(filename);
		// 由于是使用的引用,所以需要查找完后再删除
		mEntityName_FileName_List.erase(iterEntityName);
		auto& fileList = iterFileName->second;
		auto iterFileEntityName = fileList.begin();
		auto iterFileEntityNameEnd = fileList.end();
		FOR_STL(fileList, ; iterFileEntityName != iterFileEntityNameEnd; ++iterFileEntityName)
		{
			// 在实体名称列表找到该名称,然后删除
			if (*iterFileEntityName == name)
			{
				fileList.erase(iterFileEntityName, false);
				break;
			}
		}
		END_FOR_STL(fileList);
		// 如果该文件名的实体列表已经空了,则删除该文件名
		if (fileList.size() == 0)
		{
			mFileName_EntityName_List.erase(iterFileName);
		}
	}
}