#ifndef _SUPPORTED_TABLE_WINDOW_H_
#define _SUPPORTED_TABLE_WINDOW_H_

#include "DataDefine.h"
#include "EditorUtility.h"

class ConfigDataDialog;
class DataTemplate;
class SupportedTableWindow : public wxPanel
{
public:
	SupportedTableWindow(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	virtual ~SupportedTableWindow();
	void setup();
	void refresh(); // Ë¢ÐÂÁÐ±í
	void editTable(DataTemplate* dataTemplate);

	DECLARE_EVENT_TABLE()
	void OnShow(wxShowEvent& event);
	void OnRightUp(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);
	void OnNewData(wxCommandEvent& event);
	void OnRefresh(wxCommandEvent& event);
	void OnCreateTable(wxCommandEvent& event);
protected:
	void OnContextMenu();
protected:
	wxAuiToolBar* mToolBar;
	wxListBox* mResourceList;
	EditorFrame* mEditorFrame;
	ConfigDataDialog* mConfigDataDialog;
};

#endif