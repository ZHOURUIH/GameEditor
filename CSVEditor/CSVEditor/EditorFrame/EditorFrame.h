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
	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// �˵����˳�
	void OnOpenFile(wxCommandEvent& event);		// �˵��Ĵ��ļ�
	void OnSaveFile(wxCommandEvent& event);		// �˵��ı����ļ�
	void OnCloseWindow(wxCloseEvent& event);	// ���򷢳��Ĺر��¼�
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