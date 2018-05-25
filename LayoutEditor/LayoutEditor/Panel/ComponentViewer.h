#ifndef _COMPONENT_VIEWER_H_
#define _COMPONENT_VIEWER_H_

#include "EditorPanel.h"

class ComponentViewer : public EditorPanel
{
public:
	ComponentViewer(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~ComponentViewer(){}
	void Setup();
	void refreshList(); // Ë¢ÐÂÁÐ±í
	void notifyComponentActived(txComponent* component);
	void setSelection(txVector<std::string>& componentList);
	void unselectAll();
	DECLARE_EVENT_TABLE()
	void OnComponentItemSelected(wxListEvent& event);
	void OnRefresh(wxCommandEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mComponentList;
};

#endif