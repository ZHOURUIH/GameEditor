#pragma once

#include "EditorUtility.h"

class MainListWindow;
class CSVEditor;
class EditorFrame : public wxFrame
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
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// 菜单的退出
	void OnOpenFile(wxCommandEvent& event);		// 菜单的打开文件
	void OnSaveFile(wxCommandEvent& event);		// 菜单的保存文件
	void OnCloseWindow(wxCloseEvent& event);	// 程序发出的关闭事件
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
protected:
	wxTimer* mTimer = nullptr;
	wxMenu* mFileMenu = nullptr;
	wxAuiManager mAuiManager = nullptr;
	MainListWindow* mMainListWindow = nullptr;
	CSVEditor* mCSVEditor = nullptr;
};