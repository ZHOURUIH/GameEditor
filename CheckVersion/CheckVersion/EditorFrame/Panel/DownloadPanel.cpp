#include "DownloadPanel.h"
#include "File.h"
#include "EditorCore.h"
#include "DownloadManager.h"
#include "StringUtility.h"


BEGIN_EVENT_TABLE(DownloadPanel, EditorPanel)
END_EVENT_TABLE()

DownloadPanel::DownloadPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

DownloadPanel::~DownloadPanel()
{
	;
}

void DownloadPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	mFileList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	bSizer1->Add(mFileList, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void DownloadPanel::notifyFileList(const std::string& fileListString)
{
	txVector<std::string> fileList;
	StringUtility::split(fileListString, "%", fileList);
	mFileList->Clear();
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		mFileList->Append(fileList[i]);
	}
}