#include "txScene.h"
#include "txEntity.h"
#include "txEngineRoot.h"
#include "txSceneManager.h"

#include "ECScene.h"
#include "EditorCoreRoot.h"
#include "ECEditorManager.h"
#include "ECEditor.h"
#include "GameLog.h"
#include "SceneEditorCore.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorCommandHeader.h"
#include "SceneHierarchy.h"

enum
{
	ID_TREE,
	ID_REFRESH,
};

BEGIN_EVENT_TABLE(SceneHierarchy, wxPanel)
EVT_TREE_SEL_CHANGED(ID_TREE, SceneHierarchy::OnItemSelected)
EVT_TREE_KEY_DOWN(ID_TREE, SceneHierarchy::OnItemKeyDown)
EVT_TREE_ITEM_RIGHT_CLICK(ID_TREE, SceneHierarchy::OnItemRClick)
EVT_TREE_BEGIN_LABEL_EDIT(ID_TREE, SceneHierarchy::OnItemBeginEdit)
EVT_TREE_END_LABEL_EDIT(ID_TREE, SceneHierarchy::OnItemEndEdit)
EVT_TOOL(ID_REFRESH, SceneHierarchy::OnRefresh)
END_EVENT_TABLE()

SceneHierarchy::SceneHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mHandleEvent(true)
{
	setup();
}

void SceneHierarchy::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT);
	mAuiToolBar->AddTool(ID_REFRESH, "刷新", wxBITMAP(IDB_REFRESH));
	mAuiToolBar->Realize();

	bSizer1->Add(mAuiToolBar, 0, wxALL, 5);

	mHierarchyTree = new wxTreeCtrl(this, ID_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxTR_MULTIPLE);
	bSizer1->Add(mHierarchyTree, 1, wxALL | wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
}

void SceneHierarchy::showScene(ECScene* scene)
{
	mHierarchyTree->DeleteAllItems();
	mTreeItemMap.clear();

	if (scene == NULL)
	{
		return;
	}

	wxTreeItemId rootID = mHierarchyTree->AddRoot(wxString(scene->getScene()->getName().c_str()));
	mTreeItemMap.insert(scene->getScene()->getName(), rootID);

	auto& editorList = scene->getEditorList();
	auto iterObject = editorList.begin();
	auto iterObjectEnd = editorList.end();
	FOR_STL(editorList, ; iterObject != iterObjectEnd; ++iterObject)
	{
		wxTreeItemId itemId = mHierarchyTree->AppendItem(rootID, iterObject->second->getName());
		mTreeItemMap.insert(iterObject->second->getName(), itemId);
	}
	END_FOR_STL(editorList);
	mHierarchyTree->ExpandAll();
}

void SceneHierarchy::setSelections(txVector<std::string>& itemLabel)
{
	setHandleEvent(false);
	mHierarchyTree->UnselectAll();
	int itemCount = itemLabel.size();
	FOR_STL(itemLabel, int i = 0; i < itemCount; ++i)
	{
		txMap<std::string, wxTreeItemId>::iterator iter = mTreeItemMap.find(itemLabel[i]);
		if (iter != mTreeItemMap.end())
		{
			mHierarchyTree->SelectItem(iter->second, true);
		}
	}
	END_FOR_STL(itemLabel);
	setHandleEvent(true);
}

void SceneHierarchy::OnRefresh(wxCommandEvent& event)
{
	showScene(mSceneEditorCore->getCurScene());
}

void SceneHierarchy::OnItemSelected(wxTreeEvent& event)
{
	// 如果不接收wx的事件则直接返回
	if (!mHandleEvent)
	{
		return;
	}

	txVector<ECEditor*> selections;
	wxArrayTreeItemIds seletionIDList;
	int count = mHierarchyTree->GetSelections(seletionIDList);
	for (int i = 0; i < count; ++i)
	{
		wxTreeItemId selectedItem = seletionIDList[i];
		std::string secletString = mHierarchyTree->GetItemText(selectedItem).ToStdString();	
		ECEditor* editor = mEditorManager->getEditor(secletString);
		if (editor != NULL)
		{
			selections.push_back(editor);
		}
	}
	mSceneEditorCore->setEditorSelection(selections);

	event.Skip();
}

void SceneHierarchy::OnItemKeyDown(wxTreeEvent& event)
{
	int keyCode = event.GetKeyEvent().GetRawKeyCode();
	if (VK_DELETE == keyCode)
	{
		// 将所有选中项文本放入一个列表中
		wxArrayTreeItemIds seletionIDList;
		int count = mHierarchyTree->GetSelections(seletionIDList);
		txVector<std::string> itemTextList;
		for (int i = 0; i < count; ++i)
		{
			std::string text = mHierarchyTree->GetItemText(seletionIDList[i]).ToStdString();
			itemTextList.push_back(text);
		}

		// 再根据列表中的名字删除编辑体
		int itemCount = itemTextList.size();
		FOR_STL(itemTextList, int i = 0; i < itemCount; ++i)
		{
			// 删除对应的编辑体
			if (itemTextList[i] != mSceneEditorCore->getCurScene()->getName())
			{
				CommandSceneDestroyEditor* cmd = NEW_CMD(cmd);
				cmd->mEditorName = itemTextList[i];
				mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
			}
		}
		END_FOR_STL(itemTextList);
	}
}

void SceneHierarchy::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	wxCHECK_RET(itemId, "item id valid error");
	wxPoint pointEvent = event.GetPoint();
	if (mHierarchyTree->GetRootItem().GetID() == itemId)
	{
		event.Skip();
		return;
	}
	wxMenu menu;
	PopupMenu(&menu, pointEvent);
	event.Skip();
}

void SceneHierarchy::renameItem(const std::string& label, const std::string& newLabel)
{
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(label);
	if (iterItem != mTreeItemMap.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		GAME_ERROR("error : can not find editor item in tree! editor name : %s", label.c_str());
		return;
	}
	mHierarchyTree->SetItemText(itemID, newLabel);
	// 删除旧的项,插入新的项
	mTreeItemMap.erase(iterItem);
	mTreeItemMap.insert(newLabel, itemID);
}

void SceneHierarchy::OnItemBeginEdit(wxTreeEvent& event)
{
	mPreString = mHierarchyTree->GetItemText(event.GetItem()).ToStdString();
}

void SceneHierarchy::OnItemEndEdit(wxTreeEvent& event)
{
	std::string newName = event.GetLabel().ToStdString();
	if (newName == EMPTY_STRING)
	{
		event.Veto();
	}
	else
	{
		ECScene* scene = mSceneEditorCore->getCurScene();
		ECEditor* editor = scene->getEditor(mPreString);
		// 判断选择的是layout还是window
		if (editor != NULL)
		{
			bool result = false;
			CommandEditorRename* cmdRename = NEW_CMD(cmdRename);
			cmdRename->mNewName = newName;
			cmdRename->setResult(&result);
			mCommandSystem->pushCommand(cmdRename, editor);
			if (!result)
			{
				event.Veto();
			}
		}
		else
		{
			bool result = false;
			CommandSceneRename* cmdRenameScene = NEW_CMD(cmdRenameScene);
			cmdRenameScene->mNewName = newName;
			cmdRenameScene->setResult(&result);
			mCommandSystem->pushCommand(cmdRenameScene, scene);
			if (!result)
			{
				event.Veto();
			}
		}
	}
}