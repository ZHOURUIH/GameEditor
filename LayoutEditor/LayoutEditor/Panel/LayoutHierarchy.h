#ifndef _LAYOUT_HIERARCHY_H_
#define _LAYOUT_HIERARCHY_H_

#include "EditorPanel.h"

class Layout;
class Window;
class LayoutHierarchy : public EditorPanel
{
public:
	LayoutHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~LayoutHierarchy();
	void setup();

	// 显示布局的层级视图
	void displayLayout(::Layout* layout);

	// 设置指定项为选中状态,如果该项已经是选中了,则不做任何操作
	void setSelection(txVector<std::string>& itemLabel);
	void unSelectAll();
	void setHandleEvent(bool handleEvent) { mHandleEvent = handleEvent; }
	void setVisibleIcon(Window* window);
	void addItem(Window* window, int pos = -1);
	void deleteItem(const std::string& label);
	void renameItem(const std::string& label, const std::string& newLabel);
	void moveItemPosition(const std::string& parentLabel, Window* window, int oldPosition, int newPosition);
	void refreshChildItem(Window* window);
	void resetParentItem(Window* window);	// 刷新window的父节点

	DECLARE_EVENT_TABLE()
	void OnItemSelected(wxTreeEvent& event);
	void OnItemBeginEdit(wxTreeEvent& event);
	void OnItemEndEdit(wxTreeEvent& event);
	void OnItemKeyDown(wxTreeEvent& event);
	void OnItemRightClick(wxTreeEvent& event);
	void OnMenuCollapse(wxCommandEvent& event);
	void OnMenuExpand(wxCommandEvent& event);
	void OnMenuCreatePrefab(wxCommandEvent& event);
	void OnMenuCreateTemplate(wxCommandEvent& event);
	void OnMenuCopy(wxCommandEvent& event);
	void OnMenuCopyWithChildren(wxCommandEvent& event);
	void OnMenuCut(wxCommandEvent& event);
	void OnMenuPaste(wxCommandEvent& event);
	void OnMenuLockExpand(wxCommandEvent& event);
	void OnRefreshButton(wxCommandEvent& event);
	void OnSearch(wxCommandEvent& event);   // 搜索
	void OnSearchListSelect(wxListEvent& event);
	void OnItemStateClick(wxTreeEvent& event);
protected:
	// 添加到指定的ID的项中
	void appendItem(wxTreeItemId parent, Window* window, int pos = -1);
	void initTreeImage();
	int getWindowIconIndex(std::string windowName);
	void collapseItem(wxTreeItemId item);
	void expandItem(wxTreeItemId item);
	void notifyItemDelete(wxTreeItemId item);	// 通知即将删除某一项
	void showMenu(const wxPoint& pt);			// 右键菜单
	void showSearch(const std::string& value);	// 显示搜索内容
protected:
	wxAuiToolBar*	mAuiToolBar;
	wxTreeCtrl*		mHierarchyTree;
	wxListCtrl*		mSearchResultList;
	wxTextCtrl*		mSearchTextCtrl;
	wxTreeItemId	mRootId;
	wxString		mPreString;
	wxImageList*	mImageList;
	wxImageList*	mStateImageList;
	txMap<std::string, int> mWindowIconIndexList;
	txSet<wxTreeItemId> mLockExpandItemList;		// 始终锁定不展开的项
	txMap<std::string, wxTreeItemId> mTreeItemMap;
	bool mHandleEvent;			// 是否处理wx触发的事件
};

#endif