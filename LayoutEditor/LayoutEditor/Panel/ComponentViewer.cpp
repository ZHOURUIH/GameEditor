#include "txComponent.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "WindowFactory.h"
#include "WindowTemplateManager.h"
#include "LayoutManager.h"

#include "LayoutEditorCore.h"
#include "ComponentEditorCore.h"
#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "ComponentViewer.h"

enum
{
	ID_COMPONENT_LIST,
	ID_REFRESH,
};

BEGIN_EVENT_TABLE(ComponentViewer, EditorPanel)
EVT_LIST_ITEM_SELECTED(ID_COMPONENT_LIST, ComponentViewer::OnComponentItemSelected)
EVT_TOOL(ID_REFRESH, ComponentViewer::OnRefresh)
END_EVENT_TABLE()

ComponentViewer::ComponentViewer(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL)
{
	Setup();
}

void ComponentViewer::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新", wxBITMAP(IDB_REFRESH), "刷新");
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mComponentList = new wxListCtrl(this, ID_COMPONENT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	mComponentList->InsertColumn(1, wxT("组件"), 0, 300);
	bSizer1->Add(mComponentList, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void ComponentViewer::refreshList()
{
	mComponentList->DeleteAllItems();
	const txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	if (selections.size() > 0)
	{
		Window* window = selections[0];
		txVector<txComponent*>& componentList = window->getRootComponentList();
		int componentCount = componentList.size();
		FOR_STL(componentList, int i = 0; i < componentCount; ++i)
		{
			int index = mComponentList->InsertItem(mComponentList->GetItemCount(), componentList[i]->getName());
			mComponentList->SetItemTextColour(index, componentList[i]->isActive() ? wxColour(0, 255, 0) : wxColour(255, 0, 0));
		}
		END_FOR_STL(componentList);
	}
}

void ComponentViewer::notifyComponentActived(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	int itemIndex = mComponentList->FindItem(-1, component->getName());
	if (itemIndex >= 0)
	{
		mComponentList->SetItemTextColour(itemIndex, component->isActive() ? wxColour(0, 255, 0) : wxColour(255, 0, 0));
	}
}

void ComponentViewer::setSelection(txVector<std::string>& componentList)
{
	int componentCount = componentList.size();
	FOR_STL(componentList, int i = 0; i < componentCount; ++i)
	{
		int itemIndex = mComponentList->FindItem(-1, componentList[i]);
		mComponentList->SetItemState(itemIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	END_FOR_STL(componentList);
}

void ComponentViewer::unselectAll()
{
	int itemCount = mComponentList->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		mComponentList->SetItemState(i, wxLIST_STATE_DONTCARE, wxLIST_STATE_DONTCARE);
	}
}

void ComponentViewer::OnComponentItemSelected(wxListEvent& event)
{
	if (mLayoutEditorCore->getCurLayout() != NULL)
	{
		if (mLayoutEditorCore->getSelections().size() > 0)
		{
			Window* window = mLayoutEditorCore->getSelections()[0];
			mComponentEditorCore->selectComponent(window->getComponent(event.GetLabel().ToStdString()));
		}
	}
}

void ComponentViewer::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}