#ifndef _ACTION_TREE_RESOURCE_H_
#define _ACTION_TREE_RESOURCE_H_

#include "EditorPanel.h"

class ActionTreeResource : public EditorPanel
{
public:
	ActionTreeResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ActionTreeResource(){}
	void setup();
	void refreshActionTypeList();
	void refreshList();
	void notifyActionTreeActived();
	void notifyActionTreeAdded(const std::string& name);
	void notifyActionTreeDeleted(const std::string& name);
	void notifyActionTreeRenamed(const std::string& oldName, const std::string& newName);
	void notifyActionSelectionChanged();
	void notifyActionAdded(const std::string& name, const std::string& parentName);
	void notifyActionDeleted(const std::string& name);
	void notifyActionTreeModified(const bool& modified);
	DECLARE_EVENT_TABLE()
	void OnRefreshTreeResource(wxCommandEvent& event);
	void OnNewActionTree(wxCommandEvent& event);
	void OnDeleteActionTree(wxCommandEvent& event);
	void OnSaveActionTree(wxCommandEvent& event);
	void OnTypeDoubleClick(wxListEvent& event);
	void OnActionTreeSelected(wxListEvent& event);
	void OnRefreshTreeView(wxCommandEvent& event);
	void OnDeleteAction(wxCommandEvent& event);
	void OnLayoutText(wxCommandEvent& event);
	void OnActionTreePlay(wxCommandEvent& event);
	void OnActionTreePlayInverse(wxCommandEvent& event);
	void OnActionTreePause(wxCommandEvent& event);
	void OnActionTreeStop(wxCommandEvent& event);
	void OnActionSelected(wxTreeEvent& event);
protected:
	void initTreeImage();
	int getActionIconIndex(const std::string& actionType);
	void appendItem(const wxTreeItemId& parent, txAction* action, const int& pos = -1);
	void expandItem(const wxTreeItemId& item);
	void notifyItemDelete(wxTreeItemId item);	// 通知即将删除某一项,将该项从mItemList中移除
protected:
	wxListCtrl*		mActionTypeList;
	wxListCtrl*		mActionTreeList;
	wxTreeCtrl*		mActionTreeView;
	wxAuiToolBar*	mActionTreeToolBar;
	wxAuiToolBar*	mActionToolBar;
	wxTextCtrl*		mActionTreeLayoutText;
	wxTreeItemId	mRootId;
	wxImageList*	mTypeImageList;
	txMap<std::string, int> mTypeIconIndexList;
	txMap<std::string, wxTreeItemId> mItemList;
	bool mHandleListEvent;
};

#endif