#include "MemeryDetailWindow.h"
#include "EditorUtility.h"

enum
{
	ID_SEARCH_TEXT,
};

BEGIN_EVENT_TABLE(MemeryDetailWindow, wxPanel)

EVT_TEXT(ID_SEARCH_TEXT, onSearchTextChanged)
EVT_TEXT_ENTER(ID_SEARCH_TEXT, onSearchTextEntered)

END_EVENT_TABLE()

MemeryDetailWindow::MemeryDetailWindow(wxWindow* parent, long style)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{}

MemeryDetailWindow::~MemeryDetailWindow()
{
	;
}

void MemeryDetailWindow::init(MemeryViewer* viewer)
{
	mMemeryViewer = viewer;

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxVERTICAL);

	mSearchText = new wxTextCtrl(this, ID_SEARCH_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer8->Add(mSearchText, 1, wxALL | wxEXPAND, 5);

	mMemeryCountLabel = new wxStaticText(this, wxID_ANY, wxT("内存数量:0"), wxDefaultPosition, wxDefaultSize, 0);
	mMemeryCountLabel->Wrap(-1);
	bSizer8->Add(mMemeryCountLabel, 0, wxALL, 5);

	bSizer7->Add(bSizer8, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxVERTICAL);

	mMemeryInfoListCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLC_NO_SORT_HEADER | wxLC_REPORT);
	mMemeryInfoListCtrl->InsertColumn(0, "地址");
	mMemeryInfoListCtrl->SetColumnWidth(0, 100);
	mMemeryInfoListCtrl->InsertColumn(1, "类型");
	mMemeryInfoListCtrl->SetColumnWidth(1, 200);
	mMemeryInfoListCtrl->InsertColumn(2, "大小");
	mMemeryInfoListCtrl->SetColumnWidth(2, 100);
	mMemeryInfoListCtrl->InsertColumn(3, "文件");
	mMemeryInfoListCtrl->SetColumnWidth(3, 200);
	mMemeryInfoListCtrl->InsertColumn(4, "行号");
	mMemeryInfoListCtrl->SetColumnWidth(4, 60);

	bSizer9->Add(mMemeryInfoListCtrl, 1, wxALL | wxEXPAND, 5);

	bSizer7->Add(bSizer9, 1, wxEXPAND, 5);

	this->SetSizer(bSizer7);
	this->Layout();
}

void MemeryDetailWindow::update(float elapsedTime)
{
	if (mMemeryViewer == NULL)
	{
		return;
	}
	// 如果读取已经锁定则直接返回,不再等待
	if (mMemeryViewer->isLockRead())
	{
		return;
	}
	mMemeryViewer->lockWrite();

	std::map<void*, MemeryInfo>& memeryList = mMemeryViewer->getMemeryInfoList();
	int memeryCount = 0;
	if (mClassType != "")
	{
		mClassList.clear();
		std::map<void*, MemeryInfo>::iterator iterOld = memeryList.begin();
		std::map<void*, MemeryInfo>::iterator iterOldEnd = memeryList.end();
		for (; iterOld != iterOldEnd; ++iterOld)
		{
			if (mClassType == iterOld->second.type)
			{
				mClassList.insert(std::make_pair(iterOld->first, iterOld->second));
			}
		}
		// 如果最新的数量比原来的数量多,则扩充列表,如果少,则删除列表中多余的项
		if ((int)mClassList.size() > mMemeryInfoListCtrl->GetItemCount())
		{
			int addCount = mClassList.size() - mMemeryInfoListCtrl->GetItemCount();
			for (int i = 0; i < addCount; ++i)
			{
				mMemeryInfoListCtrl->InsertItem(mMemeryInfoListCtrl->GetItemCount(), "");
			}
		}
		else if ((int)mClassList.size() < mMemeryInfoListCtrl->GetItemCount())
		{
			int deleteCount = mMemeryInfoListCtrl->GetItemCount() - mClassList.size();
			for (int i = 0; i < deleteCount; ++i)
			{
				mMemeryInfoListCtrl->DeleteItem(mMemeryInfoListCtrl->GetItemCount() - 1);
			}
		}
		std::map<void*, MemeryInfo>::iterator iterClass = mClassList.begin();
		std::map<void*, MemeryInfo>::iterator iterClassEnd = mClassList.end();
		for (int i = 0; iterClass != iterClassEnd; ++iterClass, ++i)
		{
			char ptr[32];
			sprintf_s(ptr, 32, "0x%p", iterClass->first);
			if (mMemeryInfoListCtrl->GetItemText(i, 0) != wxString(ptr))
			{
				mMemeryInfoListCtrl->SetItem(i, 0, ptr);
				mMemeryInfoListCtrl->SetItem(i, 1, iterClass->second.type);
				mMemeryInfoListCtrl->SetItem(i, 2, EditorUtility::intToString(iterClass->second.size));
				mMemeryInfoListCtrl->SetItem(i, 3, iterClass->second.file);
				mMemeryInfoListCtrl->SetItem(i, 4, EditorUtility::intToString(iterClass->second.line));
			}
		}
	}

	std::string countLabel = std::string("内存数量:") + EditorUtility::intToString(mMemeryInfoListCtrl->GetItemCount());
	if (wxString(countLabel) != mMemeryCountLabel->GetLabel())
	{
		mMemeryCountLabel->SetLabel(countLabel);
	}
	
	mMemeryViewer->unlockWrite();
}

void MemeryDetailWindow::onSearchTextChanged(wxCommandEvent& event)
{
	mClassType = mSearchText->GetValue().ToStdString();
	mClassList.clear();
	event.Skip();
}

void MemeryDetailWindow::onSearchTextEntered(wxCommandEvent& event)
{
	int a = 0;
	event.Skip();
}