#include "txTexture.h"
#include "txTextureManager.h"
#include "txEntityManager.h"
#include "txNodeManager.h"
#include "txNode.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txCamera.h"
#include "Utility.h"
#include "txBaseProperty.h"
#include "txTremblingManager.h"
#include "txTrembling.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"
#include "txEngineEventSystem.h"

#include "Layout.h"
#include "WindowHeader.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "WindowProperty.h"
#include "LayoutUtility.h"
#include "LayoutManager.h"

#include "ECLog.h"
#include "EditorCoreRoot.h"
#include "CoreEventSystem.h"
#include "ECUndoManager.h"
#include "ECUndoLayoutVisible.h"
#include "ECUndoDeleteWindow.h"
#include "ECUndoAddWindow.h"
#include "ECUtility.h"
#include "ECUndoChangeWindowProperty.h"
#include "ECRenderWindowCoordinate.h"
#include "ECUndoWindowRename.h"
#include "ECUndoMoveWindowPos.h"
#include "ECRenderWindowCoordinate.h"
#include "CommandHeader.h"
#include "ECUndoCutWindow.h"
#include "LayoutEditorCore.h"
#include "TremblingEditorCore.h"
#include "ActionTreeEditorCore.h"
#include "ComponentEditorCore.h"

template<> EditorCoreRoot* txSingleton<EditorCoreRoot>::ms_Singleton = NULL;
EditorCoreRoot::EditorCoreRoot()
:
mInitFlag(false),
mEngineRoot(NULL),
mCoreEventSystem(NULL),
mUndoManager(NULL),
mMainCamera(NULL),
mModalDialog(NULL),
mTremblingEditorCore(NULL),
mLayoutEditorCore(NULL),
mActionTreeEditorCore(NULL),
mComponentEditorCore(NULL)
{
	memset(mDialogStyle, 0, sizeof(int) * MDS_MAX);
}

EditorCoreRoot::~EditorCoreRoot()
{
	destroy();
}

void EditorCoreRoot::init(const HWND& hwnd, const int& colourBits, const int& width, const int& height)
{
	// 初始化引擎
	mEngineRoot = TRACE_NEW(txEngineRoot, mEngineRoot);
	mEngineRoot->initialise(hwnd, colourBits, width, height);
	mEngineRoot->getRenderWindow()->setBackgroundColour(VECTOR4(0.5f, 0.5f, 0.5f, 1.0f));

	// 创建事件系统
	mCoreEventSystem = TRACE_NEW(CoreEventSystem, mCoreEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();
	mUndoManager = TRACE_NEW(ECUndoManager, mUndoManager);// 创建撤销管理器
	mTremblingEditorCore = TRACE_NEW(TremblingEditorCore, mTremblingEditorCore);
	mLayoutEditorCore = TRACE_NEW(LayoutEditorCore, mLayoutEditorCore);
	mActionTreeEditorCore = TRACE_NEW(ActionTreeEditorCore, mActionTreeEditorCore);
	mComponentEditorCore = TRACE_NEW(ComponentEditorCore, mComponentEditorCore);

	// 所有类都构造完成后通知EditorCoreBase
	EditorCoreBase::notifyConstructDone();

	mLayoutManager->init();
	mLayoutManager->setForceVisible(true);
	mLayoutManager->setForceHandInput(true);
	mUndoManager->init();
	mTremblingEditorCore->init();
	mLayoutEditorCore->init();
	mActionTreeEditorCore->init();
	mComponentEditorCore->init();

	// 摄像机
	initCamera();

	// 注册事件
	registerEngineEventHandler();

	// 加载关键帧文件
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_TREMBLING_PATH, fileList, TREMBLING_SUFFIX);
	int size = fileList.size();
	FOR_STL(fileList, int i = 0; i < size; ++i)
	{
		mEngineRoot->getTremblingManager()->loadTrembling(fileList[i]);
	}
	END_FOR_STL(fileList);
}

void EditorCoreRoot::registerEngineEventHandler()
{
	mEngineEventSystem->registeAllEvent(this);
}

void EditorCoreRoot::unregisterEngineEventHandler()
{
	mEngineEventSystem->unregisteAllEvent(this);
}

void EditorCoreRoot::onEngineEvent(const ENGINE_EVENT& type, const txVector<std::string>& params)
{
	if (type == EE_MOUSE_ENTER_WINDOW)
	{
		mLayoutEditorCore->notifyMouseEnterWindow(params[0]);
	}
	else if (type == EE_MOUSE_LEAVE_WINDOW)
	{
		mLayoutEditorCore->notifyMouseLeaveWindow(params[0]);
	}
	else if (type == EE_WINDOW_SELECTED)
	{
		Layout* curLayout = mLayoutEditorCore->getCurLayout();
		if (curLayout != NULL)
		{
			Window* window = curLayout->getWindow(params[0]);
			// 设置选中窗口
			mLayoutEditorCore->setSelection(window);
		}
	}
	else if (type == EE_WINDOW_UNSELECTED)
	{
		mLayoutEditorCore->setSelection(NULL);
	}
	else if (type == EE_WINDOW_PREFAB_CHANGED)
	{
		sendEvent(CE_WINDOW_PREFAB_CHANGED, params);
	}
	else if (type == EE_ERROR_LOG)
	{
		sendEvent(CE_ERROR_LOG, params);
	}
	else if (type == EE_PREFAB_RESOURCE_CHANGED)
	{
		sendEvent(CE_PREFAB_RESOURCE_CHANGED);
	}
	else if (type == EE_TEMPLATE_RESOURCE_CHANGED)
	{
		sendEvent(CE_TEMPLATE_RESOURCE_CHANGED);
	}
}

void EditorCoreRoot::preUpdate(const float& elapsedTime)
{
	mEngineRoot->preUpdate(elapsedTime);
}

void EditorCoreRoot::update(const float& elapsedTime)
{
	// 更新引擎
	mEngineRoot->update(elapsedTime);
	// 更新事件系统
	mCoreEventSystem->update(elapsedTime);
	mTremblingEditorCore->update(elapsedTime);
	mLayoutEditorCore->update(elapsedTime);
	mActionTreeEditorCore->update(elapsedTime);
	mComponentEditorCore->update(elapsedTime);
}

void EditorCoreRoot::lateUpdate(const float& elapsedTime)
{
	mEngineRoot->lateUpdate(elapsedTime);
}

void EditorCoreRoot::render()
{
	mEngineRoot->beginRender();
	if (NULL != mLayoutManager)
	{
		mLayoutManager->render();
	}
	mLayoutEditorCore->getRenderWindowCoordinate()->render();
	mEngineRoot->endRender();
}

void EditorCoreRoot::destroy()
{
	unregisterEngineEventHandler();
	TRACE_DELETE(mTremblingEditorCore);
	TRACE_DELETE(mLayoutEditorCore);
	TRACE_DELETE(mActionTreeEditorCore);
	TRACE_DELETE(mComponentEditorCore);
	TRACE_DELETE(mMainCamera);
	TRACE_DELETE(mUndoManager);
	TRACE_DELETE(mCoreEventSystem);
	TRACE_DELETE(mEngineRoot);
}

void EditorCoreRoot::resize(const int& width, const int& height)
{
	mEngineRoot->getRenderWindow()->setWindowSize(width, height);
	mMainCamera->setViewport(0.0f, 0.0f, (float)width, (float)height);
	mLayoutManager->notifyRenderWindowChanged();
}

void EditorCoreRoot::setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX])
{
	mModalDialog = dialog;
	memcpy(mDialogStyle, style, MDS_MAX * sizeof(int));
	memcpy(mDialogResult, result, MDR_MAX * sizeof(int));
}

void EditorCoreRoot::sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
}

void EditorCoreRoot::sendEvent(const CORE_EVENT& type, const txVector<std::string>& params, bool sendImmediately)
{
	// 如果是立即发送的事件,则需要根据初始化标记判断是否应该立即发送
	if (sendImmediately)
	{
		sendImmediately = mInitFlag;
	}
	if (mCoreEventSystem != NULL)
	{
		mCoreEventSystem->pushEvent(type, params, sendImmediately);
	}
}

void EditorCoreRoot::sendDelayEvent(const CORE_EVENT& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void EditorCoreRoot::sendDelayEvent(const CORE_EVENT& type, const txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void EditorCoreRoot::logError(const std::string& info)
{
	// 如果事件系统还没有创建,则先放入错误信息缓冲中
	if (mCoreEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	sendEvent(CE_ERROR_LOG, info);
}

void EditorCoreRoot::logInfo(const std::string& info)
{
	sendEvent(CE_INFO_LOG, info);
}

void EditorCoreRoot::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	FOR_STL(mErrorInfoBuffer, int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	END_FOR_STL(mErrorInfoBuffer);
	mErrorInfoBuffer.clear();
}

void EditorCoreRoot::initCamera()
{
	mMainCamera = TRACE_NEW(txCamera, mMainCamera);
	txRenderWindow* pEngineRenderWindow = mEngineRoot->getRenderWindow();
	float aspect = pEngineRenderWindow->getAspect();
	mMainCamera->setPerspectiveParameters(45.0f, aspect, 1.0f, 600.0f);
	float width = (float)(pEngineRenderWindow->getWidth());
	float height = (float)(pEngineRenderWindow->getHeight());
	mMainCamera->setViewport(0.0f, 0.0f, width, height);
	mMainCamera->setName("Main");
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModalYesNoCancel(const std::string& info)
{
	return showModal(info, mDialogStyle[MDS_YES] | mDialogStyle[MDS_NO] | mDialogStyle[MDS_CANCEL]);
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModalYesNo(const std::string& info)
{
	return showModal(info, mDialogStyle[MDS_YES] | mDialogStyle[MDS_NO]);
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModal(const std::string& info, const int& flag)
{
	// 如果没有设置打开提示对话框的函数,则直接继续执行下去
	if (mModalDialog == NULL)
	{
		return MDR_MAX;
	}
	int ret = mModalDialog("提示", info, flag);
	for (int i = 0; i < MDR_MAX; ++i)
	{
		if (ret == mDialogResult[i])
		{
			return (MODAL_DIALOG_RESULT)i;
		}
	}
	return MDR_MAX;
}