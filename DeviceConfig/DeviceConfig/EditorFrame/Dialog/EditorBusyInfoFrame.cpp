#include "EditorBusyInfoFrame.h"

EditorInfoFrame::EditorInfoFrame(wxWindow *parent, const wxString& message)
: 
wxFrame(parent, wxID_ANY, wxT("Busy"), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP)
{
	wxPanel *panel = new wxPanel(this);

	mText = new wxStaticText(panel, wxID_ANY, message);
	mText->Wrap(-1);
	panel->SetCursor(*wxHOURGLASS_CURSOR);
	mText->SetCursor(*wxHOURGLASS_CURSOR);
	wxSize sizeText = mText->GetBestSize();
	SetClientSize(400, 100);
	//SetClientSize(wxMax(sizeText.x, 340) + 60, wxMax(sizeText.y, 40) + 40);
	panel->SetSize(GetClientSize());

	mText->Centre(wxBOTH);
	Centre(wxBOTH);
}

void EditorInfoFrame::setText(wxString text)
{
	mText->SetLabelText(text);
	mText->Centre(wxBOTH);
}