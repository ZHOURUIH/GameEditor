#include "txAction.h"
#include "txActionTree.h"
#include "txActionTreeManager.h"
#include "txActionFactoryManager.h"
#include "txActionFactory.h"
#include "Layout.h"
#include "LayoutManager.h"

#include "ECLog.h"
#include "ActionTreeEditorCore.h"

#include "ActionTreeResource.h"

enum
{
	ID_REFRESH_TREE_RESOURCE,
	ID_NEW_ACTION_TREE,
	ID_DELETE_ACTION_TREE,
	ID_SAVE_ACTION_TREE,
	ID_ACTION_TYPE_LIST,
	ID_ACTION_TREE_LIST,
	ID_REFRESH_TREE_VIEW,
	ID_DELETE_ACTION,
	ID_ACTION_TREE_LAYOUT_TEXT,
	ID_ACTION_TREE_PLAY,
	ID_ACTION_TREE_PLAY_INVERSE,
	ID_ACTION_TREE_PAUSE,
	ID_ACTION_TREE_STOP,
	ID_ACTION_TREE_VIEW,
};

BEGIN_EVENT_TABLE(ActionTreeResource, EditorPanel)
EVT_TOOL(ID_REFRESH_TREE_RESOURCE, ActionTreeResource::OnRefreshTreeResource)
EVT_TOOL(ID_NEW_ACTION_TREE, ActionTreeResource::OnNewActionTree)
EVT_TOOL(ID_DELETE_ACTION_TREE, ActionTreeResource::OnDeleteActionTree)
EVT_TOOL(ID_SAVE_ACTION_TREE, ActionTreeResource::OnSaveActionTree)
EVT_LIST_ITEM_ACTIVATED(ID_ACTION_TYPE_LIST, ActionTreeResource::OnTypeDoubleClick)
EVT_LIST_ITEM_ACTIVATED(ID_ACTION_TREE_LIST, ActionTreeResource::OnActionTreeSelected)
EVT_TOOL(ID_REFRESH_TREE_VIEW, ActionTreeResource::OnRefreshTreeView)
EVT_TOOL(ID_DELETE_ACTION, ActionTreeResource::OnDeleteAction)
EVT_TREE_ITEM_ACTIVATED(ID_ACTION_TREE_VIEW, ActionTreeResource::OnActionSelected)
EVT_TREE_SEL_CHANGED(ID_ACTION_TREE_VIEW, ActionTreeResource::OnActionSelected)
EVT_TEXT_ENTER(ID_ACTION_TREE_LAYOUT_TEXT, ActionTreeResource::OnLayoutText)
EVT_TOOL(ID_ACTION_TREE_PLAY, ActionTreeResource::OnActionTreePlay)
EVT_TOOL(ID_ACTION_TREE_PLAY_INVERSE, ActionTreeResource::OnActionTreePlayInverse)
EVT_TOOL(ID_ACTION_TREE_PAUSE, ActionTreeResource::OnActionTreePause)
EVT_TOOL(ID_ACTION_TREE_STOP, ActionTreeResource::OnActionTreeStop)

END_EVENT_TABLE()

ActionTreeResource::ActionTreeResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mHandleListEvent(true)
{
	setup();
}

void ActionTreeResource::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	mActionTypeList = new wxListCtrl(this, ID_ACTION_TYPE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_NO_HEADER);
	bSizer1->Add(mActionTypeList, 1, wxALL | wxEXPAND, 5);

	mActionTreeToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mActionTreeToolBar->SetToolBitmapSize(wxSize(16, 16));
	mActionTreeToolBar->AddTool(ID_REFRESH_TREE_RESOURCE, wxT("刷新资源"), wxBITMAP(IDB_REFRESH), wxT("刷新资源"));
	mActionTreeToolBar->AddTool(ID_NEW_ACTION_TREE, wxT("新建行为树"), wxBITMAP(IDB_NEW_ACTION_TREE), wxT("新建行为树"));
	mActionTreeToolBar->AddTool(ID_DELETE_ACTION_TREE, wxT("删除行为树"), wxBITMAP(IDB_DELETE_ACTION_TREE), wxT("删除行为树"));
	mActionTreeToolBar->AddTool(ID_SAVE_ACTION_TREE, wxT("保存行为树"), wxBITMAP(IDB_SAVE), wxT("保存行为树"));
	mActionTreeToolBar->Realize();
	bSizer1->Add(mActionTreeToolBar, 0, wxALL, 5);
	
	mActionTreeList = new wxListCtrl(this, ID_ACTION_TREE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	mActionTreeList->InsertColumn(0, wxT(""), 0, -1);
	bSizer1->Add(mActionTreeList, 1, wxALL | wxEXPAND, 5);

	mActionToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mActionToolBar->SetToolBitmapSize(wxSize(16, 16));
	mActionToolBar->AddTool(ID_REFRESH_TREE_VIEW, wxT("刷新行为树"), wxBITMAP(IDB_REFRESH), wxT("刷新行为树"));
	mActionToolBar->AddTool(ID_DELETE_ACTION, wxT("删除行为"), wxBITMAP(IDB_DELETE_ACTION), wxT("删除行为"));
	mActionToolBar->AddLabel(wxID_ANY, wxT("行为树布局:"));
	mActionTreeLayoutText = new wxTextCtrl(mActionToolBar, ID_ACTION_TREE_LAYOUT_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	mActionToolBar->AddControl(mActionTreeLayoutText);
	mActionToolBar->AddTool(ID_ACTION_TREE_PLAY, wxT("正向播放"), wxBITMAP(IDB_PLAY), wxT("正向播放"));
	mActionToolBar->AddTool(ID_ACTION_TREE_PLAY_INVERSE, wxT("反向播放"), wxBITMAP(IDB_PLAY_INVERSE), wxT("反向播放"));
	mActionToolBar->AddTool(ID_ACTION_TREE_PAUSE, wxT("暂停"), wxBITMAP(IDB_PAUSE), wxT("暂停"));
	mActionToolBar->AddTool(ID_ACTION_TREE_STOP, wxT("停止"), wxBITMAP(IDB_STOP), wxT("停止"));
	mActionToolBar->Realize();
	bSizer1->Add(mActionToolBar, 0, wxALL, 5);

	mActionTreeView = new wxTreeCtrl(this, ID_ACTION_TREE_VIEW, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxTR_MULTIPLE);
	bSizer1->Add(mActionTreeView, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	initTreeImage();
}

void ActionTreeResource::initTreeImage()
{
	txMap<std::string, wxBitmap> windowIconList;
	windowIconList.insert(TOSTRING(txAction), wxBITMAP(IDB_ACTION_BASE));
	windowIconList.insert(TOSTRING(ActionWindow), wxBITMAP(IDB_ACTION_WINDOW));
	windowIconList.insert(TOSTRING(ActionWindowAlpha), wxBITMAP(IDB_ACTION_ALPHA));
	windowIconList.insert(TOSTRING(ActionWindowHSL), wxBITMAP(IDB_ACTION_HSL));
	windowIconList.insert(TOSTRING(ActionWindowMove), wxBITMAP(IDB_ACTION_MOVE));
	windowIconList.insert(TOSTRING(ActionWindowRotate), wxBITMAP(IDB_ACTION_ROTATE));
	windowIconList.insert(TOSTRING(ActionWindowScale), wxBITMAP(IDB_ACTION_SCALE));
	mTypeImageList = TRACE_NEW(wxImageList, mTypeImageList, 16, 16);
	auto iterBitmap = windowIconList.begin();
	auto iterBitmapEnd = windowIconList.end();
	FOR_STL(windowIconList, ; iterBitmap != iterBitmapEnd; ++iterBitmap)
	{
		int imageIndex = mTypeImageList->Add(iterBitmap->second);
		mTypeIconIndexList.insert(iterBitmap->first, imageIndex);
	}
	END_FOR_STL(windowIconList);
	// 行为类型列表和行为树视图使用同一图标列表
	mActionTypeList->SetImageList(mTypeImageList, 0);
	mActionTreeView->SetImageList(mTypeImageList);
}

void ActionTreeResource::refreshActionTypeList()
{
	mActionTypeList->DeleteAllItems();
	auto& factoryList = mActionFactoryManager->getFactoryList();
	auto iterFactory = factoryList.begin();
	auto iterFactoryEnd = factoryList.end();
	FOR_STL(factoryList, ; iterFactory != iterFactoryEnd; ++iterFactory)
	{
		int imageIndex = getActionIconIndex(iterFactory->second->getType());
		mActionTypeList->InsertItem(mActionTreeList->GetItemCount(), iterFactory->second->getType(), imageIndex);
	}
	END_FOR_STL(factoryList);
}

void ActionTreeResource::refreshList()
{
	mActionTreeList->DeleteAllItems();
	auto& actionTreeList = mActionTreeManager->getActionTreeList();
	auto iterTree = actionTreeList.begin();
	auto iterTreeEnd = actionTreeList.end();
	FOR_STL(actionTreeList, ; iterTree != iterTreeEnd; ++iterTree)
	{
		mActionTreeList->InsertItem(mActionTreeList->GetItemCount(), iterTree->second->getName());
	}
	END_FOR_STL(actionTreeList);
}

void ActionTreeResource::notifyActionTreeActived()
{
	mItemList.clear();
	mActionTreeView->DeleteAllItems();
	mActionTreeLayoutText->Clear();
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		mRootId = mActionTreeView->AddRoot(curTree->getName());
		mItemList.insert(curTree->getName(), mRootId);
		txAction* rootAction = curTree->getRootAction();
		if (rootAction != NULL)
		{
			appendItem(mRootId, rootAction);
		}
		expandItem(mRootId);
		if (curTree->getUserData() != NULL)
		{
			::Layout* layout = (::Layout*)(curTree->getUserData());
			mActionTreeLayoutText->SetValue(layout->getName());
		}
	}
}

void ActionTreeResource::notifyActionTreeAdded(const std::string& name)
{
	mActionTreeList->InsertItem(mActionTreeList->GetItemCount(), name);
}

void ActionTreeResource::notifyActionTreeDeleted(const std::string& name)
{
	mActionTreeList->DeleteItem(mActionTreeList->FindItem(-1, name));
}

void ActionTreeResource::notifyActionTreeRenamed(const std::string& oldName, const std::string& newName)
{
	mActionTreeList->SetItemText(mActionTreeList->FindItem(-1, oldName), newName);
}

void ActionTreeResource::notifyActionSelectionChanged()
{
	mHandleListEvent = false;
	mActionTreeView->UnselectAll();
	txAction* action = mActionTreeEditorCore->getCurAction();
	if (action != NULL)
	{
		auto iterItem = mItemList.find(action->getName());
		if (iterItem != mItemList.end())
		{
			mActionTreeView->SelectItem(iterItem->second);
		}
	}
	
	mHandleListEvent = true;
}

void ActionTreeResource::notifyActionAdded(const std::string& name, const std::string& parentName)
{
	txActionTree* actionTree = mActionTreeEditorCore->getCurActionTree();
	if (actionTree == NULL)
	{
		return;
	}
	txAction* action = actionTree->getAction(name);
	wxTreeItemId parentID = mRootId;
	auto iterItem = mItemList.find(parentName);
	if (iterItem != mItemList.end())
	{
		parentID = iterItem->second;
	}
	else
	{
		return;
	}

	appendItem(parentID, action, -1);
	expandItem(parentID);
}

void ActionTreeResource::notifyActionDeleted(const std::string& name)
{
	wxTreeItemId itemID;
	auto iterItem = mItemList.find(name);
	if (iterItem != mItemList.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		return;
	}
	notifyItemDelete(itemID);
	mActionTreeView->Delete(itemID);
}

void ActionTreeResource::notifyActionTreeModified(const bool& modified)
{
	mActionTreeToolBar->EnableTool(ID_SAVE_ACTION_TREE, modified);
}

void ActionTreeResource::OnRefreshTreeResource(wxCommandEvent& event)
{
	refreshList();
}

void ActionTreeResource::OnNewActionTree(wxCommandEvent& event)
{
	mHandleListEvent = false;
	mActionTreeEditorCore->createActionTree();
	mHandleListEvent = true;
}

void ActionTreeResource::OnDeleteActionTree(wxCommandEvent& event)
{
	if (mActionTreeEditorCore->getCurActionTree() != NULL)
	{
		mActionTreeEditorCore->deleteActionTree(mActionTreeEditorCore->getCurActionTree()->getName());
	}
}

void ActionTreeResource::OnSaveActionTree(wxCommandEvent& event)
{
	mActionTreeEditorCore->saveActionTree();
}

void ActionTreeResource::OnTypeDoubleClick(wxListEvent& event)
{
	txActionTree* actionTree = mActionTreeEditorCore->getCurActionTree();
	if (actionTree == NULL)
	{
		return;
	}
	txAction* action = mActionTreeEditorCore->getCurAction();
	std::string parentName = action != NULL ? action->getName() : EMPTY_STRING;
	std::string itemName = mActionTypeList->GetItemText(event.GetItem());
	mActionTreeEditorCore->addAction(itemName, EMPTY_STRING, parentName);
}

void ActionTreeResource::OnActionTreeSelected(wxListEvent& event)
{
	if (!mHandleListEvent)
	{
		return;
	}
	mActionTreeEditorCore->selectActionTree(event.GetLabel().ToStdString());
}

void ActionTreeResource::OnRefreshTreeView(wxCommandEvent& event)
{
	notifyActionTreeActived();
}

void ActionTreeResource::OnDeleteAction(wxCommandEvent& event)
{
	txAction* curAction = mActionTreeEditorCore->getCurAction();
	if (curAction != NULL)
	{
		mActionTreeEditorCore->deleteAction(curAction->getName());
	}
}

void ActionTreeResource::OnLayoutText(wxCommandEvent& event)
{
	std::string layoutName = mActionTreeLayoutText->GetValue();
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		curTree->setUserData(mLayoutManager->getLayout(layoutName));
	}
}

void ActionTreeResource::OnActionTreePlay(wxCommandEvent& event)
{
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		curTree->play();
	}
}

void ActionTreeResource::OnActionTreePlayInverse(wxCommandEvent& event)
{
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		curTree->play(false);
	}
}

void ActionTreeResource::OnActionTreePause(wxCommandEvent& event)
{
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		curTree->pause();
	}
}

void ActionTreeResource::OnActionTreeStop(wxCommandEvent& event)
{
	txActionTree* curTree = mActionTreeEditorCore->getCurActionTree();
	if (curTree != NULL)
	{
		curTree->stop();
	}
}

void ActionTreeResource::OnActionSelected(wxTreeEvent& event)
{
	// 如果不接收wx的事件则直接返回
	if (!mHandleListEvent)
	{
		return;
	}
	// 此处不能再处理wx所触发的事件
	mHandleListEvent = false;

	txActionTree* actionTree = mActionTreeEditorCore->getCurActionTree();
	if (actionTree == NULL)
	{
		return;
	}
	txVector<txAction*> selectActionList;
	wxArrayTreeItemIds seletionIDList;
	int count = mActionTreeView->GetSelections(seletionIDList);
	for (int i = 0; i < count; ++i)
	{
		wxTreeItemId selectedItem = seletionIDList[i];
		// 不能选中布局
		if (selectedItem != mRootId)
		{
			std::string secletString = mActionTreeView->GetItemText(selectedItem).ToStdString();
			txAction* windFind = actionTree->getAction(secletString);
			if (windFind != NULL)
			{
				selectActionList.push_back(windFind);
				break;
			}
		}
	}
	if (selectActionList.size() > 0)
	{
		mActionTreeEditorCore->selectAction(selectActionList[0]->getName());
	}

	// 恢复wx事件处理
	mHandleListEvent = true;
}

int ActionTreeResource::getActionIconIndex(const std::string& actionType)
{
	auto iter = mTypeIconIndexList.find(actionType);
	if (iter != mTypeIconIndexList.end())
	{
		return iter->second;
	}
	return -1;
}

void ActionTreeResource::appendItem(const wxTreeItemId& parent, txAction* action, const int& pos)
{
	if (action == NULL)
	{
		return;
	}
	// 先把自己添加到节点上,然后再把自己的所有子窗口添加到自己的节点上
	int iconIndex = getActionIconIndex(action->getTypeName());
	wxTreeItemId actionId = NULL;
	if (pos >= 0 && pos < (int)mActionTreeView->GetChildrenCount(parent, false))
	{
		actionId = mActionTreeView->InsertItem(parent, pos, action->getName(), iconIndex);
	}
	else
	{
		actionId = mActionTreeView->AppendItem(parent, action->getName(), iconIndex);
	}
	mItemList.insert(action->getName(), actionId);
	txVector<txAction*>& childList = action->getChildList();
	int childCount = childList.size();
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		appendItem(actionId, childList[i]);
	}
	END_FOR_STL(childList);
}

void ActionTreeResource::expandItem(const wxTreeItemId& item)
{
	if (!item.IsOk())
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mActionTreeView->GetFirstChild(item, cookie);
	// 如果有子节点,就先展开所有的子节点
	while (childItem.IsOk())
	{
		expandItem(childItem);
		childItem = mActionTreeView->GetNextChild(item, cookie);
	}
	// 展开完所有子节点后,展开自己
	mActionTreeView->Expand(item);
}

void ActionTreeResource::notifyItemDelete(wxTreeItemId item)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mActionTreeView->GetFirstChild(item, cookie);
	// 如果有子节点,就先遍历所有的子节点
	while (childItem.IsOk())
	{
		notifyItemDelete(childItem);
		childItem = mActionTreeView->GetNextChild(item, cookie);
	}
	// 将自己从列表中删除
	std::string itemLabel = mActionTreeView->GetItemText(item).ToStdString();
	auto iterTree = mItemList.find(itemLabel);
	if (iterTree == mItemList.end())
	{
		EDITOR_CORE_ERROR("error : can not find item in tree item map! item label : %s", itemLabel.c_str());
		return;
	}
	mItemList.erase(iterTree);
}