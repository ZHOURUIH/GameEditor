#include "ConfigDataDialog.h"
#include "EditorUtility.h"
#include "EditorFrame.h"
#include "EditorApp.h"
#include "SupportedTableWindow.h"
#include "MainViewerWindow.h"
#include "DataDefine.h"
#include "DataBase.h"
#include "DataTemplate.h"
#include "txUtility.h"

enum
{
	ID_TYPE_CHOICE,
	ID_ARRAY_LENGTH_TEXT,
	ID_ADD_BUTTON,
	ID_MOVE_UP,
	ID_MOVE_DOWN,
	ID_MOVE_TOP,
	ID_MOVE_BOTTOM,
	ID_DELETE_ITEM,
	ID_DATA_VIEW_LIST,
	ID_CREATE_TABLE_BUTTON,
	ID_CLEAR_TABLE_BUTTON,
};

BEGIN_EVENT_TABLE(ConfigDataDialog, wxDialog)
EVT_CHOICE(ID_TYPE_CHOICE, OnTypeChoiceChanged)
EVT_TEXT(ID_ARRAY_LENGTH_TEXT, OnArrayLengthChanged)
EVT_BUTTON(ID_ADD_BUTTON, OnAddButton)
EVT_BUTTON(ID_MOVE_UP, OnMoveUp)
EVT_BUTTON(ID_MOVE_DOWN, OnMoveDown)
EVT_BUTTON(ID_MOVE_TOP, OnMoveTop)
EVT_BUTTON(ID_MOVE_BOTTOM, OnMoveBottom)
EVT_BUTTON(ID_DELETE_ITEM, OnDeleteItem)
EVT_LIST_ITEM_SELECTED(ID_DATA_VIEW_LIST, OnListItemSelected)
EVT_LIST_ITEM_ACTIVATED(ID_DATA_VIEW_LIST, OnListItemSelected)
EVT_LIST_KEY_DOWN(ID_DATA_VIEW_LIST, OnListItemKeyDown)
EVT_BUTTON(ID_CREATE_TABLE_BUTTON, OnCreateTableButton)
EVT_BUTTON(ID_CLEAR_TABLE_BUTTON, OnClearTableButton)
END_EVENT_TABLE()

ConfigDataDialog::ConfigDataDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
wxDialog(parent, wxID_ANY, title, position, size, style, name),
mCurSelectIndex(-1)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

ConfigDataDialog::~ConfigDataDialog()
{
	mDataViewList->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(ConfigDataDialog::OnRightUp), NULL, this);
	mDataViewList->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ConfigDataDialog::OnRightDown), NULL, this);
}

void ConfigDataDialog::setup()
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* describeStaticText = new wxStaticText(this, wxID_ANY, wxT("描述:"), wxDefaultPosition, wxDefaultSize, 0);
	mDescribeTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	wxStaticText* typeStaticText = new wxStaticText(this, wxID_ANY, wxT("数据类型:"), wxDefaultPosition, wxDefaultSize, 0);
	wxArrayString choices;
	std::map<std::string, int>::iterator iterType = Data::mTypeSize.begin();
	std::map<std::string, int>::iterator iterTypeEnd = Data::mTypeSize.end();
	for (; iterType != iterTypeEnd; ++iterType)
	{
		choices.Add(iterType->first);
	}
	mTypeChoice = new wxChoice(this, ID_TYPE_CHOICE, wxDefaultPosition, wxDefaultSize, choices, 0);
	mTypeChoice->SetSelection(0);
	wxStaticText* arrayLengthStaticText = new wxStaticText(this, wxID_ANY, wxT("数组长度:"), wxDefaultPosition, wxDefaultSize, 0);
	mArrayLengthTextCtrl = new wxTextCtrl(this, ID_ARRAY_LENGTH_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mArrayLengthTextCtrl->Enable(true);
	wxButton* addButton = new wxButton(this, ID_ADD_BUTTON, wxT("添加"), wxDefaultPosition, wxDefaultSize, 0);

	bSizer2->Add(describeStaticText, 0, wxALL, 5);
	bSizer2->Add(mDescribeTextCtrl, 0, wxALL, 5);
	bSizer2->Add(typeStaticText, 0, wxALL, 5);
	bSizer2->Add(mTypeChoice, 0, wxALL, 5);
	bSizer2->Add(arrayLengthStaticText, 0, wxALL, 5);
	bSizer2->Add(mArrayLengthTextCtrl, 0, wxALL, 5);
	bSizer2->Add(addButton, 0, wxALL, 5);

	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	wxButton* moveUp = new wxButton(this, ID_MOVE_UP, wxT("上移"), wxDefaultPosition, wxDefaultSize, 0);
	wxButton* moveDown = new wxButton(this, ID_MOVE_DOWN, wxT("下移"), wxDefaultPosition, wxDefaultSize, 0);
	wxButton* moveTop = new wxButton(this, ID_MOVE_TOP, wxT("移至顶部"), wxDefaultPosition, wxDefaultSize, 0);
	wxButton* moveBottom = new wxButton(this, ID_MOVE_BOTTOM, wxT("移至底部"), wxDefaultPosition, wxDefaultSize, 0);
	wxButton* deleteItemButton = new wxButton(this, ID_DELETE_ITEM, wxT("  删除  "), wxDefaultPosition, wxDefaultSize, 0);

	bSizer3->Add(moveUp, 0, wxALL, 5);
	bSizer3->Add(moveDown, 0, wxALL, 5);
	bSizer3->Add(moveTop, 0, wxALL, 5);
	bSizer3->Add(moveBottom, 0, wxALL, 5);
	bSizer3->Add(deleteItemButton, 0, wxALL, 5);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	mDataViewList = new wxListCtrl(this, ID_DATA_VIEW_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	mDataViewList->InsertColumn(0, "描述");
	mDataViewList->InsertColumn(1, "类型");
	mDataViewList->InsertColumn(2, "长度");
	mDataViewList->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ConfigDataDialog::OnRightDown), NULL, this);
	mDataViewList->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(ConfigDataDialog::OnRightUp), NULL, this);
	bSizer4->Add(mDataViewList, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer4, 5, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* tableNameStaticText = new wxStaticText(this, wxID_ANY, wxT("表格名称:"), wxDefaultPosition, wxDefaultSize, 0);
	mTableNameTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mCreateTableButton = new wxButton(this, ID_CREATE_TABLE_BUTTON, wxT("创建表格"), wxDefaultPosition, wxDefaultSize, 0);
	mClearButton = new wxButton(this, ID_CLEAR_TABLE_BUTTON, wxT("清空表格"), wxDefaultPosition, wxDefaultSize, 0);

	bSizer5->Add(tableNameStaticText, 0, wxALL, 5);
	bSizer5->Add(mTableNameTextCtrl, 0, wxALL, 5);
	bSizer5->Add(mCreateTableButton, 0, wxALL, 5);
	bSizer5->Add(mClearButton, 0, wxALL, 5);

	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);
}

void ConfigDataDialog::OnTypeChoiceChanged(wxCommandEvent& event)
{
	wxString str = mTypeChoice->GetString(mTypeChoice->GetSelection());
	mArrayLengthTextCtrl->Enable(EditorUtility::endWith(str.ToStdString(), "*"));
	mArrayLengthTextCtrl->Clear();
}

void ConfigDataDialog::OnArrayLengthChanged(wxCommandEvent& event)
{
	wxString str = mArrayLengthTextCtrl->GetValue();
	int len = str.length();
	for (int i = 0; i < len; ++i)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			EditorFrame::errorMessage("只能输入数字!");
			mArrayLengthTextCtrl->SetValue("");
			break;
		}
	}
}

void ConfigDataDialog::OnAddButton(wxCommandEvent& event)
{
	std::string paramType = mTypeChoice->GetString(mTypeChoice->GetSelection()).ToStdString();
	int arrayLen = 0;
	wxString arrayLenStr = mArrayLengthTextCtrl->GetValue();
	if (arrayLenStr != "")
	{
		arrayLen = txUtility::stringToInt(arrayLenStr.ToStdString());
	}
	addItem(paramType, mDescribeTextCtrl->GetValue().ToStdString(), arrayLen, mCurSelectIndex + 1);
}

void ConfigDataDialog::OnMoveUp(wxCommandEvent& event)
{
	moveItem(mCurSelectIndex - 1);
}

void ConfigDataDialog::OnMoveDown(wxCommandEvent& event)
{
	moveItem(mCurSelectIndex + 1);
}

void ConfigDataDialog::OnMoveTop(wxCommandEvent& event)
{
	moveItem(0);
}

void ConfigDataDialog::OnMoveBottom(wxCommandEvent& event)
{
	moveItem(mDataParamList.size() - 1);
}

void ConfigDataDialog::OnDeleteItem(wxCommandEvent& event)
{
	deleteItem(mCurSelectIndex);
}

void ConfigDataDialog::OnListItemSelected(wxListEvent& event)
{
	mCurSelectIndex = event.GetIndex();
}

void ConfigDataDialog::OnRightUp(wxMouseEvent& event)
{
	//wxPoint clientpt = event.GetPosition();
	//wxMenu menu;
	//PopupMenu(&menu, clientpt);
	event.Skip();
}

void ConfigDataDialog::OnRightDown(wxMouseEvent& event)
{
	event.Skip();
}

void ConfigDataDialog::OnListItemKeyDown(wxListEvent& event)
{
	int keyCode = event.GetKeyCode();
	// 这里无法检测键值,只能检测字符
	if (keyCode == 127)
	{
		deleteItem(mCurSelectIndex);
	}
}

void ConfigDataDialog::OnCreateTableButton(wxCommandEvent& event)
{
	std::string tableName = mTableNameTextCtrl->GetValue();
	if (tableName == "")
	{
		EditorFrame::errorMessage("表格名不能为空!");
		return;
	}
	DataBase* dataBase = mEditorFrame->getDataBase();
	if (mDataParamList.size() == 0)
	{
		EditorFrame::errorMessage("不能创建没有参数的表格!");
		return;
	}
	DataTemplate* dataTemplate = dataBase->createDataTemplate(tableName, mDataParamList);
	if (dataTemplate != NULL)
	{
		dataBase->writeTemplateConfig(tableName);
	}
	else
	{
		EditorFrame::errorMessage("创建表格失败! 表格名已存在");
		return;
	}
	// 创建完表格后刷新支持的表格列表
	SupportedTableWindow* supportedTableWindow = mEditorFrame->getSupportedTableWindow();
	supportedTableWindow->refresh();
}

void ConfigDataDialog::OnClearTableButton(wxCommandEvent& event)
{
	mDataParamList.clear();
	refresh();
}

void ConfigDataDialog::refresh()
{
	mDataViewList->DeleteAllItems();
	int paramCount = mDataParamList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		mDataViewList->InsertItem(i, "");
		mDataViewList->SetItem(i, 0, mDataParamList[i][0]);
		mDataViewList->SetItem(i, 1, mDataParamList[i][1]);
		mDataViewList->SetItem(i, 2, mDataParamList[i][2]);
	}
	if (mCurSelectIndex >= 0)
	{
		mDataViewList->SetItemState(mCurSelectIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
}

void ConfigDataDialog::editTable(DataTemplate* dataTemplate)
{
	mDataParamList.clear();
	mDataTemplate = dataTemplate;
	// 描述,类型,长度
	if (mDataTemplate != NULL)
	{
		const std::vector<DataParameter>& paramList = dataTemplate->getParameterList();
		int listSize = paramList.size();
		for (int i = 0; i < listSize; ++i)
		{
			std::vector<std::string> params;
			params.push_back(paramList[i].mDescribe);
			params.push_back(paramList[i].mDataType);
			if (txSerializedData::isArrayType(paramList[i].mDataType))
			{
				params.push_back(txUtility::intToString(paramList[i].mDataSize / Data::getTypeSize(paramList[i].mDataType)));
			}
			else
			{
				params.push_back("0");
			}
			mDataParamList.push_back(params);
		}
		mCreateTableButton->Enable(false);
		mClearButton->Enable(false);
		mTableNameTextCtrl->Enable(false);
		mTableNameTextCtrl->SetValue(mDataTemplate->getType());
		mCurSelectIndex = 0;
	}
	else
	{
		mCreateTableButton->Enable(true);
		mClearButton->Enable(true);
		mTableNameTextCtrl->Enable(true);
		mCurSelectIndex = -1;
	}
	refresh();
}

void ConfigDataDialog::deleteItem(int index)
{
	if (index >= 0 && index <= (int)mDataParamList.size() - 1)
	{
		std::vector<std::string> params = mDataParamList[index];
		mDataParamList.erase(mDataParamList.begin() + index);
		if (mDataTemplate != NULL)
		{
			mEditorFrame->getDataBase()->deleteTemplateParam(mDataTemplate->getType(), index);
			mEditorFrame->setDataModified(true);
			if (mCurSelectIndex >= (int)mDataParamList.size())
			{
				mCurSelectIndex = (int)mDataParamList.size() - 1;
			}
		}
		refresh();
		mEditorFrame->getMainViewerWindow()->showData(mDataTemplate->getType());
	}
}

void ConfigDataDialog::moveItem(int destPos)
{
	if (mCurSelectIndex >= 0 && destPos >= 0 && destPos < (int)mDataParamList.size())
	{
		std::vector<std::string> params = mDataParamList[mCurSelectIndex];
		mDataParamList.erase(mDataParamList.begin() + mCurSelectIndex);
		mDataParamList.insert(mDataParamList.begin() + destPos, params);
		if (mDataTemplate != NULL)
		{
			mEditorFrame->getDataBase()->moveTemplateParam(mDataTemplate->getType(), mCurSelectIndex, destPos);
			mEditorFrame->setDataModified(true);
		}
		mCurSelectIndex = destPos;
		refresh();
		mEditorFrame->getMainViewerWindow()->showData(mDataTemplate->getType());
	}
}

void ConfigDataDialog::addItem(const std::string& paramType, const std::string& describe, int count, int paramPos)
{
	// 如果是数组类型的,则长度不能为0
	if (txSerializedData::isArrayType(paramType) && count == 0)
	{
		EditorFrame::errorMessage("数组类型的长度不能为0");
		return;
	}
	if (paramPos >= 0 && paramPos <= (int)mDataParamList.size())
	{
		std::vector<std::string> params;
		params.push_back(describe);
		params.push_back(paramType);
		params.push_back(txUtility::intToString(count));
		mDataParamList.insert(mDataParamList.begin() + paramPos, params);
		if (mDataTemplate != NULL)
		{
			if (txSerializedData::isArrayType(paramType))
			{
				mEditorFrame->getDataBase()->addTemplateArrayParam(mDataTemplate->getType(), paramType, count, describe, paramPos);
			}
			else
			{
				mEditorFrame->getDataBase()->addTemplateParam(mDataTemplate->getType(), paramType, describe, paramPos);
			}
			mEditorFrame->getMainViewerWindow()->showData(mDataTemplate->getType());
			mEditorFrame->setDataModified(true);
		}
		mCurSelectIndex = paramPos;
		refresh();
	}
}