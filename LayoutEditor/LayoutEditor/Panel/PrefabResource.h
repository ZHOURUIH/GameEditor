#ifndef _PREFAB_RESOURCE_H_
#define _PREFAB_RESOURCE_H_

#include "EditorPanel.h"

class WindowPrefabManager;
class PrefabResource : public EditorPanel
{
public:
	PrefabResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~PrefabResource();
	void Setup();
	void refreshList(); // 刷新列表

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// 响应刷新按钮的点击事件
	void OnDoubleClick(wxCommandEvent& event);
	void OnRightUp(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuOpenDir(wxCommandEvent& event);
	void OnMenuEditPrefab(wxCommandEvent& event);
protected:
	void ShowMenu(const wxPoint& pt);	// 右键菜单
protected:
	wxAuiToolBar* mToolBar;
	wxListBox* mResourceList;
};

#endif