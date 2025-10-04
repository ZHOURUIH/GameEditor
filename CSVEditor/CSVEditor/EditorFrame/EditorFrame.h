#pragma once

#include "FrameHeader.h"

class MainListWindow;
class CSVEditor;
class EditorFrame : public wxFrame
{
public:
	EditorFrame(wxString title, wxSize size);
	virtual ~EditorFrame() = default;
	void init();
	void destroy();
	void setup();
	// 刷新全部的可选中菜单和可选中工具按钮的选中状态
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render() {}
	void KeyProcess() {}
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// 菜单的退出
	void OnOpenFile(wxCommandEvent& event);		// 菜单的打开文件
	void OnNewFile(wxCommandEvent& event);
	void OnSaveFile(wxCommandEvent& event);		// 菜单的保存文件
	void OnCopy(wxCommandEvent& event);			// 菜单的复制
	void OnPaste(wxCommandEvent& event);		// 菜单的粘贴
	void OnUndo(wxCommandEvent& event);			// 菜单的撤销
	void OnRedo(wxCommandEvent& event);			// 菜单的重做
	void OnAddRowToFirst(wxCommandEvent& event);
	void OnAddRowToEnd(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);	// 程序发出的关闭事件
protected:
	static void onDirty();
	static void onUndoChanged();
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus() {}
	void CreateEditorCore() {}
	void RefreshAllResource() {}
protected:
	wxTimer* mTimer = nullptr;
	wxAuiManager mAuiManager = nullptr;
};