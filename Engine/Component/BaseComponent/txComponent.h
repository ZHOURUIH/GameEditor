#ifndef _TX_COMPONENT_H_
#define _TX_COMPONENT_H_

#include "txMathUtility.h"
#include "txPropertyReceiver.h"
#include "txEngineBase.h"

class txComponentOwner;
class txNode;
class txComponent : public txEngineBase, public txPropertyReceiver
{
public:
	txComponent(const std::string& typeName, const std::string& name);
	virtual ~txComponent(){ destroy(); }
	virtual void init(txComponentOwner* owner)
	{
		mComponentOwner = owner;
		// setBaseType不能在构造中调用,因为构造时对象还没有完全创建出,所以无法正确调用派生类的函数
		setBaseType();
		initReceiver();
	}
	virtual void initProperty();
	virtual void setBaseType() = 0;
	virtual void preUpdate(const float& elapsedTime);
	virtual void update(const float& elapsedTime);
	virtual void lateUpdate(const float& elapsedTime);
	virtual bool isType(const std::string& type) = 0;	// 判断该组件是否是指定类型的,包括所有继承的类型
	void destroy();
	void saveProperty(std::string& stream, const int& tableCount);
	bool rename(const std::string& newName);
	bool addChild(txComponent* component);		// 将一个组件添加作为当前组件的子组件
	bool removeChild(txComponent* component);	// 将一个组件从当前组件的子组件列表中移除
	void copyProperty(txComponent* destComponent);	// 拷贝当前组件的所有属性到目标组件中,返回值表示当前组件是否已经链接了预设
	bool isActive();
	// 断开与拥有者和父组件的联系,使该组件成为一个独立的组件,该组件下的所有子组件也会断开与拥有者的联系,但是父子关系仍然存在
	// detachOwnerOnly表示是否只断开与拥有者的联系,不断开组件之间的父子关系,外部使用时一般填false
	virtual void detachOwnerParentComponent(const bool& detachOwnerOnly = false);
	// 建立与拥有者和父组件的联系,使该组件成为拥有者中的一个组件,该组件下的所有子组件也会重建与拥有者的联系,父子关系仍然存在
	virtual void attachOwnerParentComponent(txComponentOwner* owner, txComponent* parent, const int& childPos);
	int getChildPos(txComponent* component);					// 得到指定组件在当前组件中的位置
	bool moveChildPos(txComponent* component, const int& destPos);		// 将指定组件移动到当前组件中的指定位置
	bool moveChildPos(const std::string& name, const int& destPos);
	virtual void setParentComponent(txComponent* component);
	txComponent* getChildComponent(const std::string& childName)
	{
		auto iter = mChildComponentMap.find(childName);
		if (iter != mChildComponentMap.end())
		{
			return iter->second;
		}
		return NULL;
	}

	// 设置成员变量
	virtual void setActive(const bool& active)	{ mActive = active; }
	void setLockOneFrame(const bool& lock)		{ mLockOneFrame = lock; }
	void setPreUpdate(const bool& pre)			{ mPreUpdate = pre; }

	// 获得成员变量
	txComponentOwner* getOwner()		{ return mComponentOwner; }
	txComponent* getParentComponent()	{ return mParent; }
	const std::string& getType()		{ return mType; }
	const std::string& getBaseType()	{ return mBaseType; }
	const std::string& getName()		{ return mName; }
	const bool& isLockOneFrame()		{ return mLockOneFrame; }
	const bool& isComponentActive()		{ return mActive; }		// 组件自己是否激活,不考虑父组件
	const bool& isPreUpdate()			{ return mPreUpdate; }
	txNode* getNode()					{ return mNode; }
	txVector<txComponent*>& getChildComponentList() { return mChildComponentList; }
	txMap<std::string, txComponent*>& getChildComponentMap() { return mChildComponentMap; }

	// 通知
	virtual void notifyParentDestroied(){ mParent = NULL; }
	virtual void notifyChildDestroied(txComponent* component){ removeChild(component); }
	virtual void notifyChildDetached(txComponent* component){ removeChild(component); }	// 通知该组件有子组件断开了联系
	virtual void notifyChildNameChanged(const std::string& oldName, txComponent* component);
protected:
	virtual void clearCallback(){}
protected:
	txComponentOwner* mComponentOwner;	// 该组件的拥有者
	txNode* mNode;						// 组件节点
	std::string mBaseType;				// 基础组件类型,指定该组件属于什么基础类型的组件,如果不属于任何基础类型,则填实际组件类型
	std::string mType;					// 实际组件类型
	std::string mName;					// 组件名
	bool mActive;						// 是否激活组件
	bool mLockOneFrame;					// 是否将组件锁定一次
	bool mPreUpdate;					// 是否为预更新组件
	txComponent* mParent;				// 父级组件
	txMap<std::string, txComponent*> mChildComponentMap;	// 用于查找组件
	txVector<txComponent*> mChildComponentList;			// 该组件下的子组件列表,保存了子组件之间的顺序
};

#endif