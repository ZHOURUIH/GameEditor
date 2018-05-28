#ifndef _INNER_CHILD_HIERARCHY_H_
#define _INNER_CHILD_HIERARCHY_H_

#include "EditorPanel.h"

class ComplexWindow;
class Window;
class InnerChildHierarchy : public EditorPanel
{
public:
	InnerChildHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~InnerChildHierarchy();
	void setup();
	void displayInnerChild(ComplexWindow* window);
	void setHandleEvent(bool handleEvent) { mHandleEvent = handleEvent; }
	void setSelection(txVector<std::string>& itemLabel);
	void setTreeItemVisibleIcon(Window* window);

	DECLARE_EVENT_TABLE()
	void OnItemSelected(wxTreeEvent& event);
	void OnItemStateClick(wxTreeEvent& event);

protected:
	void initTreeImage();
	int getWindowIconIndex(const std::string& windowName);
	void collapseItem(wxTreeItemId item);
	void expandItem(wxTreeItemId item);
	void appendItem(wxTreeItemId parent, Window* window, int pos = -1);
protected:
	bool mHandleEvent;
	wxTreeCtrl* mHierarchyTree;
	wxImageList* mImageList;
	wxImageList* mStateImageList;
	wxTreeItemId mRootId;
	txMap<std::string, int> mWindowIconIndexList;
	txMap<std::string, wxTreeItemId> mTreeItemMap;
};

#endif