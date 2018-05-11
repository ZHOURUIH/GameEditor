#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "EventHandler.h"
#include "CommandReceiver.h"
#include "EditorUtility.h"
#include "EditorCoreBase.h"

class CommandSystem;
class EditorCore;
class DownloadPanel;
class EditorApp;
class EditorFrame : public wxFrame, public EventHandler, public CommandReceiver, public EditorCoreBase
{
public:
	EditorFrame(EditorApp* app, wxString title, wxPoint position, wxSize size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame();
	// 刷新全部的可选中菜单和可选中工具按钮的选中状态
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
	virtual void registerEditorCoreEventHandler();
	virtual void unregisterEditorCoreEventHandler();
	virtual void onEditorCoreEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params);
	void showPanel(wxWindow* panel, bool show);
	void start(const txVector<std::pair<std::string, std::string>>& uploadFileList);
	txMap<wxWindow*, int>& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	wxWindow* getWindow(const std::string& name);
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }
	EditorCore* getEditorCore() { return mEditorCore; }
	void setAutoStartGame(const bool& autoStart) { mAutoStartGame = autoStart; }
	void setAutoCloseUploaded(const bool& autoClose) { mAutoClose = autoClose; }
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// 响应菜单的退出事件
	void OnCloseWindow(wxCloseEvent& event);	// 响应程序发出的关闭事件
	void OnPanelShow(wxCommandEvent& event);
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
	void uploadDone();
	void downloadDone(bool launch);
protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mWindowToolBar;
	wxAuiManager mAuiManager;
	txMap<wxWindow*, int> mWindowIDList;
	txMap<std::string, wxWindow*> mWindowList;
	EditorApp* mApp;
	EditorCore* mEditorCore;
	DownloadPanel* mDownloadPanel;
	bool mRunning;
	bool mUpdateDone;
	bool mAutoStartGame;	// 下载完毕后是否自动启动游戏
	bool mAutoClose;		// 上传完毕后是否自动关闭程序
};

#endif