#ifndef _TX_COMPONENT_MESH_H_
#define _TX_COMPONENT_MESH_H_

#include "txAABox.h"
#include "txComponent.h"

class txNode;
class txEntity;
class txObjectBoneHook;
// 模型组件
class txComponentMesh : public txComponent
{
public:
	txComponentMesh(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mEntity(NULL),
		mIsOutEntity(false),
		mHook(NULL)
	{}
	virtual ~txComponentMesh(){ destroy(); }
	virtual void setBaseType() { mBaseType = TOSTRING(txComponentMesh); }
	virtual void initProperty();
	virtual void init(txComponentOwner* owner);
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentMesh); }
	void destroy(){ destroyEntity(); }
	void destroyEntity();
	virtual bool initEntityByFileName(const std::string& filePath, const bool& hasAnim, const bool& loadTexture);
	virtual bool initEntityByEntityName(const std::string& sourceEntityName, const bool& hasAnim, const bool& loadTexture);
	// resetNode为是否重新设置父节点, 如果重新设置,则将实体节点的父节点设置为parentNode
	virtual bool initEntityByOutEntity(const std::string& sourceEntityName, const bool& resetNode);
	const std::string& getModelFileName() { return mModelFileName; }
	txEntity* getEntity(){ return mEntity; }
	virtual void setActive(const bool& active);
	void generateTexture();
	const VECTOR3& getPosition();
	const VECTOR3& getScale();
	const MATRIX3& getRotation();
	void setPosition(const VECTOR3& pos);
	void setScale(const VECTOR3& scale);
	void setRotation(const MATRIX3& rot);
	void resetRotation();
	virtual txAABox getWorldAABB();
	virtual txAABox getLocalAABB();
	void setShowAABB(const bool& show);
	bool getShowAABB();
	void roll(const float& angle);
	void pitch(const float& angle);
	void yaw(const float& angle);
	void setHook(const std::string& hookName);
protected:
	txEntity* mEntity;			// 实体
	bool mIsOutEntity;			// entity是否是由外部创建的,而不是由静态模型组件创建的
	std::string mModelFileName; // 不带路径的模型文件名
	txObjectBoneHook* mHook;	// 模型组件所挂接的挂点,只有当模型组件挂接到一个动态模型组件下才会有挂点
};

#endif