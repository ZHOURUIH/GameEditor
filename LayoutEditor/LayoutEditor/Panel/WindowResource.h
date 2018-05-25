#ifndef _WINDOW_RESOURCE_H_
#define _WINDOW_RESOURCE_H_

#include "EditorPanel.h"

class WindowResource : public EditorPanel
{
public:
	WindowResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~WindowResource();
	void Setup();
	void refreshList(); // 刷新列表

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// 响应刷新按钮的点击事件
	void OnDoubleClick(wxListEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mResourceList;
	wxImageList* mImageList;
	txMap<WINDOW_TYPE, int> mWindowIconIndexList;
};

#endif