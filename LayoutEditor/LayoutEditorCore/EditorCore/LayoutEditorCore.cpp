#include "txRenderWindow.h"
#include "LayoutEditorCore.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "WindowPrefab.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "UndoHeader.h"
#include "ECRenderWindowCoordinate.h"
#include "ECLog.h"
#include "CommandHeader.h"
#include "WindowProperty.h"
#include "ComponentEditorCore.h"
#include "InputSystem.h"
#include "Utility.h"

LayoutEditorCore::LayoutEditorCore()
:
mCurLayout(NULL),
mLayoutModified(false),
mCopyWindow(NULL),
mCopySrcLayout(NULL),
mCutWindow(false),
mWireframeVisible(true),
mCoordinateVisible(false)
{
	mRenderWindowCoordinate = TRACE_NEW(ECRenderWindowCoordinate, mRenderWindowCoordinate);
}

void LayoutEditorCore::init()
{
	mRenderWindowCoordinate->init();
}

void LayoutEditorCore::update(float elapsedTime)
{
	processKey();
}

Layout* LayoutEditorCore::createLayout(const std::string& layoutName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("文件未保存,是否保存然后新建文件?"))
		{
			return NULL;
		}
	}

	if (mLayoutManager->getLayout(layoutName) != NULL)
	{
		EDITOR_CORE_ERROR("error : there is a layout named : %s, can not create again!", layoutName.c_str());
		return NULL;
	}
	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	mCurLayout = mLayoutManager->createLayout(-1, layoutName, 0);
	mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());
	setLayoutModified(true);
	// 如果layout创建成功,则发送layout改变的事件
	if (mCurLayout != NULL)
	{
		mEditorCoreRoot->sendEvent(CE_LAYOUT_ADDED, mCurLayout->getName());
		setLayoutVisible(mCurLayout, true);
	}
	return mCurLayout;
}

Layout* LayoutEditorCore::loadLayout(const std::string& fileName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("当前文件未保存,是否保存文件然后打开新的文件?"))
		{
			return NULL;
		}
	}

	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	std::string layoutName = fileName.substr(0, fileName.find_last_of('.'));
	mCurLayout = mLayoutManager->createLayout(-1, layoutName, 0, fileName);
	mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());
	// 强制发送布局改变的事件,以便能在加载布局时提示界面层修改窗口标题
	setLayoutModified(false, true);
	// 如果layout加载成功,则发送layout改变的事件
	if (mCurLayout != NULL)
	{
		// 强制显示该布局
		setLayoutVisible(mCurLayout, true);
		mEditorCoreRoot->sendEvent(CE_LAYOUT_ADDED, layoutName);
	}
	return mCurLayout;
}

void LayoutEditorCore::closeLayout(const std::string& layoutName)
{
	closeLayout(mLayoutManager->getLayout(layoutName));
}

void LayoutEditorCore::closeLayout(Layout* layout)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("当前文件未保存,是否保存然后关闭文件?"))
		{
			return;
		}
	}
	if (layout == NULL)
	{
		return;
	}
	// 如果关闭当前layout时需要将选择项清空
	if (layout == mCurLayout)
	{
		clearSelection();
		mCurLayout = NULL;
		// 关闭当前布局后设置布局修改状态为未修改
		setLayoutModified(false);
		mLayoutManager->clearActiveHoverWindow();
	}
	// 隐藏布局
	setLayoutVisible(layout, false);
	// 发出layout已经被关闭的事件
	mEditorCoreRoot->sendEvent(CE_LAYOUT_CLOSED, layout->getName());

	// 关闭布局时清空所有撤销操作
	mUndoManager->clearUndo();
	mUndoManager->clearRedo();
}

void LayoutEditorCore::destroyLayout(const std::string& layoutName)
{
	destroyLayout(mLayoutManager->getLayout(layoutName));
}

void LayoutEditorCore::destroyLayout(Layout* layout)
{
	if (mCurLayout == layout && isLayoutModified())
	{
		if (!askSaveLayout("当前文件未保存,是否保存文件然后销毁文件?"))
		{
			return;
		}
	}
	if (layout == NULL)
	{
		return;
	}
	// 关闭布局
	closeLayout(layout);
	// 如果是销毁的拷贝源所属的布局,则需要清空源布局指针
	if (mCopySrcLayout == layout)
	{
		mCopySrcLayout = NULL;
	}
	// 销毁布局,此处不能使用引用,因为后面还要使用该变量的值
	std::string layoutName = layout->getName();
	mLayoutManager->destroyLayout(layoutName);
	// 发出layout已经被销毁的事件
	mEditorCoreRoot->sendEvent(CE_LAYOUT_DESTROIED, layoutName);
}

void LayoutEditorCore::saveLayout()
{
	if (mCurLayout == NULL || !mLayoutModified)
	{
		return;
	}
	std::string layoutStream;
	mCurLayout->saveLayout(layoutStream);
	std::string filePath = P_UI_LAYOUT_PATH + mCurLayout->getName() + ".json";
	txFileUtility::writeFile(filePath, layoutStream.length(), layoutStream.c_str());
	setLayoutModified(false);
}

void LayoutEditorCore::editPrefab(const std::string& prefabName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("文件未保存, 是否保存文件然后创建预设布局?"))
		{
			return;
		}
	}
	// 关闭当前布局
	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	// 创建预设布局,并重命名
	Layout* prefabEditLayout = createLayout(prefabName + "_layout");
	if (prefabEditLayout == NULL)
	{
		return;
	}
	// 添加默认根窗口
	const std::string& windowTypeName = LayoutUtility::getTypeNameFromWindowType(WT_WINDOW);
	Window* rootWindow = addWindow(prefabEditLayout, windowTypeName, prefabName);
	// 然后将预设实例化到根窗口中
	WindowPrefab* prefab = mWindowPrefabManager->getPrefab(prefabName);
	CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
	cmdProperty->mPropertyName = txProperty::getPropertyName<WindowPropertyPrefab>();
	cmdProperty->mPropertyValue = prefabName;
	cmdProperty->mOperator = this;
	mCommandSystem->pushCommand(cmdProperty, rootWindow);
	// 实例化预设后必须将预设链接去除
	rootWindow->setPrefabName(EMPTY_STRING);
	// 将预设根窗口的属性拷贝到当前布局根窗口中
	prefab->copyProperty(rootWindow);
	// 实例化完后,修改所有窗口的名字,去掉根窗口的前缀,由于在改名时会修改布局中的窗口列表,所以需要复制一份列表
	auto windowList = prefabEditLayout->getWindowList();
	auto iterWindow = windowList.begin();
	auto iterWindowEnd = windowList.end();
	FOR_STL(windowList, ; iterWindow != iterWindowEnd; ++iterWindow)
	{
		if (iterWindow->second != rootWindow)
		{
			CommandWindowRename* cmdRename = NEW_CMD(cmdRename);
			std::string windowName = iterWindow->second->getName();
			if (txStringUtility::startWith(windowName, prefabName))
			{
				windowName = windowName.substr(prefabName.length(), windowName.length() - prefabName.length());
			}
			cmdRename->mNewName = windowName;
			mCommandSystem->pushCommand(cmdRename, iterWindow->second);
		}
	}
	END_FOR_STL(windowList);
}

void LayoutEditorCore::savePrefab(const std::string& windowName)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(windowName);
	if (window == NULL)
	{
		EDITOR_CORE_ERROR("error : can not find source window!");
		return;
	}
	if (window->getType() != WT_WINDOW)
	{
		EDITOR_CORE_ERROR("error : prefab must create from a base window! can not create from a %s window", window->getTypeName().c_str());
		return;
	}
	if (window->hasLinkedPrefab())
	{
		EDITOR_CORE_ERROR("error : window has link a prefab! can not create prefab from this window!");
		return;
	}
	// 如果已经存在同名预设,则先销毁原有的预设
	WindowPrefab* existPrefab = mWindowPrefabManager->getPrefab(windowName);
	if (existPrefab != NULL)
	{
		int ret = mEditorCoreRoot->showModalYesNo("是否覆盖已有的预设?");
		if (ret == MDR_YES)
		{
			mWindowPrefabManager->destroyPrefab(existPrefab);
		}
		else
		{
			return;
		}
	}
	WindowPrefab* prefab = mWindowPrefabManager->createPrefab(window, windowName);
	if (prefab == NULL)
	{
		return;
	}
	std::string stream;
	prefab->savePrefab(stream);
	std::string filePath = P_UI_WINDOW_PREFAB_PATH + prefab->getName() + PREFAB_SUFFIX;
	txFileUtility::writeFile(filePath, stream.length(), stream.c_str());
}

void LayoutEditorCore::saveTemplate(const std::string& windowName)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(windowName);
	if (window == NULL)
	{
		return;
	}
	// 如果已经存在同名模板,则先销毁原有模板
	Window* existTemplate = mWindowTemplateManager->getTemplate(windowName);
	if (existTemplate != NULL)
	{
		int ret = mEditorCoreRoot->showModalYesNo("是否覆盖已有的模板?");
		if (ret == MDR_YES)
		{
			mWindowTemplateManager->destroyTemplate(existTemplate);
		}
		else
		{
			return;
		}
	}
	Window* templateWindow = mWindowTemplateManager->createTemplate(window, windowName);
	const std::string& fileName = mWindowTemplateManager->getTemplateFileName(windowName);
	std::string stream;
	window->saveTemplate(stream);
	txFileUtility::writeFile(P_UI_WINDOW_TEMPLATE_PATH + fileName, stream.length(), stream.c_str());
}

void LayoutEditorCore::setLayoutVisible(Layout* layout, const bool& visible)
{
	if (layout == NULL || layout->isVisible() == visible)
	{
		return;
	}
	layout->setVisible(visible, true, EMPTY_STRING);
	txVector<std::string> params;
	params.push_back(layout->getName());
	params.push_back(txStringUtility::boolToString(visible));
	mEditorCoreRoot->sendEvent(CE_LAYOUT_VISIBLE_CHANGED, params);

	ECUndoLayoutVisible* undo = mUndoManager->createUndo<ECUndoLayoutVisible>();
	undo->setLayout(layout);
	undo->setVisible(!visible);
	mUndoManager->addUndo(undo);
}

void LayoutEditorCore::setLayoutVisible(const std::string& layoutName, const bool& visible)
{
	setLayoutVisible(mLayoutManager->getLayout(layoutName), visible);
}

void LayoutEditorCore::setWireframeVisible(const bool& visible)
{
	if (mWireframeVisible == visible)
	{
		return;
	}
	mWireframeVisible = visible;
	// 设置当前选中项的线框显示状态
	int selectionCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
	{
		mMultiSelections[i]->setShowWireframe(mWireframeVisible);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setSelectionWindowCoordinateVisible(const bool& visible)
{
	if (mCoordinateVisible == visible)
	{
		return;
	}
	mCoordinateVisible = visible;
	// 设置当前选中项的本地坐标系显示状态
	int selectionCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
	{
		mMultiSelections[i]->setShowCoordinate(mCoordinateVisible);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setRenderWindowCoordinateVisible(const bool& visible)
{
	mRenderWindowCoordinate->setVisible(visible);
}

const bool& LayoutEditorCore::getRenderWindowCoordinateVisible()
{
	return mRenderWindowCoordinate->isVisible();
}

void LayoutEditorCore::setScreenDensity(const float& density)
{
	mRenderWindow->setScreenDensity(density);
	// 通知刷新所有窗口
	mLayoutManager->notifyRenderWindowChanged();
}

void LayoutEditorCore::pickupAllResources(const std::string& targetPath)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("文件未保存,是否保存文件然后开始提取所有资源?"))
		{
			return;
		}
	}
	// 如果目标目录不存在,创建目标目录
	txFileUtility::createFolder(targetPath);

	// 加载当前目录中的所有布局
	txVector<std::string> layoutFiles;
	txFileUtility::findFiles(P_UI_LAYOUT_PATH, layoutFiles, ".json");
	int fileCount = layoutFiles.size();
	FOR_STL(layoutFiles, int i = 0; i < fileCount; ++i)
	{
		std::string fileName = txStringUtility::getFileName(layoutFiles[i]);
		// 创建layout,并且获得其中的所有使用的资源的名称列表,然后销毁layout
		Layout* layout = loadLayout(fileName);
		txVector<std::string> resourceList;
		layout->getUsedResourcesList(resourceList);
		destroyLayout(layout);

		// 将所有已使用的资源拷贝到目标目录中
		int resourceCount = resourceList.size();
		FOR_STL(resourceList, int j = 0; j < resourceCount; ++j)
		{
			txFileUtility::copyFile(resourceList[j], targetPath + "/" + resourceList[j]);
		}
		END_FOR_STL(resourceList);
	}
	END_FOR_STL(layoutFiles);
	mEditorCoreRoot->logInfo("资源提取完毕!");
}

void LayoutEditorCore::moveSelectedWindow(const VECTOR2& moveDelta)
{
	int selectCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectCount; ++i)
	{
		txPoint windowPos = mMultiSelections[i]->getPosition();
		windowPos.x.mAbsolute += (int)moveDelta.x;
		windowPos.y.mAbsolute += (int)moveDelta.y;
		mMultiSelections[i]->setPosition(windowPos);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setLayoutModified(const bool& modified, const bool& force, const bool& notifyEvent)
{
	if (modified != mLayoutModified || force)
	{
		mLayoutModified = modified;
		if (notifyEvent)
		{
			// 发送修改状态改变的事件
			txVector<std::string> params;
			params.push_back(mCurLayout ? mCurLayout->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mLayoutModified));
			mEditorCoreRoot->sendEvent(CE_LAYOUT_MODIFIED, params);
		}
	}
}

void LayoutEditorCore::deleteWindow(const std::string& name, const std::string& layoutName)
{
	Layout* layout = layoutName != EMPTY_STRING ? mLayoutManager->getLayout(layoutName) : mCurLayout;
	Window* window = layout->getWindow(name);
	if (NULL == window)
	{
		return;
	}

	// 在窗口被删除之前添加删除窗口的撤销操作
	ECUndoDeleteWindow* undo = mUndoManager->createUndo<ECUndoDeleteWindow>();
	undo->setWindow(window);
	mUndoManager->addUndo(undo);

	clearSelection();

	Window* parent = window->getParent();
	std::string parentName = EMPTY_STRING;
	if (NULL != parent)
	{
		parentName = parent->getName();
	}
	layout->deleteWindow(name);
	txVector<std::string> params;
	params.push_back(parentName);
	params.push_back(name);
	mEditorCoreRoot->sendEvent(CE_WINDOW_DELETED, params);

	// 记录布局有改变
	setLayoutModified(true);
}

Window* LayoutEditorCore::addWindow(Layout* parent, const std::string& typeName, const std::string& windowName, const int& childPos)
{
	if (parent == NULL)
	{
		EDITOR_CORE_ERROR("Layout不能为空!");
		return NULL;
	}
	else if (parent->getRootWindow() != NULL)
	{
		EDITOR_CORE_ERROR("Layout只能有一个根窗口,请选择其他窗口添加子窗口");
		return NULL;
	}
	Window* window = parent->createWindow(typeName, windowName);
	addWindow(parent, NULL, window, childPos);
	return window;
}

Window* LayoutEditorCore::addWindow(Window* parent, const std::string& typeName, const std::string& windowName, const int& childPos)
{
	Layout* layout = parent->getLayout();
	Window* windowNew = layout->createWindow(typeName, windowName);
	addWindow(layout, parent, windowNew, childPos);
	return windowNew;
}

void LayoutEditorCore::addWindow(Layout* layout, Window* parent, Window* child, const int& childPos)
{
	if (layout == NULL || child == NULL)
	{
		return;
	}

	// 先将子窗口与原来的布局和父窗口的联系断开
	child->detachLayoutParent();
	// 重新建立与布局和父窗口的联系
	child->attachLayoutParent(layout, parent, childPos);

	// 发送已添加窗口的事件
	txVector<std::string> params;
	params.push_back(parent != NULL ? parent->getName() : EMPTY_STRING);
	params.push_back(child->getName());
	params.push_back(txStringUtility::intToString(childPos));
	mEditorCoreRoot->sendEvent(CE_WINDOW_ADDED, params);

	// 记录布局有改变
	setLayoutModified(true);

	// 加入添加窗口的撤销操作
	ECUndoAddWindow* undo = mUndoManager->createUndo<ECUndoAddWindow>();
	undo->setWindowName(child->getName());
	undo->setLayoutName(layout != NULL ? layout->getName() : EMPTY_STRING);
	mUndoManager->addUndo(undo);
}

void LayoutEditorCore::setSelection(Window* selection)
{
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mMultiSelections.size() == 1 && mMultiSelections[0] == selection)
	{
		return;
	}

	clearSelection(false);
	if (selection != NULL)
	{
		mMultiSelSet.insert(selection);
		mMultiSelections.push_back(selection);
	}

	notifySelectionChanged();
}

void LayoutEditorCore::setSelection(txVector<Window*>& selection)
{
	bool sameSelection = true;
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mMultiSelections.size() == selection.size())
	{
		int selectionCount = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mMultiSelections[i] != selection[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mMultiSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearSelection(false);
	int selCount = selection.size();
	FOR_STL(selection, int i = 0; i < selCount; ++i)
	{
		auto iter = mMultiSelSet.find(selection[i]);
		if (iter == mMultiSelSet.end())
		{
			mMultiSelections.push_back(selection[i]);
			mMultiSelSet.insert(selection[i]);
		}
	}
	END_FOR_STL(selection);

	notifySelectionChanged();
}

void LayoutEditorCore::reselect()
{
	mEditorCoreRoot->sendEvent(CE_SELECTION_CHANGED);
}

void LayoutEditorCore::clearSelection(const bool& sendEvt)
{
	// 清空窗口选择项时也需要清空组件选择项
	mComponentEditorCore->clearComponentSelection(sendEvt);
	// 先将当前所有选中项的线框隐藏
	if (mWireframeVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowWireframe(false);
		}
		END_FOR_STL(mMultiSelections);
	}
	if (mCoordinateVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowCoordinate(false);
		}
		END_FOR_STL(mMultiSelections);
	}
	mMultiSelSet.clear();
	mMultiSelections.clear();
	notifySelectionChanged(sendEvt);
}

void LayoutEditorCore::copyWindow(const std::string& name, const bool& copyChild, const bool& cutWindow)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(name);
	if (window == NULL)
	{
		return;
	}
	if (mCopyWindow != NULL)
	{
		mLayoutManager->destroyWindow(mCopyWindow);
		mCopyWindow = NULL;
	}
	mCopySrcLayout = mCurLayout;
	if (cutWindow)
	{
		mCutWindowName = name;
	}
	else
	{
		mCopyWindow = Layout::createIndependentWindow(window, name);
		// 只有没有链接预设时才能拷贝子窗口
		if (copyChild && mCopyWindow->getPrefabName() == EMPTY_STRING)
		{
			// 由于拷贝出来的窗口不属于任何布局,所以子窗口名字不加任何前缀
			window->copyChildren(mCopyWindow, false, EMPTY_STRING);
		}
	}
	mCutWindow = cutWindow;
}

void LayoutEditorCore::pasteWindow(const std::string& destParent, const int& pos)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* destWindow = mCurLayout->getWindow(destParent);
	if (destWindow == NULL)
	{
		mEditorCoreRoot->logInfo("找不到拷贝的目标窗口");
		return;
	}
	if (mCutWindow)
	{
		if (mCopySrcLayout != mCurLayout)
		{
			mEditorCoreRoot->logInfo("剪切操作只能在同一布局中进行的");
			return;
		}
		if (mCutWindowName == destParent)
		{
			mEditorCoreRoot->logInfo("剪切到的目标窗口不能是源窗口");
			return;
		}
		Window* cutWindow = mCopySrcLayout->getWindow(mCutWindowName);
		if (cutWindow == NULL)
		{
			return;
		}
		if (cutWindow->isChild(destWindow))
		{
			mEditorCoreRoot->logInfo("剪切到的目标窗口不能是源窗口的子窗口");
			return;
		}
		const std::string& oldParent = cutWindow->getParent()->getName();
		int oldPos = cutWindow->getParent()->getChildPos(cutWindow);
		// 将窗口与父窗口的链接断开
		cutWindow->detachLayoutParent();
		// 挂接到新的父窗口下
		cutWindow->attachLayoutParent(mCurLayout, destWindow, pos);

		// 发送剪切窗口的事件
		mEditorCoreRoot->sendEvent(CE_WINDOW_CUTED, mCutWindowName);

		// 加入添加窗口的撤销操作
		ECUndoCutWindow* undo = mUndoManager->createUndo<ECUndoCutWindow>();
		undo->setCutWindowInfo(mCutWindowName, oldParent, oldPos);
		mUndoManager->addUndo(undo);

		// 标记布局已修改
		setLayoutModified(true);
	}
	else
	{
		if (mCopyWindow == NULL)
		{
			mEditorCoreRoot->logInfo("找不到拷贝的源窗口");
			return;
		}
		// 根据拷贝的窗口新创建一个窗口,会自动将属性拷贝到新建窗口中
		Window* newWindow = Layout::createIndependentWindow(mCopyWindow, EMPTY_STRING);
		// 没有链接预设时,再将所有的子窗口拷贝到新创建的窗口下
		if (mCopyWindow->getPrefabName() == EMPTY_STRING)
		{
			mCopyWindow->copyChildren(newWindow, false, newWindow->getName());
		}
		// 将新创建的窗口添加到目标窗口下面
		addWindow(mCurLayout, destWindow, newWindow, pos);
	}
}

bool LayoutEditorCore::renameLayout(const std::string& newName)
{
	if (mCurLayout != NULL)
	{
		// 此处不能使用常量引用,因为此时会改变名字,导致oldName和newName相同
		std::string oldName = mCurLayout->getName();
		bool ret = mCurLayout->rename(newName);
		// 改名成功
		if (ret)
		{
			// 设置布局管理器中需要检测鼠标悬停的布局名
			mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());

			// 发送布局改名的事件
			txVector<std::string> params;
			params.push_back(oldName);
			params.push_back(newName);
			mEditorCoreRoot->sendEvent(CE_LAYOUT_RENAMED, params);

			// 记录布局有改变
			setLayoutModified(true);
		}
		else
		{
			mEditorCoreRoot->logInfo("布局改名失败,已存在同名布局或与布局中窗口重名");
		}
		return ret;
	}
	return false;
}

void LayoutEditorCore::moveWindowPos(Window* window, const int& destPos)
{
	if (window == NULL || destPos < 0)
	{
		return;
	}

	Window* parent = window->getParent();
	if (NULL != parent)
	{
		int oldPos = parent->getChildPos(window);
		if (parent->moveChildPos(window, destPos))
		{
			// 发送窗口位置顺序发生改变
			txVector<std::string> params;
			params.push_back(window->getName());
			params.push_back(parent->getName());
			params.push_back(txStringUtility::intToString(oldPos));
			params.push_back(txStringUtility::intToString(destPos));
			mEditorCoreRoot->sendEvent(CE_WINDOW_POS_CHANGED, params);

			// 记录布局有改变
			setLayoutModified(true);

			// 添加到撤销列表
			ECUndoMoveWindowPos* movePosUndo = mUndoManager->createUndo<ECUndoMoveWindowPos>();
			if (NULL != movePosUndo)
			{
				movePosUndo->setPos(oldPos);
				movePosUndo->setWindowName(window->getName());
				mUndoManager->addUndo(movePosUndo);
			}
		}
	}
}

bool LayoutEditorCore::askSaveLayout(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// 点击是,则保存文件
	if (ret == MDR_YES)
	{
		saveLayout();
	}
	// 点击否,则标记当前布局为未修改状态
	else if (ret == MDR_NO)
	{
		setLayoutModified(false, false, false);
	}
	// 点击取消则不打开文件
	else
	{
		return false;
	}
	return true;
}

void LayoutEditorCore::notifySelectionChanged(const bool& notifyEvent)
{
	if (notifyEvent)
	{
		mEditorCoreRoot->sendEvent(CE_SELECTION_CHANGED);
	}

	// 如果需要显示线框,则设置显示,否则不作处理
	if (mWireframeVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowWireframe(true);
		}
		END_FOR_STL(mMultiSelections);
	}
	if (mCoordinateVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowCoordinate(true);
		}
		END_FOR_STL(mMultiSelections);
	}
}

void LayoutEditorCore::notifyMouseEnterWindow(const std::string& windowName)
{
	if (mCurLayout != NULL)
	{
		Window* window = mCurLayout->getWindow(windowName);
		mPreWireframeColor = window->getWireframeColour();
		mPreCoordinateColor = window->getCoordinateColour();
		window->setShowCoordinate(mCoordinateVisible);
		window->setShowWireframe(mWireframeVisible);
		window->setWireframeColour(VECTOR4(1.0f, 0.0f, 1.0f, 1.0f));
		window->setCoordinateColour(VECTOR4(1.0f, 0.0f, 1.0f, 1.0f));
	}
}

void LayoutEditorCore::notifyMouseLeaveWindow(const std::string& windowName)
{
	if (mCurLayout != NULL)
	{
		Window* window = mCurLayout->getWindow(windowName);
		window->setWireframeColour(mPreWireframeColor);
		window->setCoordinateColour(mPreCoordinateColor);
		auto iter = mMultiSelSet.find(window);
		if (iter == mMultiSelSet.end())
		{
			window->setShowWireframe(false);
			window->setShowCoordinate(false);
		}
	}
}

void LayoutEditorCore::processKey()
{
	float moveSpeed = 0.0f;
	if (mInputSystem->isKeyDown(VK_SHIFT))
	{
		moveSpeed = 1.0f;
	}
	else if (mInputSystem->isKeyDown(VK_CONTROL))
	{
		moveSpeed = 10.0f;
	}
	if (moveSpeed > 0.0f)
	{
		// 上下左右键移动窗口
		if (mInputSystem->isKeyDown(VK_UP))
		{
			moveSelectedWindow(VECTOR2(0.0f, 1.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_DOWN))
		{
			moveSelectedWindow(VECTOR2(0.0f, -1.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_LEFT))
		{
			moveSelectedWindow(VECTOR2(-1.0f, 0.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_RIGHT))
		{
			moveSelectedWindow(VECTOR2(1.0f, 0.0f) * moveSpeed);
		}
	}
}