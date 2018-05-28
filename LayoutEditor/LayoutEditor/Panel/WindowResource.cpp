#include "txEngineRoot.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "WindowFactory.h"
#include "LayoutManager.h"
#include "WindowFactoryManager.h"

#include "ECLog.h"
#include "LayoutEditorCore.h"
#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "WindowResource.h"

enum
{
	ID_REFRESH,
	ID_WINDOW_RESOURCE_LISTCTRL,
};

BEGIN_EVENT_TABLE(WindowResource, EditorPanel)
EVT_TOOL(ID_REFRESH, WindowResource::OnRefresh)
EVT_LIST_ITEM_ACTIVATED(ID_WINDOW_RESOURCE_LISTCTRL, WindowResource::OnDoubleClick)
END_EVENT_TABLE()

WindowResource::WindowResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL)
{
	Setup();
}

WindowResource::~WindowResource()
{
	TRACE_DELETE(mImageList);
}

void WindowResource::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListCtrl(this, ID_WINDOW_RESOURCE_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_NO_HEADER);
	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);
	this->SetSizer(bSizer1);
	this->Layout();

	txMap<WINDOW_TYPE, wxBitmap> bitmapList;
	// 由于预设窗口类型不能直接创建,所以该类型没有图标,也不会显示,只能与其他类型共用
	bitmapList.insert(WT_PREFAB, wxBITMAP(IDB_BUTTON));
	bitmapList.insert(WT_BUTTON, wxBITMAP(IDB_BUTTON));
	bitmapList.insert(WT_CHECKBOX, wxBITMAP(IDB_CHECKBOX));
	bitmapList.insert(WT_EDITBOX, wxBITMAP(IDB_EDITBOX));
	bitmapList.insert(WT_NUMBER_WINDOW, wxBITMAP(IDB_NUMBERWINDOW));
	bitmapList.insert(WT_PROGRESS_WINDOW, wxBITMAP(IDB_PROGRESSWINDOW));
	bitmapList.insert(WT_TEXTURE_ANIM_WINDOW, wxBITMAP(IDB_TEXTUREANIMWINDOW));
	bitmapList.insert(WT_TEXTURE_WINDOW, wxBITMAP(IDB_TEXTUREWINDOW));
	bitmapList.insert(WT_TEXT_WINDOW, wxBITMAP(IDB_TEXTWINDOW));
	bitmapList.insert(WT_WINDOW, wxBITMAP(IDB_WINDOW));
	bitmapList.insert(WT_VIDEO_WINDOW, wxBITMAP(IDB_VIDEO_WINDOW));
	if (bitmapList.size() < WT_MAX)
	{
		EDITOR_CORE_ERROR("error : not all window type icon added! added count : %d, max count : %d", bitmapList.size(), WT_MAX);
	}
	mImageList = TRACE_NEW(wxImageList, mImageList, 16, 16);
	auto iterBitmap = bitmapList.begin();
	auto iterBitmapEnd = bitmapList.end();
	FOR_STL(bitmapList, ; iterBitmap != iterBitmapEnd; ++iterBitmap)
	{
		int imageIndex = mImageList->Add(iterBitmap->second);
		mWindowIconIndexList.insert(iterBitmap->first, imageIndex);
	}
	END_FOR_STL(bitmapList);
	mResourceList->SetImageList(mImageList, 0);
}

void WindowResource::refreshList()
{
	mResourceList->DeleteAllItems();
	auto& factoryList = mWindowFactoryManager->getFactoryList();
	auto iter = factoryList.begin();
	auto iterEnd = factoryList.end();
	FOR_STL(factoryList, ; iter != iterEnd; ++iter)
	{
		if (iter->second->isPublicType())
		{
			int imageIndex = -1;
			auto iterIndex = mWindowIconIndexList.find(iter->first);
			if (iterIndex != mWindowIconIndexList.end())
			{
				imageIndex = iterIndex->second;
			}
			const std::string& typeName = LayoutUtility::getTypeNameFromWindowType(iter->first);
			mResourceList->InsertItem(mResourceList->GetItemCount(), wxString(typeName.c_str()), imageIndex);
		}
	}
	END_FOR_STL(factoryList);
}

void WindowResource::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void WindowResource::OnDoubleClick(wxListEvent& event)
{
	auto& selections = mLayoutEditorCore->getSelections();
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout == NULL)
	{
		return;
	}

	std::string itemName = mResourceList->GetItemText(event.GetItem());
	if (selections.size() > 0)
	{
		mLayoutEditorCore->addWindow(selections[0], itemName);
	}
	else if (layout != NULL)
	{
		mLayoutEditorCore->addWindow(layout, itemName);
	}
}