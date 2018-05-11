#include "txComponentHeader.h"
#include "txComponentProperty.h"
#include "txEngineLog.h"
#include "txNodeManager.h"
#include "txNode.h"

txComponent::txComponent(const std::string& typeName, const std::string& name)
:
mType(typeName),
mName(name),
mComponentOwner(NULL),
mActive(true),
mLockOneFrame(false),
mParent(NULL),
mNode(NULL),
mPreUpdate(false)
{}

void txComponent::initProperty()
{
	addProperty<txComponentPropertyActive>();
}

void txComponent::destroy()
{
	// 首先通知所有的子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mChildComponentList);
	mChildComponentList.clear();
	mChildComponentMap.clear();

	// 通知父组件
	if (mParent != NULL)
	{
		mParent->notifyChildDestroied(this);
		mParent = NULL;
	}

	if (mComponentOwner != NULL)
	{
		mComponentOwner->notifyComponentDestroied(this);
	}

	if (mNode != NULL)
	{
		mNodeManager->DestroyNode(mNode->GetName());
		mNode = NULL;
	}
}

void txComponent::saveProperty(std::string& stream, const int& tableCount)
{
	txStringUtility::jsonStartStruct(stream, tableCount, true);
	txStringUtility::jsonAddPair(stream, "name", mName, tableCount + 1, true);
	txStringUtility::jsonAddPair(stream, "type", mType, tableCount + 1, true);
	txStringUtility::jsonAddPair(stream, "parent", mParent != NULL ? mParent->getName() : EMPTY_STRING, tableCount + 1, true);
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(stream, iterProperty->first, iterProperty->second->get(this), tableCount + 1, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(stream, tableCount, true);
}

void txComponent::preUpdate(const float& elapsedTime)
{
	if (mLockOneFrame || !isActive())
	{
		return;
	}
	// 预更新子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->preUpdate(elapsedTime);
	}
	END_FOR_STL(mChildComponentList);
}

void txComponent::update(const float& elapsedTime)
{
	if (mLockOneFrame || !isActive())
	{
		return;
	}
	// 更新子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->update(elapsedTime);
	}
	END_FOR_STL(mChildComponentList);
}

void txComponent::lateUpdate(const float& elapsedTime)
{
	if (mLockOneFrame)
	{
		mLockOneFrame = false;
		return;
	}
	if (!isActive())
	{
		return;
	}
	// 后更新子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->lateUpdate(elapsedTime);
	}
	END_FOR_STL(mChildComponentList);
}

bool txComponent::rename(const std::string& newName)
{
	if (mName == newName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	if (mComponentOwner != NULL)
	{
		// 通知Layout自己的名字改变了
		bool ret = mComponentOwner->notifyComponentNameChanged(oldName, this);
		// 如果父窗口不允许自己改名,则需要将名字还原
		if (!ret)
		{
			mName = oldName;
			return false;
		}
	}
	// 通知父窗口自己的名字改变了
	if (NULL != mParent)
	{
		mParent->notifyChildNameChanged(oldName, this);
	}
	return true;
}

bool txComponent::addChild(txComponent* component)
{
	if (component == NULL || mChildComponentMap.find(component->getName()) != mChildComponentMap.end())
	{
		return false;
	}
	component->setParentComponent(this);
	mChildComponentList.push_back(component);
	mChildComponentMap.insert(component->getName(), component);
	return true;
}

bool txComponent::removeChild(txComponent* component)
{
	if (component == NULL)
	{
		return false;
	}
	auto iter = mChildComponentMap.find(component->getName());
	if (iter == mChildComponentMap.end())
	{
		return false;
	}
	mChildComponentMap.erase(iter);
	int compoentPos = -1;
	auto iterList = mChildComponentList.begin();
	auto iterListEnd = mChildComponentList.end();
	FOR_STL(mChildComponentList, int i = 0; iterList != iterListEnd; ++iterList)
	{
		if (*iterList == component)
		{
			compoentPos = i;
			break;
		}
		++i;
	}
	END_FOR_STL(mChildComponentList);
	if (compoentPos >= 0)
	{
		mChildComponentList.erase(mChildComponentList.begin() + compoentPos);
	}
	return true;
}

void txComponent::copyProperty(txComponent* destComponent)
{
	auto iter = mPropertyMap.begin();
	auto iterEnd = mPropertyMap.end();
	FOR_STL(mPropertyMap, ; iter != iterEnd; ++iter)
	{
		txProperty* prop = destComponent->getProperty(iter->first);
		if (prop != NULL)
		{
			prop->set(iter->second->get(this), destComponent);
		}
	}
	END_FOR_STL(mPropertyMap);
}

bool txComponent::isActive()
{
	if (mParent != NULL)
	{
		return mParent->isActive() && mActive;
	}
	return mActive;
}

void txComponent::detachOwnerParentComponent(const bool& detachOwnerOnly)
{
	if (mComponentOwner != NULL)
	{
		mComponentOwner->notifyComponentDetached(this);
		mComponentOwner = NULL;
	}
	// 如果不是只断开与组件拥有者的联系,则还需要断开与父组件的联系
	if (!detachOwnerOnly && mParent != NULL)
	{
		mParent->notifyChildDetached(this);
		setParentComponent(NULL);
	}
	// 使自己所有的子组件都断开与拥有者的联系,但是不能打断子组件的父子关系
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->detachOwnerParentComponent(true);
	}
	END_FOR_STL(mChildComponentList);
}

void txComponent::attachOwnerParentComponent(txComponentOwner* owner, txComponent* parent, const int& childPos)
{
	// 先建立与父组件的联系,避免在建立与组件拥有者的联系时被当作第一级组件
	if (parent != NULL && mParent == NULL)
	{
		parent->addChild(this);
		parent->moveChildPos(this, childPos);
	}
	if (owner != NULL && mComponentOwner == NULL)
	{
		mComponentOwner = owner;
		mComponentOwner->notifyComponentAttached(this);
		// 使自己所有的子窗口都建立与布局的联系
		int childCount = mChildComponentList.size();
		FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
		{
			mChildComponentList[i]->attachOwnerParentComponent(owner, NULL, -1);
		}
		END_FOR_STL(mChildComponentList);
	}
}

int txComponent::getChildPos(txComponent* component)
{
	if (component == NULL)
	{
		return -1;
	}
	// 首先查找当前窗口的位置
	int pos = -1;
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		if (component == mChildComponentList[i])
		{
			pos = i;
			break;
		}
	}
	END_FOR_STL(mChildComponentList);
	return pos;
}

bool txComponent::moveChildPos(txComponent* component, const int& destPos)
{
	if (component == NULL || destPos < 0 || destPos >= (int)mChildComponentList.size())
	{
		return false;
	}
	// 首先查找当前窗口的位置
	int pos = getChildPos(component);
	if (pos < 0 || pos == destPos)
	{
		return false;
	}
	mChildComponentList.erase(mChildComponentList.begin() + pos);
	mChildComponentList.insert(mChildComponentList.begin() + destPos, component);
	return true;
}

bool txComponent::moveChildPos(const std::string& name, const int& destPos)
{
	return moveChildPos(getChildComponent(name), destPos);
}

void txComponent::setParentComponent(txComponent* component)
{ 
	// 如果要去除父组件,则需要设置节点的关系
	if (component == NULL && mParent != NULL && mParent->getNode() != NULL && mNode != NULL)
	{
		mParent->getNode()->RemoveChildNode(mNode);
	}
	mParent = component;
	// 如果都有节点,则设置节点的父子关系
	if (mParent != NULL && mParent->getNode() != NULL && mNode != NULL)
	{
		mParent->getNode()->AddChildNode(mNode);
	}
}

void txComponent::notifyChildNameChanged(const std::string& oldName, txComponent* component)
{
	// 修改全部子窗口查找列表中的名字
	auto iterAll = mChildComponentMap.find(oldName);
	if (iterAll != mChildComponentMap.end())
	{
		auto iterNew = mChildComponentMap.find(component->mName);
		if (iterNew == mChildComponentMap.end())
		{
			mChildComponentMap.insert(component->mName, component);
			mChildComponentMap.erase(iterAll);
		}
		else
		{
			ENGINE_ERROR("error : there is a child named : %s!", component->mName.c_str());
		}
	}
}