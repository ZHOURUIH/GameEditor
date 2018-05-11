#ifndef _EDITOR_BUSY_INFO_H_
#define _EDITOR_BUSY_INFO_H_

#include "EditorUtility.h"

class EditorInfoFrame;
class EditorBusyInfo : public wxFrame
{
public:
	EditorBusyInfo();
	EditorBusyInfo(const wxString& message, wxWindow *parent);
	~EditorBusyInfo();
	void setBusyInfo(const char* info);
	void show();
	void hide();
protected:
	EditorInfoFrame* mInfoFrame;
};

#endif