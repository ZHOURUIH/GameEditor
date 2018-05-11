#ifndef _TX_COMPONENT_OWNER_H_
#define _TX_COMPONENT_OWNER_H_

#include "txCommandReceiver.h"

class txProperty;
class txComponent;
class txComponentOwner : public txCommandReceiver
{
public:
	txComponentOwner(const std::string& name)
		:
		txCommandReceiver(name)
	{}
	virtual ~txComponentOwner(){ destroyAllComponents(); }
	virtual void initComponents() = 0;
	virtual void updatePreComponent(float elapsedTime);		// 更新需要最先更新的组件
	virtual void updateComponents(float elapsedTime);		// 更新正常更新的组件
	virtual void notifyAddComponent(txComponent* component){}
	// 通知布局窗口断开了与布局的联系,由窗口发出
	virtual void notifyComponentDetached(txComponent* component){ removeComponentFromList(component); }
	// 通知布局窗口建立了与布局的联系,由窗口发出
	virtual void notifyComponentAttached(txComponent* component);
	// 如果派生类想要在组件属性改变时得到通知,则只需要重写该虚函数即可	
	virtual void setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue);
	virtual bool notifyComponentNameChanged(const std::string& oldName, txComponent* component);
	void setComponentProperty(const std::string& componentName, const std::string& propertyName, const std::string& propertyValue);
	void setComponentProperty(const std::string& componentName, txProperty* prop, const std::string& propertyValue);
	void setComponentProperty(txComponent* component, const std::string& propertyName, const std::string& propertyValue);
	static txComponent* createIndependentComponent(const std::string& name, const std::string& type, bool initComponent = true);
	static txComponent* createIndependentComponent(const std::string& name, txComponent* component, bool initComponent = true);
	txComponent* addComponent(const std::string& name, const std::string& type);
	static void destroyComponent(txComponent* component);
	virtual void destroyComponent(const std::string& name);
	void destroyAllComponents();
	// 得到组件在父组件(有父组件)或者组件拥有者(第一级组件)中的位置
	int getComponentPosition(txComponent* component);
	txComponent* getComponent(const std::string& name)
	{
		auto itrFind = mAllComponentList.find(name);
		if (itrFind == mAllComponentList.end())
		{
			return NULL;
		}
		return itrFind->second;
	}
	txMap<std::string, txComponent*>& getComponentsByType(const std::string& type)
	{
		auto iterType = mAllComponentTypeList.find(type);
		if (iterType != mAllComponentTypeList.end())
		{
			return iterType->second;
		}
		return EMPTY_COMPONENT_MAP;
	}
	txMap<std::string, txComponent*>& getComponentsByBaseType(const std::string& type)
	{
		auto iterType = mAllComponentBaseTypeList.find(type);
		if (iterType != mAllComponentBaseTypeList.end())
		{
			return iterType->second;
		}
		return EMPTY_COMPONENT_MAP;
	}
	txComponent* getFirstActiveComponentByBaseType(const std::string& type);
	txComponent* getFirstComponentByBaseType(const std::string& type)
	{
		auto iter = mAllComponentBaseTypeList.find(type);
		if (iter != mAllComponentBaseTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	txComponent* getFirstActiveComponent(const std::string& type);
	txComponent* getFirstComponent(const std::string& type)
	{
		auto iter = mAllComponentTypeList.find(type);
		if (iter != mAllComponentTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	virtual void notifyComponentDestroied(txComponent* component)
	{
		removeComponentFromList(component);
	}
	txMap<std::string, txComponent*>& getAllComponent() { return mAllComponentList; }
	txMap<std::string, txMap<std::string, txComponent*> >& getComponentTypeList() { return mAllComponentTypeList; }
	txVector<txComponent*>& getRootComponentList() { return mRootComponentList; }
protected:
	void addComponentToList(txComponent* component, int componentPos = -1);
	void removeComponentFromList(txComponent* component);
protected:
	txVector<txComponent*> mRootComponentList;			// 一级组件列表,保存着组件之间的更新顺序
	txMap<std::string, txComponent*> mAllComponentList;	// 组件拥有者当前的所有组件列表
	txMap<std::string, txMap<std::string, txComponent*> > mAllComponentTypeList;		// 组件类型列表,first时组件的类型名
	txMap<std::string, txMap<std::string, txComponent*> > mAllComponentBaseTypeList;	// 根据组件的基础类型分组的组件列表,first是基础组件类型名
	static txMap<std::string, txComponent*> EMPTY_COMPONENT_MAP;
};

#endif