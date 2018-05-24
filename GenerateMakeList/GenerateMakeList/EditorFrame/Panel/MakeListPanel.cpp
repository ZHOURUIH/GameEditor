#include "MakeListPanel.h"
#include "EditorCore.h"
#include "Utility.h"

enum
{
	ID_GENERATE,
};

BEGIN_EVENT_TABLE(MakeListPanel, EditorPanel)
EVT_BUTTON(ID_GENERATE, OnGenerate)
END_EVENT_TABLE()

MakeListPanel::MakeListPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

MakeListPanel::~MakeListPanel()
{
	;
}

void MakeListPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	mGenerateButton = new wxButton(this, ID_GENERATE, wxT("开始计算"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mGenerateButton, 0, wxALL, 5);
	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("路径:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText1, 0, wxALL, 5);
	mFolderNameText = new wxTextCtrl(this, wxID_ANY, "Server", wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mFolderNameText, 0, wxALL, 5);
	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("排除的路径:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText2, 0, wxALL, 5);
	mExceptNameText = new wxTextCtrl(this, wxID_ANY, "Debug;Release", wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mExceptNameText, 0, wxALL, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxVERTICAL);
	mSourcePathList = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	bSizer3->Add(mSourcePathList, 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	mSourceFileList = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	bSizer4->Add(mSourceFileList, 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void MakeListPanel::OnGenerate(wxCommandEvent& event)
{
	std::string path = mFolderNameText->GetValue();
	txVector<std::string> exceptList;
	StringUtility::split(std::string(mExceptNameText->GetValue()), ";", exceptList);
	int exceptCount = exceptList.size();
	FOR_STL(exceptList, int i = 0; i < exceptCount; ++i)
	{
		exceptList[i] = path + "/" + exceptList[i];
	}
	END_FOR_STL(exceptList);
	
	std::string filePathString;
	txVector<std::string> pathList;
	FileUtility::findFolers(path, pathList, true);
	removeIgnorePath(pathList, exceptList);
	int pathCount = pathList.size();
	FOR_STL(pathList, int i = 0; i < pathCount; ++i)
	{
		filePathString += pathList[i] + ";\r\n";
	}
	END_FOR_STL(pathList);
	mSourcePathList->SetValue(filePathString);

	std::string fileListString;
	txVector<std::string> fileList;
	FileUtility::findFiles(path, fileList, ".cpp");
	removeIgnorePath(fileList, exceptList);
	int count = fileList.size();
	FOR_STL(fileList, int i = 0; i < count; ++i)
	{
		fileListString += fileList[i] + ";\r\n";
	}
	END_FOR_STL(fileList);
	mSourceFileList->SetValue(fileListString);
}

void MakeListPanel::removeIgnorePath(txVector<std::string>& pathList, const txVector<std::string>& ignoreList)
{
	int ignoreCount = ignoreList.size();
	for (int i = 0; i < pathList.size(); ++i)
	{
		bool ignore = false;
		for (int j = 0; j < ignoreCount; ++j)
		{
			if (StringUtility::startWith(pathList[i], ignoreList[j]))
			{
				ignore = true;
				break;
			}
		}
		if (ignore)
		{
			pathList.erase(pathList.begin() + i);
			--i;
		}
	}
}