#ifndef _DOWNLOAD_PANEL_H_
#define _DOWNLOAD_PANEL_H_

#include "EditorPanel.h"

class DownloadPanel : public EditorPanel
{
public:
	DownloadPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~DownloadPanel();
	void setup();
	void notifyFileList(const std::string& fileListString);
	DECLARE_EVENT_TABLE()
protected:
	wxListBox* mFileList;
};

#endif