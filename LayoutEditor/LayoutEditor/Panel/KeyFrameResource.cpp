#include "Utility.h"
#include "txTremblingNode.h"
#include "txTrembling.h"
#include "txTremblingManager.h"
#include "txTremblingKeyFrame.h"

#include "TremblingEditorCore.h"

#include "KeyFrameResource.h"
#include "EditorUtility.h"

enum
{
	ID_KEY_FRAME_REOUCRCE_LIST,
	ID_DIMENSION_LIST,
	ID_KEY_FRAME_LIST,
	ID_REFRESH,
	ID_SAVE,
	ID_NEW_RESOURCE,
	ID_INFO_TEXT,
	ID_FORMAT_TEXT,
	ID_ADD_DIMENSION,
	ID_DELETE_DIMENSION,
	ID_NODE_INFO_TEXT,
	ID_INSERT_KEY_FRAME,
	ID_ADD_KEY_FRAME,
	ID_DELETE_KEY_FRAME,
	ID_TIME_TEXT,
	ID_VALUE_TEXT,
	ID_OPEN_KEY_FRAME_FILE,
	ID_OPEN_KEY_FRAME_DIR,
};

BEGIN_EVENT_TABLE(KeyFrameResource, EditorPanel)
EVT_LIST_ITEM_ACTIVATED(ID_KEY_FRAME_REOUCRCE_LIST, KeyFrameResource::OnResourceActivated)
EVT_LIST_ITEM_SELECTED(ID_DIMENSION_LIST, KeyFrameResource::OnDimensionSelected)
EVT_LIST_ITEM_SELECTED(ID_KEY_FRAME_LIST, KeyFrameResource::OnKeyFrameSelected)
EVT_CONTEXT_MENU(KeyFrameResource::OnResourceRightUp)
EVT_TOOL(ID_REFRESH, KeyFrameResource::OnRefreshResource)
EVT_TOOL(ID_SAVE, KeyFrameResource::OnSave)
EVT_TOOL(ID_NEW_RESOURCE, KeyFrameResource::OnNewResource)
EVT_TOOL(ID_ADD_DIMENSION, KeyFrameResource::OnAddDimension)
EVT_TOOL(ID_DELETE_DIMENSION, KeyFrameResource::OnDeleteDimension)
EVT_TEXT_ENTER(ID_INFO_TEXT, KeyFrameResource::OnTremblingInfo)
EVT_TEXT_ENTER(ID_FORMAT_TEXT, KeyFrameResource::OnTremblingFormat)
EVT_TOOL(ID_INSERT_KEY_FRAME, KeyFrameResource::OnInsertKeyFrame)
EVT_TOOL(ID_ADD_KEY_FRAME, KeyFrameResource::OnAddKeyFrame)
EVT_TOOL(ID_DELETE_KEY_FRAME, KeyFrameResource::OnDeleteKeyFrame)
EVT_TEXT_ENTER(ID_NODE_INFO_TEXT, KeyFrameResource::OnDimensionInfo)
EVT_TEXT_ENTER(ID_TIME_TEXT, KeyFrameResource::OnKeyFrameTime)
EVT_TEXT_ENTER(ID_VALUE_TEXT, KeyFrameResource::OnKeyFrameValue)
EVT_MENU(ID_OPEN_KEY_FRAME_FILE, KeyFrameResource::OnOpenFile)
EVT_MENU(ID_OPEN_KEY_FRAME_DIR, KeyFrameResource::OnOpenDir)
END_EVENT_TABLE()

KeyFrameResource::KeyFrameResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	EditorPanel(parent, position, size, style, name)
{
	setup();
}

void KeyFrameResource::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	mTremblingToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mTremblingToolBar->SetToolBitmapSize(wxSize(16, 16));
	mTremblingToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH), "刷新资源");
	mTremblingToolBar->AddTool(ID_SAVE, "保存关键帧", wxBITMAP(IDB_SAVE), "保存关键帧");
	mTremblingToolBar->AddTool(ID_NEW_RESOURCE, "新建关键帧资源", wxBITMAP(IDB_NEW), "新建关键帧资源");
	mTremblingToolBar->AddTool(ID_ADD_DIMENSION, "添加维度", wxBITMAP(IDB_ADD), "添加关键帧维度");
	mTremblingToolBar->AddTool(ID_DELETE_DIMENSION, "删除维度", wxBITMAP(IDB_REMOVE), "删除关键帧维度");
	mTremblingToolBar->Realize();

	bSizer4->Add(mTremblingToolBar, 0, wxALL, 5);
	bSizer1->Add(bSizer4, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxVERTICAL);

	mTremblingInfoText = new wxTextCtrl(this, ID_INFO_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	mTremblingFormatText = new wxTextCtrl(this, ID_FORMAT_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer2->Add(mTremblingInfoText, 0, wxALL | wxEXPAND, 5);
	bSizer2->Add(mTremblingFormatText, 0, wxALL | wxEXPAND, 5);

	mResourceList = new wxListCtrl(this, ID_KEY_FRAME_REOUCRCE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	mResourceList->InsertColumn(0, wxT(""), 0, 300);

	bSizer2->Add(mResourceList, 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer2, 3, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxVERTICAL);

	mDimensionToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mDimensionToolBar->SetToolBitmapSize(wxSize(16, 16));
	mDimensionToolBar->AddTool(ID_INSERT_KEY_FRAME, wxT("插入关键帧"), wxBITMAP(IDB_INSERT), wxT("插入关键帧"));
	mDimensionToolBar->AddTool(ID_ADD_KEY_FRAME, wxT("添加关键帧"), wxBITMAP(IDB_ADD), wxT("添加关键帧"));
	mDimensionToolBar->AddTool(ID_DELETE_KEY_FRAME, wxT("删除关键帧"), wxBITMAP(IDB_REMOVE), wxT("删除关键帧"));
	mDimensionToolBar->Realize();
	bSizer3->Add(mDimensionToolBar, 0, wxALL, 5);

	wxBoxSizer* bSizer7 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* nodeInfoLabel = new wxStaticText(this, wxID_ANY, wxT("维度描述:"));
	bSizer7->Add(nodeInfoLabel, 0, wxALL | wxEXPAND, 5);
	mNodeInfoText = new wxTextCtrl(this, ID_NODE_INFO_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer7->Add(mNodeInfoText, 1, wxALL | wxEXPAND, 5);
	bSizer3->Add(bSizer7, 0, wxALL | wxEXPAND, 5);

	mDimensionList = new wxListCtrl(this, ID_DIMENSION_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	mDimensionList->InsertColumn(0, wxT(""), 0, -1);
	bSizer3->Add(mDimensionList, 13, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, wxT("关键帧:"));
	bSizer6->Add(nameLabel, 0, wxALL | wxEXPAND, 5);
	mKeyFrameNameText = new wxTextCtrl(this, ID_TIME_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer6->Add(mKeyFrameNameText, 0, wxALL | wxEXPAND, 5);

	wxStaticText* timeLabel = new wxStaticText(this, wxID_ANY, wxT("时间:"));
	bSizer6->Add(timeLabel, 0, wxALL | wxEXPAND, 5);
	mKeyFrameTimeText = new wxTextCtrl(this, ID_TIME_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer6->Add(mKeyFrameTimeText, 0, wxALL | wxEXPAND, 5);

	wxStaticText* valueLabel = new wxStaticText(this, wxID_ANY, wxT("值:"));
	bSizer6->Add(valueLabel, 0, wxALL | wxEXPAND, 5);
	mKeyFrameValueText = new wxTextCtrl(this, ID_VALUE_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer6->Add(mKeyFrameValueText, 0, wxALL | wxEXPAND, 5);
	bSizer5->Add(bSizer6, 0, wxEXPAND, 5);

	mKeyFrameList = new wxListCtrl(this, ID_KEY_FRAME_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	mKeyFrameList->InsertColumn(0, wxT(""), 0, -1);
	bSizer5->Add(mKeyFrameList, 13, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void KeyFrameResource::refreshList()
{
	mResourceList->DeleteAllItems();
	auto& tremblingList = mTremblingManager->getTremblingList();
	auto iterTrembling = tremblingList.begin();
	auto iterTremblingEnd = tremblingList.end();
	FOR_STL(tremblingList, ; iterTrembling != iterTremblingEnd; ++iterTrembling)
	{
		int itemIndex = mResourceList->GetItemCount();
		mResourceList->InsertItem(itemIndex, iterTrembling->first.c_str());
	}
	END_FOR_STL(tremblingList);
	notifyTremblingActived();
}

void KeyFrameResource::refreshDimensionList()
{
	mDimensionList->DeleteAllItems();
	txTrembling* curTrembling = mTremblingEditorCore->getCurTrembling();
	if (curTrembling != NULL)
	{
		auto& nodeList = curTrembling->getTremblingNodeList();
		int nodeCount = nodeList.size();
		FOR_STL(nodeList, int i = 0; i < nodeCount; ++i)
		{
			mDimensionList->InsertItem(mDimensionList->GetItemCount(), nodeList[i]->getName());
		}
		END_FOR_STL(nodeList);
	}
}

void KeyFrameResource::notifyTremblingActived()
{
	txTrembling* trembling = mTremblingEditorCore->getCurTrembling();
	if (trembling != NULL)
	{
		mTremblingInfoText->SetValue(trembling->getInfo());
		mTremblingFormatText->SetValue(trembling->getFormat());
	}
	refreshDimensionList();
}

void KeyFrameResource::notifyDimensionSelected()
{
	mKeyFrameList->DeleteAllItems();
	txTremblingNode* curNode = mTremblingEditorCore->getCurTremblingNode();
	if (curNode != NULL)
	{
		mNodeInfoText->SetValue(curNode->getInfo());
		int keyFrameCount = curNode->getKeyFrameCount();
		for (int i = 0; i < keyFrameCount; ++i)
		{
			std::string keyFrameName = makeKeyFrameName(curNode->getKeyFrame(i));
			mKeyFrameList->InsertItem(mKeyFrameList->GetItemCount(), keyFrameName);
		}
	}
}

void KeyFrameResource::notifyKeyFrameSelected()
{
	txTremblingKeyFrame* keyFrame = mTremblingEditorCore->getCurKeyFrame();
	if (keyFrame != NULL)
	{
		std::string keyFrameName = makeKeyFrameName(keyFrame);
		long item = mKeyFrameList->FindItem(keyFrame->getIndexInParent(), keyFrameName);
		if (item != -1 && mKeyFrameList->GetItemState(item, wxLIST_STATE_SELECTED) != wxLIST_STATE_SELECTED)
		{
			mKeyFrameList->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
		mKeyFrameNameText->SetValue(keyFrameName);
		mKeyFrameTimeText->SetValue(txStringUtility::floatToString(keyFrame->getTime()));
		mKeyFrameValueText->SetValue(txStringUtility::floatToString(keyFrame->getOffset()));
	}
	else
	{
		int itemCount = mKeyFrameList->GetItemCount();
		for (int i = 0; i < itemCount; ++i)
		{
			if (mKeyFrameList->GetItemState(i, wxLIST_STATE_DONTCARE) != wxLIST_STATE_DONTCARE)
			{
				mKeyFrameList->SetItemState(i, wxLIST_STATE_DONTCARE, wxLIST_STATE_DONTCARE);
			}
		}
		mKeyFrameNameText->SetValue(EMPTY_STRING);
		mKeyFrameTimeText->SetValue("0");
		mKeyFrameValueText->SetValue("0");
	}
}

void KeyFrameResource::notifyNewTrembling(const std::string& name)
{
	mResourceList->InsertItem(mResourceList->GetItemCount(), name);
}

void KeyFrameResource::notifyAddDimension(const std::string& name)
{
	mDimensionList->InsertItem(mDimensionList->GetItemCount(), name);
}

void KeyFrameResource::notifyDeleteDimension(const std::string& name)
{
	int itemCount = mDimensionList->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		if (name == mDimensionList->GetItemText(i))
		{
			mDimensionList->DeleteItem(i);
			break;
		}
	}
}

void KeyFrameResource::notifyAddKeyFrame(const int& index)
{
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node != NULL)
	{
		mKeyFrameList->InsertItem(index, makeKeyFrameName(node->getKeyFrame(index)));
		refreshKeyFrameList();
	}
}

void KeyFrameResource::notifyDeleteKeyFrame(const int& index)
{
	mKeyFrameList->DeleteItem(index);
	refreshKeyFrameList();
}

void KeyFrameResource::notifyTremblingModified(bool modified)
{
	mTremblingToolBar->EnableTool(ID_SAVE, modified);
}

void KeyFrameResource::OnResourceActivated(wxListEvent& event)
{
	mTremblingEditorCore->loadKeyFrameTrembling(event.GetLabel().ToStdString());
}

void KeyFrameResource::OnResourceRightUp(wxContextMenuEvent& event)
{
	wxMenu menu;
	menu.Append(ID_OPEN_KEY_FRAME_FILE, wxT("打开文件"));
	menu.Append(ID_OPEN_KEY_FRAME_DIR, wxT("打开所在文件夹"));
	PopupMenu(&menu, ScreenToClient(event.GetPosition()));// 显示弹出菜单  
	event.Skip();
}

void KeyFrameResource::OnDimensionSelected(wxListEvent& event)
{
	mTremblingEditorCore->selectKeyFrameDimension(event.GetLabel().ToStdString());
}

void KeyFrameResource::OnKeyFrameSelected(wxListEvent& event)
{
	mTremblingEditorCore->selectKeyFrame(getKeyFrameIndexFromName(event.GetLabel().ToStdString()));
}

void KeyFrameResource::OnRefreshResource(wxCommandEvent& event)
{
	refreshList();
}

void KeyFrameResource::OnSave(wxCommandEvent& event)
{
	mTremblingEditorCore->saveKeyFrameTrebling();
}

void KeyFrameResource::OnNewResource(wxCommandEvent& event)
{
	mTremblingEditorCore->createKeyFrameTrembling();
}

void KeyFrameResource::OnTremblingInfo(wxCommandEvent& event)
{
	mTremblingEditorCore->setTremblingInfo(mTremblingInfoText->GetValue().ToStdString());
}

void KeyFrameResource::OnTremblingFormat(wxCommandEvent& event)
{
	mTremblingEditorCore->setTremblingFormat(mTremblingFormatText->GetValue().ToStdString());
}

void KeyFrameResource::OnAddDimension(wxCommandEvent& event)
{
	mTremblingEditorCore->addDimension();
}

void KeyFrameResource::OnDeleteDimension(wxCommandEvent& event)
{
	if (mTremblingEditorCore->getCurTremblingNode() != NULL)
	{
		mTremblingEditorCore->deleteDimension(mTremblingEditorCore->getCurTremblingNode()->getName());
	}
}

void KeyFrameResource::OnInsertKeyFrame(wxCommandEvent& event)
{
	if (mTremblingEditorCore->getCurTremblingNode() != NULL)
	{
		long item = mKeyFrameList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (item >= 0)
		{
			mTremblingEditorCore->addKeyFrame(item + 1);
		}
	}
}

void KeyFrameResource::OnAddKeyFrame(wxCommandEvent& event)
{
	if (mTremblingEditorCore->getCurTremblingNode() != NULL)
	{
		mTremblingEditorCore->addKeyFrame(mTremblingEditorCore->getCurTremblingNode()->getKeyFrameCount());
	}
}

void KeyFrameResource::OnDeleteKeyFrame(wxCommandEvent& event)
{
	if (mTremblingEditorCore->getCurKeyFrame() != NULL)
	{
		mTremblingEditorCore->deleteKeyFrame(mTremblingEditorCore->getCurKeyFrame()->getIndexInParent());
	}
}

void KeyFrameResource::OnDimensionInfo(wxCommandEvent& event)
{
	mTremblingEditorCore->setTremblingNodeInfo(mNodeInfoText->GetValue().ToStdString());
}

void KeyFrameResource::OnKeyFrameTime(wxCommandEvent& event)
{
	float time = txStringUtility::stringToFloat(mKeyFrameTimeText->GetValue().ToStdString());
	mTremblingEditorCore->setKeyFrameTime(time);
}

void KeyFrameResource::OnKeyFrameValue(wxCommandEvent& event)
{
	float offset = txStringUtility::stringToFloat(mKeyFrameValueText->GetValue().ToStdString());
	mTremblingEditorCore->setKeyFrameValue(offset);
}

void KeyFrameResource::OnOpenFile(wxCommandEvent& event)
{
	int selCount = mResourceList->GetSelectedItemCount();
	if (selCount > 0)
	{
		long item = mResourceList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		std::string filePath = P_TREMBLING_PATH + mResourceList->GetItemText(item).ToStdString() + TREMBLING_SUFFIX;
		EditorUtility::launchDefaultApplication(filePath);
	}
}

void KeyFrameResource::OnOpenDir(wxCommandEvent& event)
{
	EditorUtility::launchDefaultBrowser(P_TREMBLING_PATH);
}

void KeyFrameResource::refreshKeyFrameList()
{
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node != NULL)
	{
		int keyFrameCount = node->getKeyFrameCount();
		for (int i = 0; i < keyFrameCount; ++i)
		{
			std::string keyFrameName = makeKeyFrameName(node->getKeyFrame(i));
			if (mKeyFrameList->GetItemText(i) != keyFrameName)
			{
				mKeyFrameList->SetItemText(i, keyFrameName);
			}
		}
	}
}

std::string KeyFrameResource::makeKeyFrameName(txTremblingKeyFrame* keyFrame)
{
	return keyFrame->getParent()->getName() + "_" + txStringUtility::intToString(keyFrame->getIndexInParent());
}

int KeyFrameResource::getKeyFrameIndexFromName(const std::string& name)
{
	return txStringUtility::stringToInt(name.substr(name.find_last_of('_') + 1, name.length() - name.find_last_of('_') - 1));
}