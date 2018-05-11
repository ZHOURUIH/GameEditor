#ifndef _TALBE_BINARY_RESOURCE_WINDOW_H_
#define _TALBE_BINARY_RESOURCE_WINDOW_H_

#include "DataDefine.h"
#include "EditorUtility.h"

class TableBinaryResourceWindow : public wxPanel
{
public:
	TableBinaryResourceWindow(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~TableBinaryResourceWindow();
	void setup();
	void refresh(); // 刷新列表

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// 响应刷新按钮的点击事件
	void OnDoubleClick(wxCommandEvent& event);
	void OnShow(wxShowEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListBox* mResourceList;
	EditorFrame* mEditorFrame;
};

#endif