#include "CardInfoPanel.h"
#include "ConfigToolCore.h"
#include "CommonDefine.h"
#include "ToolCoreUtility.h"
#include "CardData.h"

BEGIN_EVENT_TABLE(CardInfoPanel, wxPanel)
END_EVENT_TABLE()

CardInfoPanel::CardInfoPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void CardInfoPanel::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mDataLabel = new wxStaticText(this, wxID_ANY, wxT("字节内容:"), wxDefaultPosition, wxDefaultSize, 0);
	mDataLabel->Wrap(-1);
	bSizer2->Add(mDataLabel, 0, wxALL, 5);

	mDataValue = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mDataValue->Wrap(-1);
	bSizer2->Add(mDataValue, 0, wxALL, 5);

	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	mNameLabel = new wxStaticText(this, wxID_ANY, wxT("名字:"), wxDefaultPosition, wxDefaultSize, 0);
	mNameLabel->Wrap(-1);
	bSizer4->Add(mNameLabel, 0, wxALL, 5);

	mNameValue = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mNameValue->Wrap(-1);
	bSizer4->Add(mNameValue, 0, wxALL, 5);

	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	mIDLabel = new wxStaticText(this, wxID_ANY, wxT("ID:"), wxDefaultPosition, wxDefaultSize, 0);
	mIDLabel->Wrap(-1);
	bSizer5->Add(mIDLabel, 0, wxALL, 5);

	mIDValue = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mIDValue->Wrap(-1);
	bSizer5->Add(mIDValue, 0, wxALL, 5);

	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	mMachineIndexLabel = new wxStaticText(this, wxID_ANY, wxT("机器号:"), wxDefaultPosition, wxDefaultSize, 0);
	mMachineIndexLabel->Wrap(-1);
	bSizer3->Add(mMachineIndexLabel, 0, wxALL, 5);

	mMachineIndexValue = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mMachineIndexValue->Wrap(-1);
	bSizer3->Add(mMachineIndexValue, 0, wxALL, 5);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void CardInfoPanel::setCardInfo(CardData* cardData)
{
	std::string byteStr = txUtility::charArrayToHexString(cardData->mData, cardData->mDataSize);
	mDataValue->SetLabel(byteStr);
	mNameValue->SetLabel(/*txUtility::UnicodeToANSI*/(cardData->mName));
	mIDValue->SetLabel(txUtility::intToString(cardData->mCardID));
	mMachineIndexValue->SetLabel(txUtility::intToString(cardData->mMachineIndex));
}