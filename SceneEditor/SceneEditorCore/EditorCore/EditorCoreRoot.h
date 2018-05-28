#ifndef _EC_ROOT_H_
#define _EC_ROOT_H_

#include "txSingleton.h"
#include "txEngineBase.h"

#include "CommonDefine.h"
#include "EditorCoreBase.h"

class txEngineRoot;
class CoreEventSystem;
class ECEditorManager;
class ECSceneManager;
class ECUndoManager;
class SceneEditorCore;
class ComponentEditorCore;
class EditorCoreRoot : public txSingleton<EditorCoreRoot>, public EditorCoreBase, public txEngineBase
{
public:
	EditorCoreRoot();
	virtual ~EditorCoreRoot(){ destroy(); }
	void init(const HWND& hwnd, const int& colourBits, const int& width, const int& height);
	void preUpdate(float elapsedTime);
	void update(float elapsedTime);
	void lateUpdate(float elapsedTime);
	void render();
	void destroy();
	void notifyInitDone(){ mInitFlag = true; }
	void setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX]);
	void logError(const std::string& info);
	// 尽量使用EditorCoreRoot的sendEvent来替代EventSystem的pushEvent
	void sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately = true);
	void sendEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void sendDelayEvent(const CORE_EVENT& type, const std::string& param){ sendEvent(type, param, false); }
	void sendDelayEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>()){ sendEvent(type, params, false); }
	MODAL_DIALOG_RESULT showModalYesNoCancel(const std::string& info);
	MODAL_DIALOG_RESULT showModalYesNo(const std::string& info);

	CoreEventSystem* getCoreEventSystem() { return mCoreEventSystem; }
	txEngineRoot* getEngineRoot() { return mEngineRoot; }
	ECEditorManager* getEditorManager() { return mEditorManager; }
	ECSceneManager* getEditorSceneManager() { return mEditorSceneManager; }
	ECUndoManager* getUndoManager() { return mUndoManager; }
	SceneEditorCore* getSceneEditorCore() { return mSceneEditorCore; }
	ComponentEditorCore* getComponentEditorCore() { return mComponentEditorCore; }
protected:
	void checkErrorInfoBuffer();
	MODAL_DIALOG_RESULT showModal(const std::string& info, const int& flag);
protected:
	txEngineRoot* mEngineRoot;							// 引擎
	CoreEventSystem* mCoreEventSystem;					// 事件系统
	ECEditorManager* mEditorManager;					// 编辑体管理器
	ECSceneManager* mEditorSceneManager;				// 场景管理器
	ECUndoManager* mUndoManager;						// 撤销管理器
	SceneEditorCore* mSceneEditorCore;
	ComponentEditorCore* mComponentEditorCore;
	txVector<std::string> mErrorInfoBuffer;				// 错误信息缓冲区,仅在事件系统初始化完成之前使用,用于保存事件系统初始化完成之前所产生的错误信息
	bool mInitFlag;										// 初始化标记,为false表示还未初始化完成
	ModalDialog mModalDialog;							// 用于询问的模式对话框
	int mDialogStyle[MDS_MAX];							// 对话框风格
	int mDialogResult[MDR_MAX];							// 对话框返回结果
};

#endif