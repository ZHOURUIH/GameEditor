#ifndef _TX_ENTITY_MANAGER_H_
#define _TX_ENTITY_MANAGER_H_

#include "txEngineDefine.h"

class txMesh;
class txMaterial;
class txEntity;
class txTexture;
class txEntityManager
{
public:
	txEntityManager();
	virtual ~txEntityManager();
	virtual void init(){}

	// 创建一个空的实体
	txEntity* CreateEmptyEntity(const std::string& name);
	// 创建模型,name是创建的模型的名字,filename是该模型的文件,包含相对路径,hasAnimation是该模型是否带动作
	// copyType指定了当同文件的实体已经存在时,创建新的实体的拷贝方式
	// loadTexture是指定是否加载贴图
	// texture返回创建出来的纹理指针
	txEntity* CreateEntity(const std::string& name, const std::string& filename, const bool& hasAnimation, const ENTITY_COPY_TYPE& copyType, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);

	// 初始化实体,实体必须是一个已经存在的空实体
	bool initEntity(txEntity* entity, const std::string& filename, const bool& hasAnimation, const ENTITY_COPY_TYPE& copyType, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);

	// 通过一个已经初始化的实体来初始化一个空的实体
	bool initWithExistEntity(txEntity* entity, txEntity* sourceEntity, const ENTITY_COPY_TYPE& copyType);

	// 根据已存在的实体创建一个相同的实体
	txEntity* CreateWithExistEntity(const std::string& name, txEntity* sourceEntity, const ENTITY_COPY_TYPE& copyType);

	// 手动创建模型,只能是静态模型,pMesh是已经包含了模型数据的对象
	txEntity* CreateManualEntity(const std::string& name, txMesh* pMesh);

	// 根据实体名称得到实体对象,实体名称是唯一
	txEntity* GetEntity(const std::string& name);

	// 根据文件名得到第一个该文件名的实体,一个文件名可能会创建多个不同名称的实体
	txEntity* GetFirstEntityByFileName(const std::string& filename);

	// 根据文件名得到所有由该文件创建的实体名,包括由拷贝实体而创建的实体
	const txVector<std::string>& GetEntityNamesByFileName(const std::string& filename);

	// 根据实体名称得到该实体的文件名
	const std::string& GetFileNameByEntityName(const std::string& entityName);

	// 销毁指定名称的实体
	bool DestroyEntity(const std::string& name);

	// 清空一个实体的数据,使之成为一个空的实体
	bool ClearEntity(const std::string& name);

	// 销毁所有实体
	void DestroyAllEntity();
protected:
	// 将实体的名字从名称查找列表中移除
	void removeFromNameList(const std::string& name);
protected:
	static txVector<std::string> EMPTY_STRING_VECTOR;
	// txEntity的列表,first是实体名,不能创建相同名字的实体
	txMap<std::string, txEntity*> mEntityList;

	// 以下两个map是用于实体创建时的实体名和文件名互相查找的
	// first是文件名,带相对路径的,second是该文件名所创建的实体名称列表
	txMap<std::string, txVector<std::string> > mFileName_EntityName_List;

	// first是实体名称,second是该实体名称的文件名,带相对路径的
	txMap<std::string, std::string> mEntityName_FileName_List;
};

#endif