#include "txEngineRoot.h"

#include "Layout.h"
#include "LayoutManager.h"

#include "LayoutEditorCore.h"
#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "LoadedLayout.h"

enum
{
	ID_REFRESH,
	ID_LOADED_LAYOUT_LISTCTRL,
};

BEGIN_EVENT_TABLE(LoadedLayout, EditorPanel)
EVT_TOOL(ID_REFRESH, LoadedLayout::OnRefresh)
EVT_LIST_ITEM_ACTIVATED(ID_LOADED_LAYOUT_LISTCTRL, LoadedLayout::OnItemActivated)
EVT_LIST_ITEM_RIGHT_CLICK(ID_LOADED_LAYOUT_LISTCTRL, LoadedLayout::OnItemRightClick)
EVT_LIST_KEY_DOWN(ID_LOADED_LAYOUT_LISTCTRL, LoadedLayout::OnItemKeyDown)
END_EVENT_TABLE()

LoadedLayout::LoadedLayout(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL),
mResourceList(NULL),
mImageList(NULL)
{
	Setup();
}

LoadedLayout::~LoadedLayout()
{
	TRACE_DELETE(mImageList);
}

void LoadedLayout::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListCtrl(this, ID_LOADED_LAYOUT_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_ICON);
	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);

	mImageList = new wxImageList(16, 16);
	mImageList->Add(wxBITMAP(IDB_LAYOUT_HIDE));
	mImageList->Add(wxBITMAP(IDB_LAYOUT_SHOW));
	mResourceList->SetImageList(mImageList, wxIMAGE_LIST_NORMAL);

	this->SetSizer(bSizer1);
	this->Layout();
}

void LoadedLayout::refreshList()
{
	mResourceList->ClearAll();
	auto& layoutList = mLayoutManager->getLayoutNameList();
	auto iterLayout = layoutList.begin();
	auto iterLayoutEnd = layoutList.end();
	FOR_STL(layoutList, ; iterLayout != iterLayoutEnd; ++iterLayout)
	{
		::Layout* layout = iterLayout->second;
		mResourceList->InsertItem(mResourceList->GetItemCount(), layout->getName(), layout->isVisible() ? 1 : 0);
	}
	END_FOR_STL(layoutList);
}

void LoadedLayout::setItemImage(const std::string& itemName, bool visible)
{
	wxString itemNameWX(itemName);
	int itemCount = mResourceList->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		if (mResourceList->GetItemText(i) == itemNameWX)
		{
			mResourceList->SetItemImage(i, visible ? 1 : 0);
			break;
		}
	}
}

void LoadedLayout::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void LoadedLayout::OnItemActivated(wxListEvent& event)
{
	// 双击打开文件,也就是将选中布局切换为当前正在编辑的布局
	std::string itemName = event.GetLabel().ToStdString();
	mEditorFrame->OpenFile(itemName);
}

void LoadedLayout::OnItemRightClick(wxListEvent& event)
{
	// 单击切换显示状态
	std::string itemName = event.GetLabel().ToStdString();
	::Layout* layout = mLayoutManager->getLayout(itemName);
	if (layout != NULL)
	{
		mLayoutEditorCore->setLayoutVisible(layout, !layout->isVisible());
	}
}

void LoadedLayout::OnItemKeyDown(wxListEvent& event)
{
	int keyCode = event.GetKeyCode();
	// 这里无法检测键值,只能检测字符
	if (keyCode == 127)
	{
		mEditorFrame->DestroyLayout(event.GetItem().GetText().ToStdString());
	}
}