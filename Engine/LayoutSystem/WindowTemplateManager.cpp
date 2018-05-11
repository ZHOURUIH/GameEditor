#include "Utility.h"
#include "json/json.h"
#include "txEngineRoot.h"
#include "txProperty.h"

#include "WindowTemplateManager.h"
#include "WindowHeader.h"
#include "Layout.h"
#include "LayoutManager.h"
#include "WindowProperty.h"

void WindowTemplateManager::init()
{
	loadAllTemplate();
}

void WindowTemplateManager::destroy()
{
	// 先复制一份template列表,然后遍历复制出的列表来销毁所有的template
	auto templateList = mTemplateList;
	auto iter = templateList.begin();
	auto iterEnd = templateList.end();
	FOR_STL(templateList, ; iter != iterEnd; ++iter)
	{
		destroyTemplate(iter->first);
	}
	END_FOR_STL(templateList);
}

void WindowTemplateManager::loadAllTemplate()
{
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_UI_WINDOW_TEMPLATE_PATH, fileList, TEMPLATE_SUFFIX);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		loadTemplate(fileList[i]);
	}
	END_FOR_STL(fileList);
	mEngineRoot->sendEvent(EE_TEMPLATE_RESOURCE_CHANGED);
}

void WindowTemplateManager::loadTemplate(const std::string& fileName)
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
				bool isInnerChild = txStringUtility::stringToBool((*valueIte)[txProperty::getPropertyName<WindowPropertyInnerChild>()].asString());
				Window* window = NULL;
				// 如果是内部子窗口,则不创建窗口,因为父窗口在创建时已经将内部子窗口创建出来了
				if (isInnerChild)
				{
					// 内部子窗口则从窗口列表中获取
					auto iterChild = mWindowList.find(windowName);
					if (iterChild != mWindowList.end())
					{
						window = iterChild->second;
					}
					else
					{
						ENGINE_ERROR("error : can not find inner child in window list!child name : %s", windowName.c_str());
					}
				}
				else
				{
					// 创建窗口
					window = createTemplate(windowType, windowName, false);
				}
				// 重新设置模板所在的文件名
				setFileName(window->getName(), txStringUtility::getFileName(fileName));

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
		ENGINE_ERROR("error : parse template : %s failed!, reason : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
}

void WindowTemplateManager::setFileName(const std::string& templateName, const std::string& fileName)
{
	auto iter = mTemplateList.find(templateName);
	if (iter != mTemplateList.end())
	{
		iter->second.second = fileName;
	}
}

Window* WindowTemplateManager::createTemplate(const std::string& typeName, const std::string& name, const bool& sendEvent)
{
	auto iter = mTemplateList.find(name);
	if (iter != mTemplateList.end())
	{
		return iter->second.first;
	}
	Window* templateWindow = Layout::createIndependentWindow(typeName, name, true);
	mTemplateList.insert(templateWindow->getName(), std::make_pair(templateWindow, name + TEMPLATE_SUFFIX));
	mWindowList.insert(templateWindow->getName(), templateWindow);
	// 如果是复合窗口,则需要将复合窗口下的所有内部子窗口都加入列表中
	if (templateWindow->isComplexWindow())
	{
		addChildToList(templateWindow);
	}
	if (sendEvent)
	{
		mEngineRoot->sendEvent(EE_TEMPLATE_RESOURCE_CHANGED);
	}
	return templateWindow;
}

void WindowTemplateManager::addChildToList(Window* window)
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

Window* WindowTemplateManager::createTemplate(Window* window, const std::string& name)
{
	Window* templateWindow = createTemplate(window->getTypeName(), name);
	window->copyProperty(templateWindow);
	return templateWindow;
}

bool WindowTemplateManager::destroyTemplate(const std::string& name)
{
	auto iter = mTemplateList.find(name);
	if (iter != mTemplateList.end())
	{
		Window* window = iter->second.first;
		// 在销毁模板窗口之前,将窗口列表中的该模板窗口以及该窗口下的所有子窗口移除
		auto iterWindow = mWindowList.find(name);
		if (iterWindow != mWindowList.end())
		{
			mWindowList.erase(iterWindow);
			if (window->isComplexWindow())
			{
				removeChildFromList(window);
			}
		}
		mLayoutManager->destroyWindow(window);
		mTemplateList.erase(iter);
		mEngineRoot->sendEvent(EE_TEMPLATE_RESOURCE_CHANGED);
		return true;
	}
	return false;
}

void WindowTemplateManager::removeChildFromList(Window* window)
{
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

bool WindowTemplateManager::destroyTemplate(Window* windowTemplate)
{
	if (windowTemplate == NULL)
	{
		return false;
	}
	return destroyTemplate(windowTemplate->getName());
}

Window* WindowTemplateManager::getTemplate(const std::string& name)
{
	auto iter = mTemplateList.find(name);
	if (iter != mTemplateList.end())
	{
		return iter->second.first;
	}
	return NULL;
}

const std::string& WindowTemplateManager::getTemplateFileName(const std::string& name)
{
	auto iter = mTemplateList.find(name);
	if (iter != mTemplateList.end())
	{
		return iter->second.second;
	}
	return EMPTY_STRING;
}