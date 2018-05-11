#include "WindowHeader.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "WindowProperty.h"

ComplexWindow::ComplexWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	Window(name, type)
{
	mIsComplexWindow = true;
}
ComplexWindow::~ComplexWindow()
{
	destroy();
}
void ComplexWindow::render()
{
	// 先渲染内部子窗口
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		if (mInnerChildList[i]->isVisible())
		{
			mInnerChildList[i]->render();
		}
	}
	END_FOR_STL(mInnerChildList);
	// 然后渲染子窗口
	Window::render();
}
void ComplexWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}
	// 先调用基类的更新
	Window::update(elapsedTime);
	// 再更新内部子窗口
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->update(elapsedTime);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::destroy()
{
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mInnerChildList);

	mInnerChildList.clear();
	mInnerChildMap.clear();
}
void ComplexWindow::copyProperty(Window* window)
{
	// 先调用基类函数
	Window::copyProperty(window);
	// 还需要将自己所有的内部子窗口都拷贝到目标窗口中
	ComplexWindow* destComplexWindow = static_cast<ComplexWindow*>(window);
	// 拷贝的窗口一定是同类型的窗口,那么子窗口的顺序也相同
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->copyProperty(destComplexWindow->mInnerChildList[i]);
		copyChildProperty(mInnerChildList[i], destComplexWindow->mInnerChildList[i]);
	}
	END_FOR_STL(mInnerChildList);
}
bool ComplexWindow::rename(const std::string& newName)
{
	if (!Window::rename(newName))
	{
		return false;
	}
	// 将自己内部子窗口的名字也一起修改
	renameInnerChildren(mName);
	return true;
}
Window* ComplexWindow::getChild(const std::string& name)
{
	// 先获得普通的子窗口
	Window* child = Window::getChild(name);
	// 如果找不到,则查找第一级的内部子窗口
	if (child != NULL)
	{
		return child;
	}
	else
	{
		auto iterInner = mInnerChildMap.find(name);
		if (iterInner != mInnerChildMap.end())
		{
			return iterInner->second;
		}
	}
	return NULL;
}
void ComplexWindow::detachLayoutParent(const bool& detachLayoutOnly)
{
	// 先调用基类的函数
	Window::detachLayoutParent(detachLayoutOnly);
	// 使自己所有的子窗口都断开与布局的联系,但是不能打断子窗口的父子关系
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->detachLayoutParent(true);
	}
	END_FOR_STL(mInnerChildList);
}
// 建立与布局和父窗口的联系,使该窗口成为布局中的一个窗口,该窗口下的所有子窗口也会重建与布局的联系,父子关系仍然存在
void ComplexWindow::attachLayoutParent(Layout* layout, Window* parent, const int& childPos)
{
	// 先调用基类的函数
	Window::attachLayoutParent(layout, parent, childPos);
	// 使自己所有的子窗口都建立与布局的联系
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->attachLayoutParent(layout, NULL, -1);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::setDirty(const bool& dirty, const bool& refreshNow)
{
	Window::setDirty(dirty, refreshNow);
	if (dirty)
	{
		int innerChildCount = mInnerChildList.size();
		FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
		{
			mInnerChildList[i]->setDirty(dirty, refreshNow);
		}
		END_FOR_STL(mInnerChildList);
	}
}
void ComplexWindow::getUsedResourceList(txVector<std::string>& resourceList)
{
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->getUsedResourceList(resourceList);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::removeChild(const std::string& name)
{
	Window::removeChild(name);

	Window* window = NULL;
	auto iterMap = mInnerChildMap.find(name);
	if (iterMap != mInnerChildMap.end())
	{
		window = iterMap->second;
		mInnerChildMap.erase(iterMap);
	}

	if (window == NULL)
	{
		return;
	}

	auto iter = mInnerChildList.begin();
	auto iterEnd = mInnerChildList.end();
	FOR_STL(mInnerChildList, ; iter != iterEnd; ++iter)
	{
		if (*iter == window)
		{
			mInnerChildList.erase(iter, false);
			break;
		}
	}
	END_FOR_STL(mInnerChildList);
}

void ComplexWindow::notifyChildNameChanged(const std::string& oldName, Window* window)
{
	Window::notifyChildNameChanged(oldName, window);

	// 不判断内部子窗口改名失败的情况
	auto iterMap = mInnerChildMap.find(oldName);
	if (iterMap != mInnerChildMap.end())
	{
		mInnerChildMap.erase(iterMap);
		mInnerChildMap.insert(window->getName(), window);
	}
}

void ComplexWindow::notifyInnerChildDestroied(const std::string& name)
{
	Window* window = NULL;
	auto iterMap = mInnerChildMap.find(name);
	if (iterMap != mInnerChildMap.end())
	{
		window = iterMap->second;
		mInnerChildMap.erase(iterMap);
	}
	// 如果查找不到子窗口,则表示销毁了的窗口并不是当前复合窗口的第一级子窗口
	// 也有可能是因为在removeChild中已经将子窗口从列表中移除了
	if (window == NULL)
	{
		return;
	}
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		if (mInnerChildList[i] == window)
		{
			mInnerChildList.erase(mInnerChildList.begin() + i, false);
			break;
		}
	}
	END_FOR_STL(mInnerChildList);
}

void ComplexWindow::saveWindow(std::string& windowStream)
{
	// 先序遍历所有窗口,先保存父窗口,然后再保存所有子窗口
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	if (mParent != NULL)
	{
		txStringUtility::jsonAddPair(windowStream, PARENT_PROPERTY_NAME, mParent->getName(), 2, true);
	}
	// 遍历自己的所有属性
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	// 保存自己的内部子窗口
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->saveWindow(windowStream);
	}
	END_FOR_STL(mInnerChildList);
	// 如果没有关联任何预设,则保存子窗口信息
	if (mPrefabName == EMPTY_STRING)
	{
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			mChildren[i]->saveWindow(windowStream);
		}
		END_FOR_STL(mChildren);
	}
}

void ComplexWindow::saveTemplate(std::string& windowStream)
{
	// 先序遍历所有窗口,先保存父窗口,然后再保存所有子窗口
	txStringUtility::jsonStartArray(windowStream, 0, true);
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	// 遍历自己的所有属性
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	// 保存自己的内部子窗口
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->saveWindow(windowStream);
	}
	END_FOR_STL(mInnerChildList);
	txStringUtility::jsonEndArray(windowStream, 0, true);
}

Window* ComplexWindow::createInnerChild(const std::string& windowType, const std::string& name)
{
	Window* window = NULL;
	if (mParentLayout != NULL)
	{
		window = mParentLayout->createWindow(windowType, name);
	}
	else
	{
		window = Layout::createIndependentWindow(windowType, name);
	}
	// 标记为内部子窗口,并且必须忽略输入事件
	window->setInnerChild(true);
	window->setEnableHandleInput(false);
	window->setComplexParent(this);
	return window;
}
void ComplexWindow::copyChildProperty(Window* sourceWindow, Window* destWindow)
{
	// 拷贝子窗口
	auto& destChildren = destWindow->getChildren();
	auto& sourceChildren = sourceWindow->getChildren();
	int childCount = sourceChildren.size();
	FOR_STL(sourceChildren, int i = 0; i < childCount; ++i)
	{
		// 先拷贝自己的属性
		sourceChildren[i]->copyProperty(destChildren[i]);
		// 然后再拷贝子窗口的属性
		copyChildProperty(sourceChildren[i], destChildren[i]);
	}
	END_FOR_STL(sourceChildren);
}
void ComplexWindow::addInnerChild(Window* child)
{
	child->setParent(this);
	mInnerChildMap.insert(child->getName(), child);
	mInnerChildList.push_back(child);
	notifyChildAdded();
}