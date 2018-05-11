#include "Utility.h"
#include "ConfigToolCore.h"
#include "DataManager.h"
#include "ANTPlusPacket.h"
#include "ANTPlusData.h"

#include "HeartRatePanel.h"
#include "EditorUtility.h"

enum
{
	ID_CHECK_0,
	ID_CHECK_1,
	ID_CHECK_2,
	ID_CHECK_3,
	ID_DATA_LIST0,
	ID_DATA_LIST1,
	ID_DATA_LIST2,
	ID_DATA_LIST3,
};

BEGIN_EVENT_TABLE(HeartRatePanel, EditorPanel)
EVT_CHECKBOX(ID_CHECK_0, OnListCkeck0)
EVT_CHECKBOX(ID_CHECK_1, OnListCkeck1)
EVT_CHECKBOX(ID_CHECK_2, OnListCkeck2)
EVT_CHECKBOX(ID_CHECK_3, OnListCkeck3)
END_EVENT_TABLE()

HeartRatePanel::HeartRatePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
	init();
}

void HeartRatePanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizerCheckBox = new wxBoxSizer(wxHORIZONTAL);
	mListCheck0 = new wxCheckBox(this, ID_CHECK_0, wxT("心率"));
	bSizerCheckBox->Add(mListCheck0);
	mListCheck1 = new wxCheckBox(this, ID_CHECK_1, wxT("踏频速度"));
	bSizerCheckBox->Add(mListCheck1);
	mListCheck2 = new wxCheckBox(this, ID_CHECK_2, wxT("踏频"));
	bSizerCheckBox->Add(mListCheck2);
	mListCheck3 = new wxCheckBox(this, ID_CHECK_3, wxT("速度"));
	bSizerCheckBox->Add(mListCheck3);
	bSizer1->Add(bSizerCheckBox, 0, wxEXPAND, 5);

	wxBoxSizer* bSizerHeartBeat = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("上次接收到心跳的时间:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(mLastHeartBeat = new wxStaticText(this, wxID_ANY, wxT("0")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("秒")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizerHeartBeat->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bSizer3->Add(new wxStaticText(this, wxID_ANY, wxT("心跳包:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer3->Add(mHeartBeatBytes = new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizerHeartBeat->Add(bSizer3, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
	bSizer4->Add(new wxStaticText(this, wxID_ANY, wxT("电量:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer4->Add(mBattery = new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizerHeartBeat->Add(bSizer4, 0, wxEXPAND, 5);
	bSizer1->Add(bSizerHeartBeat, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer20 = new wxBoxSizer(wxVERTICAL);
	bSizer20->Add(mHeartRateLabel = new wxStaticText(this, wxID_ANY, wxT("心率数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	mHeartRateDataList = new wxListCtrl(this, ID_DATA_LIST0, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer20->Add(mHeartRateDataList, 1, wxALL | wxEXPAND, 5);

	bSizer20->Add(mRidingFrequencySpeedLabel = new wxStaticText(this, wxID_ANY, wxT("踏频速度数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	mRidingFrequencySpeedDataList = new wxListCtrl(this, ID_DATA_LIST1, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer20->Add(mRidingFrequencySpeedDataList, 1, wxALL | wxEXPAND, 5);

	bSizer20->Add(mRidingFrequencyLabel = new wxStaticText(this, wxID_ANY, wxT("踏频数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	mRidingFrequencyDataList = new wxListCtrl(this, ID_DATA_LIST2, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer20->Add(mRidingFrequencyDataList, 1, wxALL | wxEXPAND, 5);

	bSizer20->Add(mSpeedLabel = new wxStaticText(this, wxID_ANY, wxT("速度数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	mSpeedDataList = new wxListCtrl(this, ID_DATA_LIST3, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer20->Add(mSpeedDataList, 1, wxALL | wxEXPAND, 5);
	
	bSizer1->Add(bSizer20, 1, wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
}

void HeartRatePanel::init()
{
	// 为列表插入列
	const int heartRateColumnCount = 7;
	std::string heartRateColumn[heartRateColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "心率", "超时时间" };
	int heartRateColumnWidth[heartRateColumnCount] = { 500, 90, 100, 200, 60, 50, 60 };
	for (int i = 0; i < heartRateColumnCount; ++i)
	{
		mHeartRateDataList->InsertColumn(i, heartRateColumn[i], 0, heartRateColumnWidth[i]);
	}

	const int ridingFrequencySpeedColumnCount = 8;
	std::string ridingFrequencySpeedColumn[ridingFrequencySpeedColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "踏频", "速度", "超时时间" };
	int ridingFrequencySpeedColumnWidth[ridingFrequencySpeedColumnCount] = { 500, 90, 100, 200, 60, 50, 50, 60 };
	for (int i = 0; i < ridingFrequencySpeedColumnCount; ++i)
	{
		mRidingFrequencySpeedDataList->InsertColumn(i, ridingFrequencySpeedColumn[i], 0, ridingFrequencySpeedColumnWidth[i]);
	}

	const int ridingFrequencyColumnCount = 7;
	std::string ridingFrequencyColumn[ridingFrequencyColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "踏频", "超时时间" };
	int ridingFrequencyColumnWidth[ridingFrequencyColumnCount] = { 500, 90, 100, 200, 60, 50, 60 };
	for (int i = 0; i < ridingFrequencyColumnCount; ++i)
	{
		mRidingFrequencyDataList->InsertColumn(i, ridingFrequencyColumn[i], 0, ridingFrequencyColumnWidth[i]);
	}

	const int speedColumnCount = 7;
	std::string speedColumn[speedColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "速度", "超时时间" };
	int speedColumnWidth[speedColumnCount] = { 500, 90, 100, 200, 60, 50, 60 };
	for (int i = 0; i < speedColumnCount; ++i)
	{
		mSpeedDataList->InsertColumn(i, speedColumn[i], 0, speedColumnWidth[i]);
	}
	mListCheck0->SetValue(mHeartRateDataList->IsShown());
	mListCheck1->SetValue(mRidingFrequencySpeedDataList->IsShown());
	mListCheck2->SetValue(mRidingFrequencyDataList->IsShown());
	mListCheck3->SetValue(mSpeedDataList->IsShown());
}

void HeartRatePanel::update(float elapsedTime)
{
	// 如果数据有更新
	ANTPlusPacket* heartBeatPacket = mDataManager->getHeartBeat();
	if (heartBeatPacket != NULL)
	{
		mLastHeartBeat->SetLabel(StringUtility::floatToString(heartBeatPacket->mLastPacketTime, 1));
		if (!heartBeatPacket->mHasRead)
		{
			mHeartBeatBytes->SetLabel(StringUtility::charArrayToHexString(heartBeatPacket->mBuffer, heartBeatPacket->mLength));
			mBattery->SetLabel(StringUtility::intToString(static_cast<ANTPlusDataHeartBeat*>(heartBeatPacket->mData)->mBattery));
			heartBeatPacket->mHasRead = true;
		}
	}
	auto& packetList = mDataManager->getPacketList();
	// 根据数据类型,放入对应的列表中方便处理
	txMap<int, ANTPlusPacket*> heartRatePacketList;
	txMap<int, ANTPlusPacket*> ridingFrequencySpeedPacketList;
	txMap<int, ANTPlusPacket*> ridingFrequencyPacketList;
	txMap<int, ANTPlusPacket*> speedPacketList;
	auto iter = packetList.begin();
	auto iterEnd = packetList.end();
	FOR_STL(packetList, ; iter != iterEnd; ++iter)
	{
		ANTPlusPacket* packet = iter->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		if (data->mRidingDevice == RD_HEART_RATE)
		{
			heartRatePacketList.insert(iter->first, iter->second);
		}
		else if (data->mRidingDevice == RD_CADENCE_SPEED)
		{
			ridingFrequencySpeedPacketList.insert(iter->first, iter->second);
		}
		else if (data->mRidingDevice == RD_CADENCE)
		{
			ridingFrequencyPacketList.insert(iter->first, iter->second);
		}
		else if (data->mRidingDevice == RD_SPEED)
		{
			speedPacketList.insert(iter->first, iter->second);
		}
	}
	END_FOR_STL(packetList);
	// 将没有数据的显示内容清空
	clearUpList(mHeartRateDataList, heartRatePacketList.size());
	clearUpList(mRidingFrequencySpeedDataList, ridingFrequencySpeedPacketList.size());
	clearUpList(mRidingFrequencyDataList, ridingFrequencyPacketList.size());
	clearUpList(mSpeedDataList, speedPacketList.size());

	// 心率数据
	auto iterHeartRate = heartRatePacketList.begin();
	auto iterHeartRateEnd = heartRatePacketList.end();
	FOR_STL(heartRatePacketList, int i = 0; iterHeartRate != iterHeartRateEnd; (++iterHeartRate, ++i))
	{
		ANTPlusPacket* packet = iterHeartRate->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 数据更新时才重新显示
		if (!packet->mHasRead)
		{
			// 设置显示内容
			std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
			std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
			std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
			std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
			std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
			std::string heartRateStr = StringUtility::intToString(data->getHeartRate());
			updateItem(mHeartRateDataList, i, 0, byteStr);			// 字节十六进制内容
			updateItem(mHeartRateDataList, i, 1, deviceTypeStr);	// DeviceID
			updateItem(mHeartRateDataList, i, 2, deviceIDStr);		// DeviceType
			updateItem(mHeartRateDataList, i, 3, dataByteStr);		// 数据区
			updateItem(mHeartRateDataList, i, 4, RSSIStr);			// RSSI
			updateItem(mHeartRateDataList, i, 5, heartRateStr);		// 心率
			packet->mHasRead = true;
		}
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastPacketTime, 1);
		updateItem(mHeartRateDataList, i, 6, lastPacketTimeStr);		// 超时时间
	}
	END_FOR_STL(heartRatePacketList);

	// 踏频速度数据
	auto iterRidingFrequencySpeed = ridingFrequencySpeedPacketList.begin();
	auto iterRidingFrequencySpeedEnd = ridingFrequencySpeedPacketList.end();
	FOR_STL(ridingFrequencySpeedPacketList, int i = 0; iterRidingFrequencySpeed != iterRidingFrequencySpeedEnd; (++iterRidingFrequencySpeed, ++i))
	{
		ANTPlusPacket* packet = iterRidingFrequencySpeed->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 数据更新时才重新显示
		if (!packet->mHasRead)
		{
			// 设置显示内容
			std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
			std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
			std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
			std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
			std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
			std::string ridingFrequencyStr = StringUtility::intToString(data->getCadence());
			std::string speedStr = StringUtility::intToString(data->getSpeed(mDataManager->getWheelPerimeter()));
			updateItem(mRidingFrequencySpeedDataList, i, 0, byteStr);			// 字节十六进制内容
			updateItem(mRidingFrequencySpeedDataList, i, 1, deviceTypeStr);		// DeviceID
			updateItem(mRidingFrequencySpeedDataList, i, 2, deviceIDStr);		// DeviceType
			updateItem(mRidingFrequencySpeedDataList, i, 3, dataByteStr);		// 数据区
			updateItem(mRidingFrequencySpeedDataList, i, 4, RSSIStr);			// RSSI
			updateItem(mRidingFrequencySpeedDataList, i, 5, ridingFrequencyStr);// 踏频
			updateItem(mRidingFrequencySpeedDataList, i, 6, speedStr);			// 速度
			packet->mHasRead = true;
		}
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastPacketTime, 1);
		updateItem(mRidingFrequencySpeedDataList, i, 7, lastPacketTimeStr);		// 超时时间
	}
	END_FOR_STL(ridingFrequencySpeedPacketList);

	// 踏频数据
	auto iterRidingFrequency = ridingFrequencyPacketList.begin();
	auto iterRidingFrequencyEnd = ridingFrequencyPacketList.end();
	FOR_STL(ridingFrequencyPacketList, int i = 0; iterRidingFrequency != iterRidingFrequencyEnd; (++iterRidingFrequency, ++i))
	{
		ANTPlusPacket* packet = iterRidingFrequency->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 数据更新时才重新显示
		if (!packet->mHasRead)
		{
			// 设置显示内容
			std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
			std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
			std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
			std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
			std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
			std::string ridingFrequencyStr = StringUtility::intToString(data->getCadence());
			updateItem(mRidingFrequencyDataList, i, 0, byteStr);			// 字节十六进制内容
			updateItem(mRidingFrequencyDataList, i, 1, deviceTypeStr);		// DeviceID
			updateItem(mRidingFrequencyDataList, i, 2, deviceIDStr);		// DeviceType
			updateItem(mRidingFrequencyDataList, i, 3, dataByteStr);		// 数据区
			updateItem(mRidingFrequencyDataList, i, 4, RSSIStr);			// RSSI
			updateItem(mRidingFrequencyDataList, i, 5, ridingFrequencyStr);	// 踏频
			packet->mHasRead = true;
		}
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastPacketTime, 1);
		updateItem(mRidingFrequencyDataList, i, 6, lastPacketTimeStr);		// 超时时间
	}
	END_FOR_STL(ridingFrequencyPacketList);

	// 速度数据
	auto iterSpeed = speedPacketList.begin();
	auto iterSpeedEnd = speedPacketList.end();
	FOR_STL(speedPacketList, int i = 0; iterSpeed != iterSpeedEnd; (++iterSpeed, ++i))
	{
		ANTPlusPacket* packet = iterSpeed->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 数据更新时才重新显示
		if (!packet->mHasRead)
		{
			// 设置显示内容
			std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
			std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
			std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
			std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
			std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
			std::string speedStr = StringUtility::intToString(data->getSpeed(mDataManager->getWheelPerimeter()));
			updateItem(mSpeedDataList, i, 0, byteStr);		// 字节十六进制内容
			updateItem(mSpeedDataList, i, 1, deviceTypeStr);// DeviceID
			updateItem(mSpeedDataList, i, 2, deviceIDStr);	// DeviceType
			updateItem(mSpeedDataList, i, 3, dataByteStr);	// 数据区
			updateItem(mSpeedDataList, i, 4, RSSIStr);		// RSSI
			updateItem(mSpeedDataList, i, 5, speedStr);		// 速度
			packet->mHasRead = true;
		}
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastPacketTime, 1);
		updateItem(mSpeedDataList, i, 6, lastPacketTimeStr);		// 超时时间
	}
	END_FOR_STL(speedPacketList);
}

void HeartRatePanel::OnListCkeck0(wxCommandEvent& event)
{
	mHeartRateLabel->Show(event.IsChecked());
	mHeartRateDataList->Show(event.IsChecked());
	Layout();
}

void HeartRatePanel::OnListCkeck1(wxCommandEvent& event)
{
	mRidingFrequencySpeedLabel->Show(event.IsChecked());
	mRidingFrequencySpeedDataList->Show(event.IsChecked());
	Layout();
}

void HeartRatePanel::OnListCkeck2(wxCommandEvent& event)
{
	mRidingFrequencyLabel->Show(event.IsChecked());
	mRidingFrequencyDataList->Show(event.IsChecked());
	Layout();
}

void HeartRatePanel::OnListCkeck3(wxCommandEvent& event)
{
	mSpeedLabel->Show(event.IsChecked());
	mSpeedDataList->Show(event.IsChecked());
	Layout();
}

void HeartRatePanel::clearUpList(wxListCtrl* list, const int& shownCount)
{
	int itemCount = list->GetItemCount();
	if (itemCount < shownCount)
	{
		resetListItemCount(list, shownCount);
	}
	clearEmptyRow(list, shownCount);
}

void HeartRatePanel::resetListItemCount(wxListCtrl* listCtrl, const int& count)
{
	int curItemCount = listCtrl->GetItemCount();
	int deltaCount = curItemCount - count;
	// 行数多了,则删除多余的
	if (deltaCount > 0)
	{
		for (int i = 0; i < deltaCount; ++i)
		{
			listCtrl->DeleteItem(curItemCount - i - 1);
		}
	}
	// 行数少了,则添加
	else if (deltaCount < 0)
	{
		deltaCount = -deltaCount;
		for (int i = 0; i < deltaCount; ++i)
		{
			listCtrl->InsertItem(curItemCount + i, wxT(""), -1);
		}
	}
	else
	{
		return;
	}
}

void HeartRatePanel::clearEmptyRow(wxListCtrl* listCtrl, const int& shownCount)
{
	int itemCount = listCtrl->GetItemCount();
	for (int i = shownCount; i < itemCount; ++i)
	{
		if (listCtrl->GetItemText(i, 0) != "")
		{
			int columnCount = listCtrl->GetColumnCount();
			for (int j = 0; j < columnCount; ++j)
			{
				listCtrl->SetItem(i, j, wxT(""));
			}
		}
	}
}

void HeartRatePanel::updateItem(wxListCtrl* listCtrl, const int& item, const int& column, const std::string& text)
{
	if (listCtrl->GetItemText(item, column) != text)
	{
		listCtrl->SetItem(item, column, text);
	}
}