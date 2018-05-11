#include "MemeryTypeStatisticsWindow.h"
#include "EditorUtility.h"
#include "KeywordDialog.h"

enum
{
	ID_FILTER_TEXT,
	ID_SEARCH_HIDE_CHECK,
	ID_OPEN_LIST_BUTTON,
};

BEGIN_EVENT_TABLE(MemeryTypeStatisticsWindow, wxPanel)

EVT_TEXT(ID_FILTER_TEXT, OnFilterTextChanged)
EVT_CHECKBOX(ID_SEARCH_HIDE_CHECK, OnSearchHideItem)
EVT_BUTTON(ID_OPEN_LIST_BUTTON, OnOpenKeywordList)

END_EVENT_TABLE()

MemeryTypeStatisticsWindow::MemeryTypeStatisticsWindow(wxWindow* parent, long style, wxSize size)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, size, style)
{
	mKeywordListDialog = new KeywordDialog("内存类型关键字");
}

MemeryTypeStatisticsWindow::~MemeryTypeStatisticsWindow()
{
	delete mKeywordListDialog;
}

void MemeryTypeStatisticsWindow::init(MemeryViewer* viewer)
{
	mMemeryViewer = viewer;

	mKeywordListDialog->init();
	mKeywordListDialog->Hide();

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mFilterText = new wxTextCtrl(this, ID_FILTER_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	mFilterText->SetMaxLength(100);
	bSizer2->Add(mFilterText, 1, wxALL | wxEXPAND, 5);

	mSearchHideItem = new wxCheckBox(this, ID_SEARCH_HIDE_CHECK, wxT("搜索隐藏项"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mSearchHideItem, 0, wxALL, 5);

	mOpenIgnoreKeywordList = new wxButton(this, ID_OPEN_LIST_BUTTON, wxT("过滤列表"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mOpenIgnoreKeywordList, 0, wxALL, 5);

	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	mMemeryCountLabel = new wxStaticText(this, wxID_ANY, wxT("内存总数量:0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemeryCountLabel, 0, wxALL, 5);
	
	mMemerySizeLabel = new wxStaticText(this, wxID_ANY, wxT("内存总大小:0KB"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemerySizeLabel, 0, wxALL, 5);

	mMemeryTypeCountLabel = new wxStaticText(this, wxID_ANY, wxT("内存类型个数:0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemeryTypeCountLabel, 0, wxALL, 5);

	bSizer1->Add(bSizer3, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	mTypeList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLC_NO_SORT_HEADER | wxLC_REPORT);
	mTypeList->InsertColumn(0, "类型名");
	mTypeList->SetColumnWidth(0, 450);
	mTypeList->InsertColumn(1, "数量");
	mTypeList->SetColumnWidth(1, 80);
	mTypeList->InsertColumn(2, "大小");
	mTypeList->SetColumnWidth(2, 80);
	bSizer4->Add(mTypeList, 1, wxALL | wxEXPAND, 5);

	mSearchList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLC_NO_SORT_HEADER | wxLC_REPORT);
	mSearchList->InsertColumn(0, "类型名");
	mSearchList->SetColumnWidth(0, 450);
	mSearchList->InsertColumn(1, "数量");
	mSearchList->SetColumnWidth(1, 80);
	mSearchList->InsertColumn(2, "大小");
	mSearchList->SetColumnWidth(2, 80);
	mSearchList->Hide();
	bSizer4->Add(mSearchList, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void MemeryTypeStatisticsWindow::update(float elapsedTime)
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
	int listMaxCount = mTypeList->GetItemCount();
	int maxIndex = mMemeryViewer->getMaxIndex();
	// 如果列表已经显示不下了,则扩充列表
	if (listMaxCount < maxIndex)
	{
		setMaxCount(maxIndex);
	}
	mIgnoreMemeryType.clear();
	int totalCount = 0;
	int totalSize = 0;
	int typeCount = 0;
	MemeryType* memeryList = mMemeryViewer->getMemeryList();
	std::set<std::string>& keywordList = mKeywordListDialog->getKeywordList();
	for (int i = 0; i < maxIndex; ++i)
	{
		if (memeryList[i].size > 0)
		{
			totalSize += memeryList[i].size;
		}
		if (memeryList[i].count > 0)
		{
			totalCount += memeryList[i].count;
			++typeCount;
		}
		// 数量大于0,并且不是被忽略的类型,则显示在列表指定位置
		if (memeryList[i].count > 0 && !EditorUtility::isIgnoreClass(keywordList, memeryList[i].type))
		{
			if (mTypeList->GetItemText(i, 0) != wxString(memeryList[i].type.c_str()))
			{
				mTypeList->SetItem(i, 0, memeryList[i].type.c_str());
			}
			// 如果数量有改变,那么大小也一定会有改变
			char buf[16];
			sprintf_s(buf, 16, "%d", memeryList[i].count);
			if (mTypeList->GetItemText(i, 1) != wxString(buf))
			{
				mTypeList->SetItem(i, 1, buf);
				sprintf_s(buf, 16, "%d", memeryList[i].size);
				mTypeList->SetItem(i, 2, buf);
			}
		}
		else
		{
			// 如果是被忽略的类型,则放入忽略类型列表中
			if (EditorUtility::isIgnoreClass(keywordList, memeryList[i].type))
			{
				mIgnoreMemeryType.push_back(memeryList[i]);
				if (mTypeList->GetItemText(i, 0) != wxString("~~"))
				{
					mTypeList->SetItem(i, 0, "~~");
					mTypeList->SetItem(i, 1, "");
					mTypeList->SetItem(i, 2, "");
				}
			}
			// 如果数量为0,则使用-表示
			else
			{
				if (mTypeList->GetItemText(i, 0) != wxString("-"))
				{
					mTypeList->SetItem(i, 0, "-");
					mTypeList->SetItem(i, 1, "-");
					mTypeList->SetItem(i, 2, "-");
				}
			}
		}
	}
	mMemeryViewer->unlockWrite();

	std::string totalCountString = std::string("内存总数量:") + EditorUtility::intToString(totalCount);
	if (wxString(totalCountString) != mMemeryCountLabel->GetLabel())
	{
		mMemeryCountLabel->SetLabel(totalCountString);
	}

	std::string totalSizeString = std::string("内存总大小:") + EditorUtility::floatToString(totalSize / 1000.0f) + " KB";
	if (wxString(totalSizeString) != mMemerySizeLabel->GetLabel())
	{
		mMemerySizeLabel->SetLabel(totalSizeString);
	}

	std::string typeCountString = std::string("内存类型个数:") + EditorUtility::intToString(typeCount);
	if (wxString(typeCountString) != mMemeryTypeCountLabel->GetLabel())
	{
		mMemeryTypeCountLabel->SetLabel(typeCountString);
	}
}

void MemeryTypeStatisticsWindow::destroy()
{
	mTypeList->DeleteAllItems();
	mTypeList->DeleteAllColumns();
}

void MemeryTypeStatisticsWindow::setMaxCount(int count)
{
	if (count < 0 || count > 1000)
	{
		return;
	}
	int itemCount = mTypeList->GetItemCount();
	// 如果当前显示的数量已经大于指定的显示数量,则删除多余的项
	if (itemCount > count)
	{
		// 从列表尾部开始删除
		for (int i = 0; i < itemCount - count; ++i)
		{
			mTypeList->DeleteItem(itemCount - i - 1);
		}
	}
	// 如果当前显示的数量小于指定的显示数量,则在列表末尾添加空项
	else if (itemCount < count)
	{
		for (int i = 0; i < count - itemCount; ++i)
		{
			mTypeList->InsertItem(mTypeList->GetItemCount(), "");
		}
	}
}

void MemeryTypeStatisticsWindow::OnFilterTextChanged(wxCommandEvent& event)
{
	// 如果搜索框不为空,则隐藏类型列表,显示搜索结果列表
	std::string filterText = mFilterText->GetValue().ToStdString();
	if (filterText != wxString(""))
	{
		mTypeList->Hide();
		mSearchList->Show();
		mSearchList->DeleteAllItems();
		int typeItemCount = mTypeList->GetItemCount();
		for (int i = 0; i < typeItemCount; ++i)
		{
			wxString itemText = mTypeList->GetItemText(i, 0);
			if (strstr(itemText.ToStdString().c_str(), filterText.c_str()) != NULL)
			{
				int itemIndex = mSearchList->InsertItem(mSearchList->GetItemCount(), "");
				mSearchList->SetItem(itemIndex, 0, itemText);
				mSearchList->SetItem(itemIndex, 1, mTypeList->GetItemText(i, 1));
				mSearchList->SetItem(itemIndex, 2, mTypeList->GetItemText(i, 2));
			}
		}
		// 如果还要搜索隐藏项,则在忽略列表中继续查找
		if (mSearchHideItem->GetValue())
		{
			int listSize = mIgnoreMemeryType.size();
			for (int i = 0; i < listSize; ++i)
			{
				if (strstr(mIgnoreMemeryType[i].type.c_str(), filterText.c_str()) != NULL)
				{
					int itemIndex = mSearchList->InsertItem(mSearchList->GetItemCount(), "");
					mSearchList->SetItem(itemIndex, 0, mIgnoreMemeryType[i].type);
					mSearchList->SetItem(itemIndex, 1, EditorUtility::intToString(mIgnoreMemeryType[i].count));
					mSearchList->SetItem(itemIndex, 2, EditorUtility::intToString(mIgnoreMemeryType[i].size));
				}
			}
		}
	}
	// 如果搜索框为空,则显示类型列表,隐藏搜索结果列表
	else
	{
		mSearchList->Hide();
		mTypeList->Show();
		mSearchList->DeleteAllItems();
	}
	// 刷新布局
	Layout();
	event.Skip();
}

void MemeryTypeStatisticsWindow::OnSearchHideItem(wxCommandEvent& event)
{
	event.Skip();
}

void MemeryTypeStatisticsWindow::OnOpenKeywordList(wxCommandEvent& event)
{
	mKeywordListDialog->Show(!mKeywordListDialog->IsShown());
	if (mKeywordListDialog->IsShown())
	{
		mKeywordListDialog->SetPosition(GetScreenPosition());
	}
	event.Skip();
}