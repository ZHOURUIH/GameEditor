#include "Utility.h"
#include "json/json.h"
#include "txEngineRoot.h"
#include "txBaseProperty.h"

#include "WindowPrefabManager.h"
#include "WindowHeader.h"
#include "WindowFactory.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "WindowProperty.h"

void WindowPrefabManager::init()
{
	loadAllPrefab();
}

void WindowPrefabManager::destroy()
{
	// 先复制一份prefab列表,然后遍历复制出的列表来销毁所有的prefab
	auto prefabList = mPrefabList;
	auto iter = prefabList.begin();
	auto iterEnd = prefabList.end();
	FOR_STL(prefabList, ; iter != iterEnd; ++iter)
	{
		destroyPrefab(iter->first);
	}
	END_FOR_STL(prefabList);
}

void WindowPrefabManager::loadAllPrefab()
{
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_UI_WINDOW_PREFAB_PATH, fileList, PREFAB_SUFFIX);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		loadPrefab(fileList[i]);
	}
	END_FOR_STL(fileList);
	mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
}

void WindowPrefabManager::loadPrefab(const std::string& fileName)
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
				std::string windowType = (*valueIte)[TYPE_PROPERTY_NAME].asString();
				std::string windowName = (*valueIte)[NAME_PROPERTY_NAME].asString();
				Window* window = NULL;
				if (windowType == PREFAB_TYPE_NAME)
				{
					// 创建预设
					window = createPrefab(windowName, false);
					WindowPrefab* windowPrefab = static_cast<WindowPrefab*>(window);
					windowPrefab->setFileName(txStringUtility::getFileName(fileName));
				}
				else
				{
					// 设置窗口的父窗口
					std::string parentName = (*valueIte)[PARENT_PROPERTY_NAME].asString();
					bool isInnerChild = txStringUtility::stringToBool((*valueIte)[txProperty::getPropertyName<WindowPropertyInnerChild>()].asString());
					// 如果是内部子窗口,则不创建窗口,因为父窗口在创建时已经将内部子窗口创建出来了
					if (isInnerChild)
					{
						if (parentName != EMPTY_STRING)
						{
							// 内部子窗口一定不是预设的子窗口,所以不需要在预设列表中查找父窗口
							auto iter = mWindowList.find(windowName);
							if (iter != mWindowList.end())
							{
								window = iter->second;
							}
							else
							{
								ENGINE_ERROR("error : can not find inner child! child name : %s", windowName.c_str());
							}
						}
						else
						{
							ENGINE_ERROR("error : inner child's parent is empty! child name : %s", windowName.c_str());
						}
					}
					else
					{
						// 创建窗口
						window = createWindow(windowType, windowName);
						if (parentName != EMPTY_STRING)
						{
							Window* parentWindow = NULL;
							auto it = mWindowList.find(parentName);
							// 在窗口列表中找不到就在预设列表中查找
							if (it != mWindowList.end())
							{
								parentWindow = it->second;
							}
							else
							{
								auto iterPrefab = mPrefabList.find(parentName);
								if (iterPrefab != mPrefabList.end())
								{
									parentWindow = iterPrefab->second;
								}
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
				}

				// 遍历窗口的所有属性
				auto& propertyMap = window->getPropertyMap();
				auto iter = propertyMap.begin();
				auto iterEnd = propertyMap.end();
				FOR_STL(propertyMap, ; iter != iterEnd; ++iter)
				{
					txProperty* property = iter->second;
					std::string propertyValue = (*valueIte)[property->getName()].asString();
					if (propertyValue != EMPTY_STRING)
					{
						property->set(propertyValue, window);
					}
				}
				END_FOR_STL(propertyMap);
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : parse prefab : %s failed! reason : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
}

WindowPrefab* WindowPrefabManager::createPrefab(const std::string& name, const bool& sendEvent)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		return iter->second;
	}
	WindowPrefab* prefab = static_cast<WindowPrefab*>(Layout::createIndependentWindow(PREFAB_TYPE_NAME, name, true));
	mPrefabList.insert(prefab->getName(), prefab);
	if (sendEvent)
	{
		mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
	}
	return prefab;
}

WindowPrefab* WindowPrefabManager::createPrefab(Window* window, const std::string& name)
{
	WindowPrefab* prefab = createPrefab(name);
	window->copyProperty(prefab);
	window->copyChildren(prefab, false, EMPTY_STRING);
	return prefab;
}

Window* WindowPrefabManager::createWindow(const std::string& typeName, const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		ENGINE_ERROR("error : there is a window named : %s, can not create in WindowPrefabManager!", name.c_str());
		return NULL;
	}
	Window* window = Layout::createIndependentWindow(typeName, name, true);
	// 将窗口放入窗口列表中
	mWindowList.insert(window->getName(), window);
	// 如果是复合窗口,则需要将复合窗口下的所有内部子窗口都加入列表中
	if (window->isComplexWindow())
	{
		addChildToList(window);
	}
	return window;
}
void WindowPrefabManager::addChildToList(Window* window)
{
	txVector<Window*> allChildList;
	window->getAllChildren(allChildList);
	int childCount = allChildList.size();
	FOR_STL(allChildList, int i = 0; i < childCount; ++i)
	{
		mWindowList.insert(allChildList[i]->getName(), allChildList[i]);
		addChildToList(allChildList[i]);
	}
	END_FOR_STL(allChildList);
}

void WindowPrefabManager::destroyPrefab(const std::string& name)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		// 在销毁预设以及子窗口前,将窗口列表中的该预设中的所有子窗口移除
		removeChildFromList(iter->second);

		// 先将prefab下的一级子窗口全部销毁,此处不能用引用获得子窗口列表,因为在销毁窗口过程中会对列表进行修改
		auto children = iter->second->getChildren();
		auto iterChild = children.begin();
		auto iterChildEnd = children.end();
		FOR_STL(children, ; iterChild != iterChildEnd; ++iterChild)
		{
			mLayoutManager->destroyWindow(*iterChild);
		}
		END_FOR_STL(children);

		// 然后再销毁prefab自身
		mLayoutManager->destroyWindow(iter->second);
		mPrefabList.erase(iter);
	}
	mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
}

void WindowPrefabManager::removeChildFromList(Window* window)
{
	// 如果该子窗口是复合窗口,则需要将复合窗口的内部子窗口从列表中移除
	// 这里需要考虑到window是复合窗口的情况
	txVector<Window*> allChildList;
	window->getAllChildren(allChildList);
	int childCount = allChildList.size();
	FOR_STL(allChildList, int i = 0; i < childCount; ++i)
	{
		auto iterInnerChild = mWindowList.find(allChildList[i]->getName());
		if (iterInnerChild != mWindowList.end())
		{
			mWindowList.erase(iterInnerChild);
		}
		removeChildFromList(allChildList[i]);
	}
	END_FOR_STL(allChildList);
}
	
void WindowPrefabManager::destroyPrefab(WindowPrefab* prefab)
{
	if (prefab == NULL)
	{
		return;
	}
	destroyPrefab(prefab->getName());
}

WindowPrefab* WindowPrefabManager::getPrefab(const std::string& name)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		return iter->second;
	}
	return NULL;
}