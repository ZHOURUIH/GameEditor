#ifndef _MAKE_LIST_PANEL_H_
#define _MAKE_LIST_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class MakeListPanel : public EditorPanel
{
public:
	MakeListPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~MakeListPanel();
	void setup();
	void refreshInfo();
	DECLARE_EVENT_TABLE()
	void OnGenerate(wxCommandEvent& event);
protected:
	void removeIgnorePath(txVector<std::string>& pathList, const txVector<std::string>& ignoreList);
protected:
	wxButton* mGenerateButton;
	wxTextCtrl* mFolderNameText;
	wxTextCtrl* mExceptNameText;
	wxTextCtrl* mSourcePathList;
	wxTextCtrl* mSourceFileList;
};

#endif