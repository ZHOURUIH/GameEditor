#ifndef _EDITOR_CORE_ROOT_H_
#define _EDITOR_CORE_ROOT_H_

#include "txSingleton.h"
#include "txEngineBase.h"
#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineEventHandler.h"

class txEngineRoot;
class txCamera;
class CoreEventSystem;
class ECEditorManager;
class ECUndoManager;
class TremblingEditorCore;
class LayoutEditorCore;
class ActionTreeEditorCore;
class ComponentEditorCore;
class EditorCoreRoot : public txSingleton<EditorCoreRoot>, public EditorCoreBase, public txEngineBase, public txEngineEventHandler
{
public:
	EditorCoreRoot();
	virtual ~EditorCoreRoot();
	void init(const HWND& hwnd, const int& colourBits, const int& width, const int& height);
	void preUpdate(const float& elapsedTime);
	void update(const float& elapsedTime);
	void lateUpdate(const float& elapsedTime);
	void render();
	void destroy();
	void resize(const int& width, const int& height);
	void notifyInitDone() { mInitFlag = true; }
	void setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX]);
	// 尽量使用ECRoot的sendEvent来替代EventSystem的pushEvent
	void sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately = true);
	void sendEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void sendDelayEvent(const CORE_EVENT& type, const std::string& param);
	void sendDelayEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>());
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	CoreEventSystem* getCoreEventSystem()			{ return mCoreEventSystem; }
	ECUndoManager* getUndoManager()					{ return mUndoManager; }
	txCamera* getCamera()							{ return mMainCamera; }
	TremblingEditorCore* getTremblingEditorCore()	{ return mTremblingEditorCore; }
	LayoutEditorCore* getLayoutEditorCore()			{ return mLayoutEditorCore; }
	ActionTreeEditorCore* getActionTreeEditorCore()	{ return mActionTreeEditorCore; }
	ComponentEditorCore* getComponentEditorCore()	{ return mComponentEditorCore; }
	virtual void registerEngineEventHandler();
	virtual void unregisterEngineEventHandler();
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList);
	MODAL_DIALOG_RESULT showModalYesNoCancel(const std::string& info);
	MODAL_DIALOG_RESULT showModalYesNo(const std::string& info);
protected:
	void initCamera();
	void checkErrorInfoBuffer();
	MODAL_DIALOG_RESULT showModal(const std::string& info, const int& flag);
protected:
	txEngineRoot*				mEngineRoot;			// 引擎
	CoreEventSystem*			mCoreEventSystem;		// 编辑核心事件系统
	ECUndoManager*				mUndoManager;			// 编辑核心撤销系统
	TremblingEditorCore*		mTremblingEditorCore;
	LayoutEditorCore*			mLayoutEditorCore;
	ActionTreeEditorCore*		mActionTreeEditorCore;
	ComponentEditorCore*		mComponentEditorCore;
	txCamera*					mMainCamera;			// 用于控制视口
	txVector<std::string>		mErrorInfoBuffer;		// 错误信息缓冲,用于保存事件系统创建之前产生的错误信息
	bool						mInitFlag;				// 初始化标记,为false表示还未初始化完成,由外部进行标记
	ModalDialog					mModalDialog;			// 用于询问的模式对话框
	int							mDialogStyle[MDS_MAX];	// 对话框风格
	int							mDialogResult[MDR_MAX];	// 对话框返回结果
};

#endif