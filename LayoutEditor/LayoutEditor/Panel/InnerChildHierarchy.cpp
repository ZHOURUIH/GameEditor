#include "WindowHeader.h"
#include "Layout.h"
#include "WindowProperty.h"

#include "LayoutEditorCore.h"
#include "WindowHeader.h"

#include "InnerChildHierarchy.h"
#include "PropertyWindow.h"
#include "EditorFrame.h"
#include "EditorCommandHeader.h"

enum
{
	ID_CHILD_TREE,
};

BEGIN_EVENT_TABLE(InnerChildHierarchy, EditorPanel)
EVT_TREE_ITEM_ACTIVATED(ID_CHILD_TREE, InnerChildHierarchy::OnItemSelected)
EVT_TREE_SEL_CHANGED(ID_CHILD_TREE, InnerChildHierarchy::OnItemSelected)
EVT_TREE_STATE_IMAGE_CLICK(ID_CHILD_TREE, InnerChildHierarchy::OnItemStateClick)
END_EVENT_TABLE()

InnerChildHierarchy::InnerChildHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mStateImageList(NULL),
mImageList(NULL),
mHierarchyTree(NULL),
mHandleEvent(true)
{
	setup();
}

InnerChildHierarchy::~InnerChildHierarchy()
{
	TRACE_DELETE(mImageList);
	TRACE_DELETE(mStateImageList);
}

void InnerChildHierarchy::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	mHierarchyTree = new wxTreeCtrl(this, ID_CHILD_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
	bSizer1->Add(mHierarchyTree, 1, wxALL | wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
	Centre(wxBOTH);

	initTreeImage();
}

void InnerChildHierarchy::displayInnerChild(ComplexWindow* window)
{
	mHierarchyTree->DeleteAllItems();
	mTreeItemMap.clear();
	if (window == NULL)
	{
		return;
	}
	mRootId = mHierarchyTree->AddRoot(window->getName());
	mTreeItemMap.insert(window->getName(), mRootId);
	txVector<Window*>& children = window->getInnerChildren();
	int childCount = children.size();
	FOR_STL(children, int i = 0; i < childCount; ++i)
	{
		appendItem(mRootId, children[i]);
	}
	END_FOR_STL(children);
	expandItem(mRootId);
}

void InnerChildHierarchy::setSelection(txVector<std::string>& itemLabel)
{
	setHandleEvent(false);

	mHierarchyTree->UnselectAll();
	int itemCount = itemLabel.size();
	FOR_STL(itemLabel, int i = 0; i < itemCount; ++i)
	{
		auto iter = mTreeItemMap.find(itemLabel[i]);
		if (iter != mTreeItemMap.end())
		{
			mHierarchyTree->SelectItem(iter->second, true);
		}
	}
	END_FOR_STL(itemLabel);

	setHandleEvent(true);
}

void InnerChildHierarchy::setTreeItemVisibleIcon(Window* window)
{
	if (window == NULL)
	{
		return;
	}
	auto iterItem = mTreeItemMap.find(window->getName());
	if (iterItem != mTreeItemMap.end())
	{
		int stateIconIndex = window->getWindowVisible() ? 0 : 1;
		if (stateIconIndex != mHierarchyTree->GetItemState(iterItem->second))
		{
			mHierarchyTree->SetItemState(iterItem->second, stateIconIndex);
		}
	}
}

void InnerChildHierarchy::OnItemSelected(wxTreeEvent& event)
{
	// 如果不接收wx的事件则直接返回
	if (!mHandleEvent)
	{
		return;
	}

	txVector<Window*> selectWindowList;
	wxArrayTreeItemIds seletionIDList;
	int count = mHierarchyTree->GetSelections(seletionIDList);
	Window* windFind = NULL;
	if (count > 0)
	{
		wxTreeItemId selectedItem = seletionIDList[0];
		// 不能选中根节点
		if (selectedItem != mRootId)
		{
			std::string secletString = mHierarchyTree->GetItemText(selectedItem).ToStdString();
			windFind = mLayoutEditorCore->getCurLayout()->getWindow(secletString);
		}
	}
	// 如果在处理选中事件时将当前子窗口视图清空了,则会发生崩溃错误
	if (windFind != NULL)
	{
		mLayoutEditorCore->setSelection(windFind);
	}
}

void InnerChildHierarchy::OnItemStateClick(wxTreeEvent& event)
{
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout != NULL)
	{
		wxTreeItemId itemId = event.GetItem();
		wxString itemText = mHierarchyTree->GetItemText(itemId);
		Window* window = layout->getWindow(itemText.ToStdString());
		if (window != NULL)
		{
			bool value = !window->getWindowVisible();
			CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
			cmdProperty->mPropertyName = txProperty::getPropertyName<WindowPropertyVisible>();
			cmdProperty->mPropertyValue = txStringUtility::boolToString(value);
			cmdProperty->mOperator = this;
			mCommandSystem->pushCommand(cmdProperty, window);
		}
	}
}

void InnerChildHierarchy::initTreeImage()
{
	// 窗口图标
	txMap<std::string, wxBitmap> windowIconList;
	windowIconList.insert("Button", wxBITMAP(IDB_BUTTON));
	windowIconList.insert("CheckBox", wxBITMAP(IDB_CHECKBOX));
	windowIconList.insert("EditBox", wxBITMAP(IDB_EDITBOX));
	windowIconList.insert("EffectRingWindow", wxBITMAP(IDB_EFFECTRINGWINDOW));
	windowIconList.insert("NumberWindow", wxBITMAP(IDB_NUMBERWINDOW));
	windowIconList.insert("ProgressWindow", wxBITMAP(IDB_PROGRESSWINDOW));
	windowIconList.insert("TextureAnimWindow", wxBITMAP(IDB_TEXTUREANIMWINDOW));
	windowIconList.insert("TextureWindow", wxBITMAP(IDB_TEXTUREWINDOW));
	windowIconList.insert("TextWindow", wxBITMAP(IDB_TEXTWINDOW));
	windowIconList.insert("Window", wxBITMAP(IDB_WINDOW));
	windowIconList.insert("VideoWindow", wxBITMAP(IDB_VIDEO_WINDOW));
	mImageList = TRACE_NEW(wxImageList, mImageList, 16, 16);
	auto iter = windowIconList.begin();
	auto iterEnd = windowIconList.end();
	FOR_STL(windowIconList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		mImageList->Add(iter->second);
		mWindowIconIndexList.insert(iter->first, i);
	}
	END_FOR_STL(windowIconList);
	mHierarchyTree->SetImageList(mImageList);

	// 窗口显示状态图标
	txMap<int, wxBitmap> windowStateIconList;
	windowStateIconList.insert(0, wxBITMAP(IDB_WINDOW_SHOW));
	windowStateIconList.insert(1, wxBITMAP(IDB_WINDOW_HIDE));
	mStateImageList = TRACE_NEW(wxImageList, mStateImageList, 16, 16);
	auto iterState = windowStateIconList.begin();
	auto iterStateEnd = windowStateIconList.end();
	FOR_STL(windowStateIconList, ; iterState != iterStateEnd; ++iterState)
	{
		mStateImageList->Add(iterState->second);
	}
	END_FOR_STL(windowStateIconList);
	mHierarchyTree->SetStateImageList(mStateImageList);
}

int InnerChildHierarchy::getWindowIconIndex(const std::string& windowName)
{
	auto iter = mWindowIconIndexList.find(windowName);
	if (iter != mWindowIconIndexList.end())
	{
		return iter->second;
	}
	return -1;
}

void InnerChildHierarchy::collapseItem(wxTreeItemId item)
{
	if (!item.IsOk())
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(item, cookie);
	// 如果有子节点,就先折叠所有的子节点
	while (childItem.IsOk())
	{
		collapseItem(childItem);
		childItem = mHierarchyTree->GetNextChild(item, cookie);
	}
	// 折叠完所有子节点后,折叠自己
	mHierarchyTree->Collapse(item);
}

void InnerChildHierarchy::expandItem(wxTreeItemId item)
{
	if (!item.IsOk())
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(item, cookie);
	// 如果有子节点,就先展开所有的子节点
	while (childItem.IsOk())
	{
		expandItem(childItem);
		childItem = mHierarchyTree->GetNextChild(item, cookie);
	}
	// 展开完所有子节点后,展开自己
	mHierarchyTree->Expand(item);
}

void InnerChildHierarchy::appendItem(wxTreeItemId parent, Window* window, int pos)
{
	if (window == NULL)
	{
		return;
	}
	// 先把自己添加到节点上,然后再把自己的所有子窗口添加到自己的节点上
	int iconIndex = getWindowIconIndex(window->getTypeName());
	wxTreeItemId windowId = NULL;
	if (pos >= 0 && pos < (int)mHierarchyTree->GetChildrenCount(parent, false))
	{
		windowId = mHierarchyTree->InsertItem(parent, pos, window->getName(), iconIndex);
	}
	else
	{
		windowId = mHierarchyTree->AppendItem(parent, window->getName(), iconIndex);
	}
	int stateIconIndex = window->getWindowVisible() ? 0 : 1;
	mHierarchyTree->SetItemState(windowId, stateIconIndex);
	mTreeItemMap.insert(window->getName(), windowId);
	auto& childList = window->getChildren();
	int childWindowNumber = childList.size();
	FOR_STL(childList, int i = 0; i < childWindowNumber; ++i)
	{
		appendItem(windowId, childList[i]);
	}
	END_FOR_STL(childList);
}