#ifndef _LOADED_LAYOUT_H_
#define _LOADED_LAYOUT_H_

#include "EditorPanel.h"

class LoadedLayout : public EditorPanel
{
public:
	LoadedLayout(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~LoadedLayout();
	void Setup();
	void refreshList(); // 刷新列表
	void setItemImage(const std::string& itemName, bool visible); // true表示切换为显示的图标,false表示切换为隐藏的图标

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// 响应刷新按钮的点击事件
	void OnItemActivated(wxListEvent& event);
	void OnItemRightClick(wxListEvent& event);
	void OnItemKeyDown(wxListEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mResourceList;
	wxImageList* mImageList;
};

#endif