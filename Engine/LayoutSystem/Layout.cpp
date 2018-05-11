#include "json/json.h"
#include "txEngineRoot.h"
#include "txBaseProperty.h"
#include "Layout.h"
#include "WindowHeader.h"
#include "LayoutManager.h"
#include "WindowFactory.h"
#include "WindowFactoryManager.h"
#include "WindowProperty.h"
#include "LayoutScript.h"
#include "LayoutScriptFactoryManager.h"
#include "LayoutScriptFactory.h"

Layout::Layout(const int& layoutID, const std::string& name)
{
	Layout(layoutID, name, EMPTY_STRING);
}

Layout::Layout(const int& layoutID, const std::string& name, const std::string& fileName)
	:
	mLayoutID(layoutID),
	mName(name),
	mFileName(fileName),
	mRootWindow(NULL),
	mDirty(true),
	mLoadDone(false),
	mScriptControlHide(false),
	mScriptInited(false),
	mDoneCallback(NULL),
	mDoneUserData(NULL)
{
	mScript = createScript();
}

Layout::~Layout()
{
	destroy();
}

bool Layout::init(const bool& async, LayoutAsyncDone callback, void* userData)
{
	if (async)
	{
		mDoneCallback = callback;
		mDoneUserData = userData;
	}
	if (mFileName != EMPTY_STRING)
	{
		if (!loadLayout(mFileName, async))
		{
			ENGINE_ERROR("error : can not create layout filename is : %s", mFileName.c_str());
			destroy();
			return false;
		}
	}
	// 如果不是异步加载,则标记加载完成
	if (!async)
	{
		mLoadDone = true;
	}
	return true;
}

LayoutScript* Layout::createScript()
{
	LayoutScriptFactoryBase* factory = mLayoutScriptFactoryManager->getFactory(mLayoutID);
	if (factory == NULL)
	{
		return NULL;
	}
	return factory->createScript(this, mName);
}

void Layout::update(float elapsedTime)
{
	// 如果还没加载完,则不更新
	if (!mLoadDone)
	{
		return;
	}
	if (mRootWindow != NULL)
	{
		mRootWindow->update(elapsedTime);
	}
	// 等待所有窗口都刷新完毕,然后判断是否需要刷新布局所占区域
	if (mDirty)
	{
		generateRect();
		mDirty = false;
	}
}

void Layout::render()
{
	// 只有加载完成时才能渲染
	if (mLoadDone && mRootWindow != NULL && mRootWindow->isVisible())
	{
		mRootWindow->render();
	}
}

void Layout::destroy()
{
	TRACE_DELETE(mScript);
	if (mRootWindow != NULL)
	{
		mLayoutManager->destroyWindow(mRootWindow);
	}
}

void Layout::generateRect()
{
	auto it = mWindowList.begin();
	auto itEnd = mWindowList.end();
	FOR_STL(mWindowList, ; it != itEnd; ++it)
	{
		Window* window = it->second;
		const VECTOR2& finalPos = window->getFinalPosition();
		const VECTOR2& finalSize = window->getFinalSize();
		mLayoutRect.merge(txRect(finalPos, finalPos + finalSize));
	}
	END_FOR_STL(mWindowList);
}

bool Layout::rename(const std::string& newName)
{
	if (mName == newName)
	{
		return false;
	}
	// 布局名不能与窗口名相同
	auto iterWindow = mWindowList.find(newName);
	if (iterWindow != mWindowList.end())
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	// 通知LayoutManager自己的名字改变了
	bool ret = mLayoutManager->notifyLayoutRenamed(oldName, this);
	// 如果管理器不允许改名,则将名字恢复
	if (!ret)
	{
		mName = oldName;
		return false;
	}
	return true;
}

Window* Layout::createIndependentWindow(const std::string& windowType, const std::string& windowName, const bool& initWindow)
{
	WINDOW_TYPE type = LayoutUtility::getWindowTypeFromTypeName(windowType);
	WindowFactoryBase* factory = mWindowFactoryManager->getFactory(type);
	if (factory == NULL)
	{
		return NULL;
	}
	std::string name = windowName;
	if (name == EMPTY_STRING)
	{
		name = factory->createUniqueName();
	}
	Window* window = factory->createWindow(name);
	if (initWindow && window != NULL)
	{
		window->init(NULL);
	}
	return window;
}

Window* Layout::createIndependentWindow(Window* sourceWindow, const std::string& windowName, const bool& initWindow)
{
	if (sourceWindow == NULL)
	{
		return NULL;
	}
	Window* window = createIndependentWindow(sourceWindow->getTypeName(), windowName, initWindow);
	sourceWindow->copyProperty(window);
	return window;
}

Window* Layout::createWindow(const std::string& windowType, const std::string& windowName)
{
	if (mWindowList.find(windowName) != mWindowList.end())
	{
		ENGINE_ERROR("error : there is a window named %s, can not create again! layout : %s", windowName.c_str(), mName.c_str());
		return NULL;
	}

	// 先创建一个独立的并且没有初始化的窗口
	Window* window = createIndependentWindow(windowType, windowName, false);
	if (window == NULL)
	{
		return NULL;
	}
	// 将窗口放入窗口列表中
	mWindowList.insert(window->getName(), window);
	window->init(this);
	// 第一个由layout创建的窗口就是该layout的根窗口
	if (mRootWindow == NULL)
	{
		setRootWindow(window);
	}
	return window;
}

bool Layout::loadLayout(const std::string& fileName, const bool& async, LayoutAsyncDone callback, void* userData)
{
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			auto valueIte = arrVal.begin();
			auto valueIteEnd = arrVal.end();
			for (; valueIte != valueIteEnd; ++valueIte)
			{
				std::string windowType = (*valueIte)["Type"].asString();
				std::string windowName = (*valueIte)["Name"].asString();
				std::string parentName = (*valueIte)["Parent"].asString();
				bool isInnerChild = txStringUtility::stringToBool((*valueIte)[txProperty::getPropertyName<WindowPropertyInnerChild>()].asString());
				Window* window = NULL;
				// 如果是内部子窗口,则不创建窗口,因为父窗口在创建时已经将内部子窗口创建出来了
				if (isInnerChild)
				{
					auto it = mWindowList.find(windowName);
					if (it != mWindowList.end())
					{
						window = it->second;
					}
				}
				else
				{
					window = createWindow(windowType, windowName);
					// 如果是根窗口,则不允许为根窗口添加父窗口
					if (window != mRootWindow && parentName != EMPTY_STRING)
					{
						// 设置窗口的父窗口
						Window* parentWindow = NULL;
						auto it = mWindowList.find(parentName);
						if (it != mWindowList.end())
						{
							parentWindow = it->second;
						}
						if (parentWindow != NULL)
						{
							parentWindow->addChild(window);
						}
						else
						{
							ENGINE_ERROR("error : can not find parent window, window name : %s, parent name : %s", window->getName().c_str(), parentName.c_str());
						}
					}
				}
				// 遍历窗口的所有属性
				auto& propertyMap = window->getPropertyMap();
				auto iter = propertyMap.begin();
				auto iterEnd = propertyMap.end();
				FOR_STL(propertyMap, ; iter != iterEnd; ++iter)
				{
					txProperty* property = iter->second;
					property->setAsync(async);
					std::string propertyValue = (*valueIte)[property->getName()].asString();
					if (propertyValue != EMPTY_STRING)
					{
						property->set(propertyValue, window);
					}
					// 设置完以后需要恢复异步标记,确保不会影响其他地方的使用
					property->setAsync(false);
				}
				END_FOR_STL(propertyMap);
				// 通知窗口属性已经设置完成
				window->notifyPropertyDone();
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : parse layout : %s failed! reason : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
	return ret;
}

void Layout::finishAsyncLoadLayout()
{
	// 遍历所有窗口,调用窗口的完成异步加载
	auto iterWindow = mWindowList.begin();
	auto iterWindowEnd = mWindowList.end();
	FOR_STL(mWindowList, ; iterWindow != iterWindowEnd; ++iterWindow)
	{
		iterWindow->second->finishAsycLoad();
	}
	END_FOR_STL(mWindowList);
	mLoadDone = true;

	mScript->assignWindow();
	mScript->init();
	mScriptInited = true;
	// 异步加载完成后立即强制隐藏
	setVisibleForce(false);
	if (mDoneCallback != NULL)
	{
		mDoneCallback(this, mDoneUserData);
		mDoneCallback = NULL;
		mDoneUserData = NULL;
	}
}

void Layout::saveLayout(std::string& layoutStream)
{
	txStringUtility::jsonStartArray(layoutStream, 0, true);
	if (mRootWindow != NULL)
	{
		mRootWindow->saveWindow(layoutStream);
	}
	txStringUtility::jsonEndArray(layoutStream, 0, true);
	txStringUtility::removeLastComma(layoutStream);
}

Window* Layout::getWindow(const std::string& windowName)
{
	auto iter = mWindowList.find(windowName);
	if (iter == mWindowList.end())
	{
		return NULL;
	}
	return iter->second;
}

void Layout::setVisibleForce(const bool& visible)
{
	if (mRootWindow != NULL)
	{
		mRootWindow->setVisible(visible);
	}
}

void Layout::setVisible(const bool& visible, const bool& immediately, const std::string& param)
{
	if (mScript == NULL || !mScriptInited)
	{
		setVisibleForce(visible);
		return;
	}
	// 设置布局显示或者隐藏时需要先通知脚本开始显示或隐藏
	mScript->notifyStartShowOrHide();
	// 显示布局时立即显示
	if (visible)
	{
		setVisibleForce(visible);
		mScript->onReset();
		mScript->onGameState();
		mScript->onShow(immediately, param);
	}
	// 隐藏布局时需要判断
	else
	{
		if (!mScriptControlHide)
		{
			setVisibleForce(visible);
		}
		mScript->onHide(immediately, param);
	}
}

bool Layout::isVisible()
{
	if (mRootWindow == NULL)
	{
		return true;
	}
	return mRootWindow->isVisible();
}

Window* Layout::getHoverWindow(const VECTOR2& point, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput)
{
	if (mRootWindow != NULL)
	{
		return mRootWindow->getHoverWindow(point, useTransform, forceVisible, forceHandleInput);
	}
	return NULL;
}

void Layout::getUsedResourcesList(txVector<std::string>& resourceList)
{
	resourceList.clear();
	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();
	FOR_STL(mWindowList, ; iter != iterEnd; ++iter)
	{
		iter->second->getUsedResourceList(resourceList);
	}
	END_FOR_STL(mWindowList);
}

void Layout::deleteWindow(const std::string& name)
{
	auto it = mWindowList.find(name);
	if (it != mWindowList.end())
	{
		Window* window = it->second;
		mLayoutManager->destroyWindow(window);
	}
}
void Layout::notifyWindowDestroied(const std::string& name)
{
	auto it = mWindowList.find(name);
	if (it != mWindowList.end())
	{
		mLayoutManager->notifyWindowDestroy(it->second);
		if (it->second == mRootWindow)
		{
			setRootWindow(NULL);
		}

		mWindowList.erase(it);
		mDirty = true;
	}
}

bool Layout::notifyWindowNameChanged(const std::string& oldName, Window* window)
{
	// 窗口名不能与布局名相同
	if (window->getName() == mName)
	{
		return false;
	}
	auto it = mWindowList.find(oldName);
	if (it == mWindowList.end())
	{
		return false;
	}

	auto itNew = mWindowList.find(window->getName());
	if (itNew != mWindowList.end())
	{
		return false;
	}
	mWindowList.insert(window->getName(), window);
	mWindowList.erase(it);
	return true;
}

void Layout::notifyRenderWindowChanged()
{
	if (NULL != mRootWindow)
	{
		mRootWindow->setDirty(true);
	}
}

void Layout::notifyWindowDetached(const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		if (iter->second == mRootWindow)
		{
			setRootWindow(NULL);
		}
		mWindowList.erase(iter);
		mDirty = true;
	}
}

void Layout::notifyWindowAttached(Window* window)
{
	if (window == NULL)
	{
		return;
	}
	auto iter = mWindowList.find(window->getName());
	if (iter == mWindowList.end())
	{
		// 如果该布局没有窗口,则连接的窗口会成为该布局的根窗口
		if (mWindowList.size() == 0)
		{
			setRootWindow(window);
		}
		mWindowList.insert(window->getName(), window);
		mDirty = true;
	}
}