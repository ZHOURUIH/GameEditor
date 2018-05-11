#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "EditorUtility.h"

class MainViewerWindow;
class TableBinaryResourceWindow;
class TableTxtResourceWindow;
class SupportedTableWindow;
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
	void notifyOpenTable(std::string tableName);
	void setDataModified(bool modified);
	void saveData();
	bool isModified() { return mDataModified; }
	DataBase* getDataBase() { return mDataBase; }
	MainViewerWindow* getMainViewerWindow() { return mMainViewerWindow; }
	TableBinaryResourceWindow* getTableBinaryResourceWindow() { return mTableBinaryResourceWindow; }
	TableTxtResourceWindow* getTableTxtResourceWindow() { return mTableTxtResourceWindow; }
	SupportedTableWindow* getSupportedTableWindow() { return mSupportedTableWindow; }
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event); // 响应菜单的退出事件
	void OnCloseWindow(wxCloseEvent& event);	// 响应程序发出的关闭事件
	void OnSave(wxCommandEvent& event);
	void OnEditTable(wxCommandEvent& event);
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
	void refreshTitle();

protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mFileToolBar;
	wxAuiManager mAuiManager;
	MainViewerWindow* mMainViewerWindow;
	TableBinaryResourceWindow* mTableBinaryResourceWindow;
	TableTxtResourceWindow* mTableTxtResourceWindow;
	SupportedTableWindow* mSupportedTableWindow;
	DataBase* mDataBase;
	std::string mAppNormalName;
	std::string mTableName;
	bool mDataModified;			// 表格数据是否已经修改过
};

#endif