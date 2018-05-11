#include "txComponentHeader.h"
#include "txComponentFactory.h"
#include "txEngineLog.h"
#include "txComponentFactoryManager.h"

txMap<std::string, txComponent*> txComponentOwner::EMPTY_COMPONENT_MAP;

void txComponentOwner::updatePreComponent(float elapsedTime)
{
	if (mAllComponentList.size() == 0)
	{
		return;
	}
	int rootComponentCount = mRootComponentList.size();
	// 预更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (component->isPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->preUpdate(elapsedTime);
			}
		}
	}
	END_FOR_STL(mRootComponentList);
	// 更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (component->isPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->update(elapsedTime);
			}
		}
	}
	END_FOR_STL(mRootComponentList);
	// 补充更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (component->isPreUpdate())
		{
			if (component != NULL && component->isActive())
			{
				// 如果组件被锁定了一帧,则不更新,解除锁定
				if (component->isLockOneFrame())
				{
					component->setLockOneFrame(false);
				}
				else
				{
					component->lateUpdate(elapsedTime);
				}
			}
		}
	}
	END_FOR_STL(mRootComponentList);
}

void txComponentOwner::updateComponents(float elapsedTime)
{
	if (mAllComponentList.size() == 0)
	{
		return;
	}
	int rootComponentCount = mRootComponentList.size();
	// 预更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (!component->isPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->preUpdate(elapsedTime);
			}
		}
	}
	END_FOR_STL(mRootComponentList);
	// 更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (!component->isPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->update(elapsedTime);
			}
		}
	}
	END_FOR_STL(mRootComponentList);
	// 补充更新基础类型组件
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (!component->isPreUpdate())
		{
			if (component != NULL && component->isActive())
			{
				// 如果组件被锁定了一帧,则不更新,解除锁定
				if (component->isLockOneFrame())
				{
					component->setLockOneFrame(false);
				}
				else
				{
					component->lateUpdate(elapsedTime);
				}
			}
		}
	}
	END_FOR_STL(mRootComponentList);
}

void txComponentOwner::setComponentProperty(const std::string& componentName, const std::string& propertyName, const std::string& propertyValue)
{
	txComponent* component = getComponent(componentName);
	if (component != NULL)
	{
		txProperty* prop = component->getProperty(propertyName);
		if (prop != NULL)
		{
			setComponentProperty(component, prop, propertyValue);
		}
	}
}

void txComponentOwner::setComponentProperty(const std::string& componentName, txProperty* prop, const std::string& propertyValue)
{
	txComponent* component = getComponent(componentName);
	if (component != NULL)
	{
		setComponentProperty(component, prop, propertyValue);
	}
}

void txComponentOwner::setComponentProperty(txComponent* component, const std::string& propertyName, const std::string& propertyValue)
{
	txProperty* prop = component->getProperty(propertyName);
	if (prop != NULL)
	{
		setComponentProperty(component, prop, propertyValue);
	}
}

void txComponentOwner::notifyComponentAttached(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	auto iter = mAllComponentList.find(component->getName());
	if (iter == mAllComponentList.end())
	{
		addComponentToList(component);
	}
}

void txComponentOwner::setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue)
{
	component->setPropertyValue(prop, propertyValue);
}

bool txComponentOwner::notifyComponentNameChanged(const std::string& oldName, txComponent* component)
{
	// 先查找是否有该名字的组件
	auto it = mAllComponentList.find(oldName);
	if (it == mAllComponentList.end())
	{
		return false;
	}
	// 再查找改名后会不会重名
	auto itNew = mAllComponentList.find(component->getName());
	if (itNew != mAllComponentList.end())
	{
		return false;
	}
	removeComponentFromList(component);
	addComponentToList(component);
	return true;
}

txComponent* txComponentOwner::createIndependentComponent(const std::string& name, const std::string& type, bool initComponent)
{
	// 查找工厂
	txComponentFactoryBase* factory = mComponentFactoryManager->getFactory(type);
	if (factory == NULL)
	{
		ENGINE_ERROR("error : can not find component factory! type : %s", type.c_str());
		return NULL;
	}
	// 创建组件并且设置拥有者,然后初始化
	txComponent* component = factory->createComponent(name);
	if (initComponent && component != NULL)
	{
		component->init(NULL);
	}
	return component;
}

txComponent* txComponentOwner::createIndependentComponent(const std::string& name, txComponent* sourceComponent, bool initComponent)
{
	txComponent* component = createIndependentComponent(name, sourceComponent->getType(), initComponent);
	if (component == NULL)
	{
		return NULL;
	}
	sourceComponent->copyProperty(component);
	return component;
}

txComponent* txComponentOwner::addComponent(const std::string& name, const std::string& type)
{
	// 不能创建重名的组件
	if (mAllComponentList.find(name) != mAllComponentList.end())
	{
		ENGINE_ERROR("error : there is component named : %s in the list", name.c_str());
		return NULL;
	}
	txComponent* component = createIndependentComponent(name, type, false);
	if (component == NULL)
	{
		return NULL;
	}
	component->init(this);
	// 将组件加入列表
	addComponentToList(component);
	// 通知创建了组件
	notifyAddComponent(component);
	return component;
}

void txComponentOwner::destroyComponent(txComponent* component)
{
	// 后序遍历销毁组件,从最底层组件开始销毁,此处不能用引用获得子组件列表,因为在销毁组件过程中会对列表进行修改
	auto children = component->getChildComponentList();
	int childCount = children.size();
	FOR_STL(children, int i = 0; i < childCount; ++i)
	{
		destroyComponent(children[i]);
	}
	END_FOR_STL(children);
	mComponentFactoryManager->getFactory(component->getType())->destroyComponent(component);
}

void txComponentOwner::destroyComponent(const std::string& name)
{
	// 在总列表中查找
	auto itrFind = mAllComponentList.find(name);
	if (itrFind == mAllComponentList.end())
	{
		return;
	}
	destroyComponent(itrFind->second);
}

void txComponentOwner::destroyAllComponents()
{
	auto iterType = mAllComponentTypeList.begin();
	auto iterTypeEnd = mAllComponentTypeList.end();
	FOR_STL(mAllComponentTypeList, ; iterType != iterTypeEnd; ++iterType)
	{
		txComponentFactoryBase* factory = mComponentFactoryManager->getFactory(iterType->first);
		if (factory == NULL)
		{
			continue;
		}
		// 因为在销毁过程中会修改列表,复制一份是为了避免迭代器失效
		auto componentList = iterType->second;
		auto iterCom = componentList.begin();
		auto iterComEnd = componentList.end();
		FOR_STL(componentList, ; iterCom != iterComEnd; ++iterCom)
		{
			factory->destroyComponent(iterCom->second);
		}
		END_FOR_STL(componentList);
	}
	END_FOR_STL(mAllComponentTypeList);
}

int txComponentOwner::getComponentPosition(txComponent* component)
{
	if (component == NULL)
	{
		return -1;
	}
	if (component->getParentComponent() != NULL)
	{
		return component->getParentComponent()->getChildPos(component);
	}
	else
	{
		int pos = -1;
		// 首先查找当前窗口的位置
		int childCount = mRootComponentList.size();
		FOR_STL(mRootComponentList, int i = 0; i < childCount; ++i)
		{
			if (component == mRootComponentList[i])
			{
				pos = i;
				break;
			}
		}
		END_FOR_STL(mRootComponentList);
		return pos;
	}
	return -1;
}

txComponent* txComponentOwner::getFirstActiveComponentByBaseType(const std::string& type)
{
	txComponent* com = NULL;
	auto iterBaseType = mAllComponentBaseTypeList.find(type);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		auto iterTypeCom = iterBaseType->second.begin();
		auto iterTypeComEnd = iterBaseType->second.end();
		FOR_STL(iterBaseType->second, ; iterTypeCom != iterTypeComEnd; ++iterTypeCom)
		{
			txComponent* component = iterTypeCom->second;
			if (component->isActive() && !component->isLockOneFrame())
			{
				com = component;
				break;
			}
		}
		END_FOR_STL(iterBaseType->second);
	}
	return com;
}

txComponent* txComponentOwner::getFirstActiveComponent(const std::string& type)
{
	txComponent* com = NULL;
	auto iter = mAllComponentTypeList.find(type);
	if (iter != mAllComponentTypeList.end())
	{
		auto iterTypeCom = iter->second.begin();
		auto iterTypeComEnd = iter->second.end();
		FOR_STL(iter->second, ; iterTypeCom != iterTypeComEnd; ++iterTypeCom)
		{
			txComponent* component = iterTypeCom->second;
			if (component->isActive() && !component->isLockOneFrame())
			{
				com = component;
				break;
			}
		}
		END_FOR_STL(iter->second);
	}
	return com;
}

void txComponentOwner::addComponentToList(txComponent* component, int componentPos)
{
	const std::string& name = component->getName();
	const std::string& type = component->getType();
	const std::string& baseType = component->getBaseType();

	// 如果没有父组件,则将组件放入第一级组件列表中
	if (component->getParentComponent() == NULL)
	{
		if (componentPos == -1)
		{
			mRootComponentList.push_back(component);
		}
		else
		{
			mRootComponentList.insert(mRootComponentList.begin() + componentPos, component);
		}
	}

	// 添加到组件列表中
	mAllComponentList.insert(name, component);

	// 添加到组件类型分组列表中
	auto iterType = mAllComponentTypeList.find(type);
	if (iterType != mAllComponentTypeList.end())
	{
		iterType->second.insert(name, component);
	}
	else
	{
		txMap<std::string, txComponent*> componentList;
		componentList.insert(name, component);
		mAllComponentTypeList.insert(type, componentList);
	}

	// 添加到基础组件类型分组列表中
	auto iterBaseType = mAllComponentBaseTypeList.find(baseType);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		iterBaseType->second.insert(name, component);
	}
	else
	{
		txMap<std::string, txComponent*> componentList;
		componentList.insert(name, component);
		mAllComponentBaseTypeList.insert(baseType, componentList);
	}
}

void txComponentOwner::removeComponentFromList(txComponent* component)
{
	// 从第一级组件列表中移除
	if (component->getParentComponent() == NULL)
	{
		int componentPos = -1;
		int componentCount = mRootComponentList.size();
		FOR_STL(mRootComponentList, int i = 0; i < componentCount; ++i)
		{
			if (mRootComponentList[i] == component)
			{
				componentPos = i;
				break;
			}
		}
		END_FOR_STL(mRootComponentList);
		if (componentPos >= 0)
		{
			mRootComponentList.erase(mRootComponentList.begin() + componentPos);
		}
	}

	// 从所有组件列表中移除
	const std::string& componentName = component->getName();
	auto iterCom = mAllComponentList.find(componentName);
	if (iterCom != mAllComponentList.end())
	{
		mAllComponentList.erase(iterCom);
	}

	// 从组件类型分组列表中移除
	const std::string& realType = component->getType();
	auto iterType = mAllComponentTypeList.find(realType);
	if (iterType != mAllComponentTypeList.end())
	{
		auto iterCom = iterType->second.find(componentName);
		if (iterCom != iterType->second.end())
		{
			iterType->second.erase(iterCom);
		}
	}

	// 从基础组件类型分组列表中移除
	const std::string& baseType = component->getBaseType();
	auto iterBaseType = mAllComponentBaseTypeList.find(baseType);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		auto iter = iterBaseType->second.find(componentName);
		if (iter != iterBaseType->second.end())
		{
			iterBaseType->second.erase(iter);
		}
	}
}