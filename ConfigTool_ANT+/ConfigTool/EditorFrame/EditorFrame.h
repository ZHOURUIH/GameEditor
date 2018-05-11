#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "CTEventHandler.h"
#include "txCommandReceiver.h"
#include "EditorUtility.h"

class ConfigToolCore;
class SpeedDataPanel;
class HeartRatePanel;
class txCommandSystem;
class EditorFrame : public wxFrame, public CTEventHandler, public txCommandReceiver
{
public:
	EditorFrame(wxString title, wxSize size);
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
	virtual void onEditorCoreEvent(const CORE_EVENT_TYPE& type, std::vector<std::string>& params);
	void showPanel(wxWindow* panel, bool show);
	txMap<wxWindow*, int>& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	wxWindow* getWindow(std::string name);
	HeartRatePanel* getHeartRatePanel() { return mHeartRatePanel; }
	ConfigToolCore* getConfigTool() { return mConfigToolCore; }
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// 响应菜单的退出事件
	void OnCloseWindow(wxCloseEvent& event);	// 响应程序发出的关闭事件
	void OnHeartRatePanel(wxCommandEvent& event);
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mWindowToolBar;
	wxAuiManager mAuiManager;
	HeartRatePanel* mHeartRatePanel;
	ConfigToolCore* mConfigToolCore;
	txCommandSystem* mCommandSystem;
	txMap<wxWindow*, int> mWindowIDList;
	txMap<std::string, wxWindow*> mWindowList;
};

#endif