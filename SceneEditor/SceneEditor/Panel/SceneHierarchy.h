#ifndef _SCENE_HIERARCHY_H_
#define _SCENE_HIERARCHY_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class ECScene;
class SceneHierarchy : public EditorPanel
{
public:
	SceneHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~SceneHierarchy(){}
	void setup();
	void showScene(ECScene* scene);
	void setSelections(txVector<std::string>& itemLabel);
	void setHandleEvent(bool handleEvent) { mHandleEvent = handleEvent; }
	void renameItem(const std::string& label, const std::string& newLabel);

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);
	void OnItemSelected(wxTreeEvent& event);
	void OnItemBeginEdit(wxTreeEvent& event);
	void OnItemEndEdit(wxTreeEvent& event);
	void OnItemKeyDown(wxTreeEvent& event);
	void OnItemRClick(wxTreeEvent& event);
protected:
	void addComponent(const std::string& type, const std::string& name);
protected:
	wxAuiToolBar* mAuiToolBar;
	wxTreeCtrl* mHierarchyTree;
	std::string mPreString;
	txMap<int, std::string> mDeleteComponentNameList;	// 用于保存在弹出菜单时,菜单ID对应的删除组件的名字
	txMap<std::string, wxTreeItemId> mTreeItemMap;
	bool mHandleEvent;
};

#endif