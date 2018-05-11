#include "EditorBusyInfo.h"
#include "EditorBusyInfoFrame.h"

EditorBusyInfo::EditorBusyInfo()
{
	mInfoFrame = new EditorInfoFrame(NULL, "");
	mInfoFrame->Show(true);
	mInfoFrame->Refresh();
	mInfoFrame->Update();
}

EditorBusyInfo::EditorBusyInfo(const wxString& message, wxWindow *parent)
{
	mInfoFrame = new EditorInfoFrame(parent, message);
	mInfoFrame->Show(true);
	mInfoFrame->Refresh();
	mInfoFrame->Update();
}

EditorBusyInfo::~EditorBusyInfo()
{
	mInfoFrame->Show(false);
	mInfoFrame->Close();
}

void EditorBusyInfo::setBusyInfo(const char* info)
{
	mInfoFrame->setText(wxString(info));
}

void EditorBusyInfo::show()
{
	mInfoFrame->Show(true);
}

void EditorBusyInfo::hide()
{
	mInfoFrame->Show(false);
}