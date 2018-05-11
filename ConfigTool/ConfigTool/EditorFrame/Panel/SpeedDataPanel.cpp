#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"
#include "SpeedData.h"
#include "SpeedDataManager.h"

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

BEGIN_EVENT_TABLE(SpeedDataPanel, wxPanel)
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
	mFrictionLabel = new wxStaticText(this, wxID_ANY, wxT("阻力:"), wxDefaultPosition, wxDefaultSize, 0);
	mFrictionLabel->Wrap(-1);
	bSizer2->Add(mFrictionLabel, 0, wxALL, 5);
	for (int i = 0; i < FRICTION_COUNT; ++i)
	{
		wxRadioButton* radioButton = new wxRadioButton(this, ID_FRICTION_0 + i, EditorUtility::intToString(i + 1), wxDefaultPosition, wxDefaultSize, 0);
		bSizer2->Add(radioButton, 0, wxALL, 5);
		mFrictionRadioButtonList.push_back(radioButton);
	}
	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("R:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_staticText1, 0, wxALL, 5);
	mColorRed = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mColorRed, 0, wxALL, 5);
	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("G:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_staticText2, 0, wxALL, 5);
	mColorGreen = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mColorGreen, 0, wxALL, 5);
	wxStaticText* m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("B:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_staticText3, 0, wxALL, 5);
	mColorBlue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mColorBlue, 0, wxALL, 5);
	mConfirmColor = new wxButton(this, ID_CONFIRM_COLOR_BUTTON, wxT("发送"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mConfirmColor, 0, wxALL, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("转速比:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(m_staticText4, 0, wxALL, 5);
	mSpeedRatio = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mSpeedRatio, 0, wxALL, 5);
	mSpeedRatioButton = new wxButton(this, ID_SPEED_RATIO_BUTTON, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mSpeedRatioButton, 0, wxALL, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	mSpeedDataList = new wxListCtrl(this, ID_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer4->Add(mSpeedDataList, 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer4, 10, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

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
	mSpeedRatio->SetValue(txUtility::floatToString(mSpeedDataManager->getSpeedRatio(), 2));
}

void SpeedDataPanel::OnFrictionChanged(wxCommandEvent& event)
{
	int id = event.GetId();
	mConfigToolCore->setFriction(id - ID_FRICTION_0 + 1);
}

void SpeedDataPanel::OnConfirmColor(wxCommandEvent& event)
{
	int red = txUtility::stringToInt(mColorRed->GetValue().ToStdString());
	txUtility::clamp(red, 0, 255);
	int green = txUtility::stringToInt(mColorGreen->GetValue().ToStdString());
	txUtility::clamp(green, 0, 255);
	int blue = txUtility::stringToInt(mColorBlue->GetValue().ToStdString());
	txUtility::clamp(blue, 0, 255);
	mConfigToolCore->setColor(red, green, blue);
}

void SpeedDataPanel::OnSpeedRatio(wxCommandEvent& event)
{
	float speedRatio = txUtility::stringToFloat(mSpeedRatio->GetValue().ToStdString());
	mSpeedDataManager->setSpeedRatio(speedRatio);
}

void SpeedDataPanel::showData(std::map<int, SpeedData*>& dataList)
{
	int itemCount = mSpeedDataList->GetItemCount();
	int shownCount = dataList.size();
	if (itemCount < shownCount)
	{
		resetListItemCount(shownCount);
	}
	// 将没有数据的显示内容清空
	for (int i = shownCount; i < itemCount; ++i)
	{
		if (mSpeedDataList->GetItemText(i, 0) != "")
		{
			int columnCount = mSpeedDataList->GetColumnCount();
			for (int j = 0; j < columnCount; ++j)
			{
				mSpeedDataList->SetItem(i, j, wxT(""));
			}
		}
	}
	
	// 设置显示内容
	float speedRatio = mSpeedDataManager->getSpeedRatio();
	std::map<int, SpeedData*>::iterator iterData = dataList.begin();
	std::map<int, SpeedData*>::iterator iterDataEnd = dataList.end();
	for (int i = 0; iterData != iterDataEnd; ++iterData, ++i)
	{
		SpeedData* data = iterData->second;
		std::string machineIndeStr = txUtility::intToString(data->mMachineIndex);
		std::string byteStr = txUtility::charArrayToHexString(data->mData, data->mDataSize);
		std::string powerStr = txUtility::intToString(data->mPower);
		std::string rotateSpeedStr = txUtility::intToString(data->mRotateSpeed);
		std::string frictionStr = txUtility::intToString(data->mFriction);
		std::string powerFrequency = txUtility::intToString(ToolCoreUtility::HWSToStepFrequency(data->mPower, speedRatio));
		std::string rotateSpeedFrequency = txUtility::intToString(ToolCoreUtility::HWSToStepFrequency(data->mRotateSpeed, speedRatio));
		updateItem(i, 0, machineIndeStr);		// 机器号
		updateItem(i, 1, byteStr);				// 字节十六进制内容
		updateItem(i, 2, powerStr);				// 功率
		updateItem(i, 3, rotateSpeedStr);		// 转速
		updateItem(i, 4, frictionStr);			// 阻力
		updateItem(i, 5, powerFrequency);		// 功率踏频
		updateItem(i, 6, rotateSpeedFrequency);	// 转速踏频
	}
}

void SpeedDataPanel::resetListItemCount(int count)
{
	int curItemCount = mSpeedDataList->GetItemCount();
	int deltaCount = curItemCount - count;
	// 行数多了,则删除多余的
	if (deltaCount > 0)
	{
		for (int i = 0; i < deltaCount; ++i)
		{
			mSpeedDataList->DeleteItem(curItemCount - i - 1);
		}
	}
	// 行数少了,则添加
	else if (deltaCount < 0)
	{
		deltaCount = -deltaCount;
		for (int i = 0; i < deltaCount; ++i)
		{
			mSpeedDataList->InsertItem(curItemCount + i, wxT(""), -1);
		}
	}
	else
	{
		return;
	}
}

void SpeedDataPanel::updateItem(const int& item, const int& column, const std::string& text)
{
	if (mSpeedDataList->GetItemText(item, column) != text)
	{
		mSpeedDataList->SetItem(item, column, text);
	}
}