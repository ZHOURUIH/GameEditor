#ifndef _EDITOR_BUSY_INFO_FRAME_H_
#define _EDITOR_BUSY_INFO_FRAME_H_

#include "EditorUtility.h"

class EditorInfoFrame : public wxFrame
{
public:
	EditorInfoFrame(wxWindow* parent, const wxString& message);
	void setText(const wxString& text);
protected:
	wxStaticText* mText;
};

#endif