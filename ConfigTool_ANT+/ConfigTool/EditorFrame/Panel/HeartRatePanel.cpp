#include "Utility.h"
#include "ConfigToolCore.h"
#include "HeartRateData.h"
#include "SpeedDataManager.h"

#include "HeartRatePanel.h"
#include "EditorUtility.h"

enum
{
	ID_DATA_LIST,
	ID_CONFIRM_TIME_BUTTON,
};

BEGIN_EVENT_TABLE(HeartRatePanel, EditorPanel)
EVT_BUTTON(ID_CONFIRM_TIME_BUTTON, OnConfirmTimeOut)
END_EVENT_TABLE()

HeartRatePanel::HeartRatePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void HeartRatePanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("设置心跳超时:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(mHeartRateTimeOutEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("秒"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(mConfirmTimeOut = new wxButton(this, ID_CONFIRM_TIME_BUTTON, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("当前超时时间:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(mCurHeartRateTimeOut = new wxStaticText(this, wxID_ANY, wxT("1秒"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	bSizer4->Add(mHeartRateDataList = new wxListCtrl(this, ID_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer4, 10, wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
}

void HeartRatePanel::init()
{
	// 为列表插入列
	const int columnCount = 3;
	std::string column[columnCount] = { "心率带编号", "字节内容", "心率" };
	int width[columnCount] = { 200, 300, 100 };
	for (int i = 0; i < columnCount; ++i)
	{
		mHeartRateDataList->InsertColumn(i, column[i], 0, width[i]);
	}
}

void HeartRatePanel::update(float elapsedTime)
{
	if (mSpeedDataManager->isHeartRateDataDirty())
	{
		showData(mSpeedDataManager->getHeartRateDataList());
		mSpeedDataManager->setHeartRateDataDirty(false);
	}
}

void HeartRatePanel::showData(txMap<std::string, HeartRateData*>& dataList)
{
	int itemCount = mHeartRateDataList->GetItemCount();
	int shownCount = dataList.size();
	if (itemCount < shownCount)
	{
		EditorUtility::resetListItemCount(mHeartRateDataList, shownCount);
	}
	// 将没有数据的显示内容清空
	EditorUtility::clearUpList(mHeartRateDataList, shownCount);

	// 设置显示内容
	auto iterData = dataList.begin();
	auto iterDataEnd = dataList.end();
	for (int i = 0; iterData != iterDataEnd; ++iterData, ++i)
	{
		HeartRateData* data = iterData->second;
		std::string number = data->mNumber;
		std::string byteStr = StringUtility::charArrayToHexString(data->mData, data->mDataSize);
		std::string heartRateStr = StringUtility::intToString(data->mHeartRate);
		EditorUtility::updateItem(mHeartRateDataList, i, 0, number);			// 心率带编号
		EditorUtility::updateItem(mHeartRateDataList, i, 1, byteStr);			// 字节十六进制内容
		EditorUtility::updateItem(mHeartRateDataList, i, 2, heartRateStr);		// 心率
	}
}

void HeartRatePanel::refreshTime()
{
	mCurHeartRateTimeOut->SetLabel(StringUtility::floatToString(mSpeedDataManager->getHeartRateTimeOut(), 1) + "秒");
}

void HeartRatePanel::OnConfirmTimeOut(wxCommandEvent& e)
{
	mSpeedDataManager->setHeartRateTimeOut(StringUtility::stringToFloat(mHeartRateTimeOutEdit->GetValue().ToStdString()));
}