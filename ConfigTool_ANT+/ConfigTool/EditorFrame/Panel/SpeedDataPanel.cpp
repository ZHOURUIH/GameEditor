#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"
#include "SpeedData.h"
#include "SpeedDataManager.h"
#include "SocketPacketFriction.h"
#include "SocketPacketColour.h"
#include "SocketNetManager.h"

#include "SpeedDataPanel.h"
#include "EditorUtility.h"

const int FRICTION_COUNT = 10;

enum
{
	ID_FRICTION_0,
	ID_DATA_LIST = ID_FRICTION_0 + FRICTION_COUNT,
	ID_CONFIRM_COLOR_BUTTON,
	ID_SPEED_RATIO_BUTTON,
};

BEGIN_EVENT_TABLE(SpeedDataPanel, EditorPanel)
EVT_BUTTON(ID_CONFIRM_COLOR_BUTTON, OnConfirmColor)
EVT_BUTTON(ID_SPEED_RATIO_BUTTON, OnSpeedRatio)
END_EVENT_TABLE()

SpeedDataPanel::SpeedDataPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

SpeedDataPanel::~SpeedDataPanel()
{
	int frictionCount = mFrictionRadioButtonList.size();
	for (int i = 0; i < frictionCount; ++i)
	{
		mFrictionRadioButtonList[i]->Disconnect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(SpeedDataPanel::OnFrictionChanged), NULL, this);
	}
}

void SpeedDataPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bSizer2->Add(mFrictionLabel = new wxStaticText(this, wxID_ANY, wxT("阻力:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	for (int i = 0; i < FRICTION_COUNT; ++i)
	{
		wxRadioButton* radioButton = new wxRadioButton(this, ID_FRICTION_0 + i, StringUtility::intToString(i + 1), wxDefaultPosition, wxDefaultSize, 0);
		bSizer2->Add(radioButton, 0, wxALL, 5);
		mFrictionRadioButtonList.push_back(radioButton);
	}
	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bSizer3->Add(new wxStaticText(this, wxID_ANY, wxT("R:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mColorRed = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(new wxStaticText(this, wxID_ANY, wxT("G:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mColorGreen = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(new wxStaticText(this, wxID_ANY, wxT("B:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mColorBlue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mConfirmColor = new wxButton(this, ID_CONFIRM_COLOR_BUTTON, wxT("发送"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);
	bSizer5->Add(new wxStaticText(this, wxID_ANY, wxT("转速比:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer5->Add(mSpeedRatio = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer5->Add(mSpeedRatioButton = new wxButton(this, ID_SPEED_RATIO_BUTTON, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	bSizer4->Add(mSpeedDataList = new wxListCtrl(this, ID_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer4, 10, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void SpeedDataPanel::init()
{
	int frictionCount = mFrictionRadioButtonList.size();
	for (int i = 0; i < frictionCount; ++i)
	{
		mFrictionRadioButtonList[i]->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(SpeedDataPanel::OnFrictionChanged), NULL, this);
	}

	// 为列表插入列
	const int columnCount = 7;
	std::string column[columnCount] = { "机器号", "字节内容", "功率", "转速", "阻力", "功率踏频", "转速踏频" };
	int width[columnCount] = { 100, 300, 100, 100, 100, 100, 100 };
	for (int i = 0; i < columnCount; ++i)
	{
		mSpeedDataList->InsertColumn(i, column[i], 0, width[i]);
	}
}

void SpeedDataPanel::refresh()
{
	mColorRed->SetValue("0");
	mColorGreen->SetValue("0");
	mColorBlue->SetValue("0");
	mSpeedRatio->SetValue(StringUtility::floatToString(mSpeedDataManager->getSpeedRatio(), 2));
}

void SpeedDataPanel::OnFrictionChanged(wxCommandEvent& event)
{
	int id = event.GetId();
	int friction = id - ID_FRICTION_0 + 1;
	SocketPacketFriction* packetFriction = static_cast<SocketPacketFriction*>(mSocketNetManager->createPacket(SP_FRICTION));
	packetFriction->setFrictionParam(friction * 10);
	mSocketNetManager->sendMessage(packetFriction);
}

void SpeedDataPanel::OnConfirmColor(wxCommandEvent& event)
{
	int red = StringUtility::stringToInt(mColorRed->GetValue().ToStdString());
	MathUtility::clamp(red, 0, 255);
	int green = StringUtility::stringToInt(mColorGreen->GetValue().ToStdString());
	MathUtility::clamp(green, 0, 255);
	int blue = StringUtility::stringToInt(mColorBlue->GetValue().ToStdString());
	MathUtility::clamp(blue, 0, 255);
	SocketPacketColour* packetColour = static_cast<SocketPacketColour*>(mSocketNetManager->createPacket(SP_COLOUR));
	packetColour->setColour(red, green, blue);
	mSocketNetManager->sendMessage(packetColour);
}

void SpeedDataPanel::OnSpeedRatio(wxCommandEvent& event)
{
	float speedRatio = StringUtility::stringToFloat(mSpeedRatio->GetValue().ToStdString());
	mSpeedDataManager->setSpeedRatio(speedRatio);
}

void SpeedDataPanel::update(float elapsedTime)
{
	if (mSpeedDataManager->isSpeedDataDirty())
	{
		showData(mSpeedDataManager->getSpeedDataList());
		mSpeedDataManager->setSpeedDataDirty(false);
	}
}

void SpeedDataPanel::showData(txMap<int, SpeedData*>& dataList)
{
	int itemCount = mSpeedDataList->GetItemCount();
	int shownCount = dataList.size();
	if (itemCount < shownCount)
	{
		EditorUtility::resetListItemCount(mSpeedDataList, shownCount);
	}
	// 将没有数据的显示内容清空
	EditorUtility::clearUpList(mSpeedDataList, shownCount);
	
	// 设置显示内容
	float speedRatio = mSpeedDataManager->getSpeedRatio();
	auto iterData = dataList.begin();
	auto iterDataEnd = dataList.end();
	for (int i = 0; iterData != iterDataEnd; ++iterData, ++i)
	{
		SpeedData* data = iterData->second;
		std::string machineIndeStr = StringUtility::intToString(data->mMachineIndex);
		std::string byteStr = StringUtility::charArrayToHexString(data->mData, data->mDataSize);
		std::string powerStr = StringUtility::intToString(data->mPower);
		std::string rotateSpeedStr = StringUtility::intToString(data->mRotateSpeed);
		std::string frictionStr = StringUtility::intToString(data->mFriction);
		std::string powerFrequency = StringUtility::intToString(ToolCoreUtility::HWSToStepFrequency(data->mPower, speedRatio));
		std::string rotateSpeedFrequency = StringUtility::intToString(ToolCoreUtility::HWSToStepFrequency(data->mRotateSpeed, speedRatio));
		EditorUtility::updateItem(mSpeedDataList, i, 0, machineIndeStr);		// 机器号
		EditorUtility::updateItem(mSpeedDataList, i, 1, byteStr);				// 字节十六进制内容
		EditorUtility::updateItem(mSpeedDataList, i, 2, powerStr);				// 功率
		EditorUtility::updateItem(mSpeedDataList, i, 3, rotateSpeedStr);		// 转速
		EditorUtility::updateItem(mSpeedDataList, i, 4, frictionStr);			// 阻力
		EditorUtility::updateItem(mSpeedDataList, i, 5, powerFrequency);		// 功率踏频
		EditorUtility::updateItem(mSpeedDataList, i, 6, rotateSpeedFrequency);	// 转速踏频
	}
}