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
	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render() {}
	void KeyProcess() {}
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// �˵����˳�
	void OnOpenFile(wxCommandEvent& event);		// �˵��Ĵ��ļ�
	void OnNewFile(wxCommandEvent& event);
	void OnSaveFile(wxCommandEvent& event);		// �˵��ı����ļ�
	void OnCopy(wxCommandEvent& event);			// �˵��ĸ���
	void OnPaste(wxCommandEvent& event);		// �˵���ճ��
	void OnUndo(wxCommandEvent& event);			// �˵��ĳ���
	void OnRedo(wxCommandEvent& event);			// �˵�������
	void OnAddRowToFirst(wxCommandEvent& event);
	void OnAddRowToEnd(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);	// ���򷢳��Ĺر��¼�
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