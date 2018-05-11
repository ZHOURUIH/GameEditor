#ifndef _TX_COMPONENT_EFFECT_H_
#define _TX_COMPONENT_EFFECT_H_

#include "txComponent.h"

class txNode;
class txEntity;
class txObjectBoneHook;
class txComponentEffect : public txComponent
{
public:
	txComponentEffect(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mHook(NULL)
	{}
	virtual ~txComponentEffect(){ destroy(); }
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentEffect); }
	void destroy();
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){return type == TOSTRING(txComponentEffect);}
	virtual void setActive(const bool& active)
	{
		txComponent::setActive(active);
		if (!active)
		{
			stop();
		}
	}
	void setRotate(const MATRIX3& rotate);
	void setPosition(const VECTOR3& translate);
	void setScale(const VECTOR3& scale);
	// 参数1是骨头名,参数2是角色组件名
	bool createHook(const std::string& boneName, txEntity* hookEntity);
	// 参数1时粒子的文件名,不带路径
	// 参数2是粒子节点的父节点,只有当粒子没有挂接到任何挂点上时才有效,否则粒子节点的父节点是挂点所在模型的节点
	bool createEffect(const std::string& fileName, txNode* parentNode);
	void SetEmittersUpdateMode(const bool& loop);
	void play();
	void stop();
	const std::string& getEffectFileName() { return mEffectFileName; }
protected:
	std::string mEffectFileName;
	txObjectBoneHook* mHook;
};

#endif