#ifndef _LAYOUT_RESOURCE_H_
#define _LAYOUT_RESOURCE_H_

#include "EditorPanel.h"

class LayoutResource : public EditorPanel
{
public:
	LayoutResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~LayoutResource();
	void Setup();
	void refreshList(); // 刷新列表

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// 响应刷新按钮的点击事件
	void OnDoubleClick(wxCommandEvent& event);
	void OnRightUp(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);
	void OnOpenFile(wxCommandEvent& event);
	void OnOpenDir(wxCommandEvent& event);
protected:
	void ShowMenu(const wxPoint& pt);	// 右键菜单
protected:
	wxAuiToolBar* mToolBar;
	wxListBox* mResourceList;
};

#endif