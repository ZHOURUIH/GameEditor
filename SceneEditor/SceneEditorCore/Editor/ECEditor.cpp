#include "txComponent.h"
#include "txProperty.h"

#include "ECEditor.h"
#include "ECEditorManager.h"
#include "ECScene.h"

void ECEditor::setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue)
{
	txMovableObject::setComponentProperty(component, prop, propertyValue);

	// 通知场景编辑体组件属性改变
	if (mScene != NULL)
	{
		mScene->notifyEditorComponentProertyChanged(component, prop->getName(), propertyValue);
	}
}

void ECEditor::destroyComponent(const std::string& name)
{
	txComponent* component = getComponent(name);
	// 通知场景编辑体删除了组件
	if (mScene != NULL)
	{
		mScene->notifyEditorDeleteComponent(component);
	}
	txMovableObject::destroyComponent(name);
}

bool ECEditor::rename(const std::string& newName)
{
	if (newName == mName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	if (!mEditorManager->notifyEditorRenamed(oldName, this))
	{
		mName = oldName;
		return false;
	}
	// 不需要再判断场景是否允许改名
	mScene->notifyEditorRenamed(oldName, this);
	return true;
}

void ECEditor::saveEditor(std::string& stream, const int& tableCount)
{
	txStringUtility::jsonStartArray(stream, tableCount, true);
	txStringUtility::jsonStartStruct(stream, tableCount + 1, true);
	// 名字
	txStringUtility::jsonAddPair(stream, "name", mName, tableCount + 2, true);
	txStringUtility::jsonEndStruct(stream, tableCount + 1, true);
	// 各个组件的属性
	int rootComponentCount = mRootComponentList.size();
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		saveComponent(mRootComponentList[i], stream, tableCount + 1);
	}
	END_FOR_STL(mRootComponentList);
	txStringUtility::jsonEndArray(stream, tableCount, true);
}

void ECEditor::saveComponent(txComponent* component, std::string& stream, const int& tableCount)
{
	// 首先保存组件自己的属性
	component->saveProperty(stream, tableCount);
	// 然后保存子组件的属性
	txVector<txComponent*>& childList = component->getChildComponentList();
	int childCount = childList.size();
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		saveComponent(childList[i], stream, tableCount);
	}
	END_FOR_STL(childList);
}