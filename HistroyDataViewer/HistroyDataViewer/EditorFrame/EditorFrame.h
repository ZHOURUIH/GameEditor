#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "EditorUtility.h"

class MainViewerPanel;
class HistoryDatePanel;
class DataBase;
class EditorFrame : public wxFrame
{
public:
	EditorFrame(wxString title, wxSize size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame(){}
	// 刷新全部的可选中菜单和可选中工具按钮的选中状态
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	static void errorMessage(const std::string& str);
	void notifyWindowShow(wxWindow* window, const bool& show);
	DataBase* getDataBase() { return mDataBase; }
	MainViewerPanel* getMainViewerPanel() { return mMainViewerPanel; }
	HistoryDatePanel* getHistoryDatePanel() { return mHistoryDatePanel; }
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event); // 响应菜单的退出事件
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
protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mWindowToolBar;
	wxAuiManager mAuiManager;
	MainViewerPanel* mMainViewerPanel;
	HistoryDatePanel* mHistoryDatePanel;
	DataBase* mDataBase;
	std::string mAppNormalName;
	std::string mTableName;
	std::map<wxWindow*, int> mWindowIDList;
};

#endif