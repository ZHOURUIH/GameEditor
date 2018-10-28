#include "Utility.h"
#include "ConfigToolCore.h"
#include "WirelessDataManager.h"
#include "ANTPlusPacket.h"
#include "ANTPlusDataHeartBeat.h"
#include "ANTPlusDataRiding.h"

#include "WirelessDevicePanel.h"
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

BEGIN_EVENT_TABLE(WirelessDevicePanel, EditorPanel)
EVT_CHECKBOX(ID_CHECK_0, OnListCkeck0)
EVT_CHECKBOX(ID_CHECK_1, OnListCkeck1)
EVT_CHECKBOX(ID_CHECK_2, OnListCkeck2)
EVT_CHECKBOX(ID_CHECK_3, OnListCkeck3)
END_EVENT_TABLE()

WirelessDevicePanel::WirelessDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void WirelessDevicePanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizerCheckBox = new wxBoxSizer(wxHORIZONTAL);
	bSizerCheckBox->Add(mListCheck0 = new wxCheckBox(this, ID_CHECK_0, wxT("心率")));
	bSizerCheckBox->Add(mListCheck1 = new wxCheckBox(this, ID_CHECK_1, wxT("踏频速度")));
	bSizerCheckBox->Add(mListCheck2 = new wxCheckBox(this, ID_CHECK_2, wxT("踏频")));
	bSizerCheckBox->Add(mListCheck3 = new wxCheckBox(this, ID_CHECK_3, wxT("速度")));
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
	bSizer20->Add(mHeartRateDataList = new wxListCtrl(this, ID_DATA_LIST0, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);
	bSizer20->Add(mRidingFrequencySpeedLabel = new wxStaticText(this, wxID_ANY, wxT("踏频速度数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer20->Add(mRidingFrequencySpeedDataList = new wxListCtrl(this, ID_DATA_LIST1, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);
	bSizer20->Add(mRidingFrequencyLabel = new wxStaticText(this, wxID_ANY, wxT("踏频数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer20->Add(mRidingFrequencyDataList = new wxListCtrl(this, ID_DATA_LIST2, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);
	bSizer20->Add(mSpeedLabel = new wxStaticText(this, wxID_ANY, wxT("速度数据:")), 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer20->Add(mSpeedDataList = new wxListCtrl(this, ID_DATA_LIST3, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT), 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer20, 1, wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
}

void WirelessDevicePanel::init()
{
	// 为列表插入列
	const int heartRateColumnCount = 9;
	std::string heartRateColumn[heartRateColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "心率", "单车号", "更新超时时间", "接收超时时间" };
	int heartRateColumnWidth[heartRateColumnCount] = { 500, 90, 100, 200, 60, 50, 60, 100, 100 };
	for (int i = 0; i < heartRateColumnCount; ++i)
	{
		mHeartRateDataList->InsertColumn(i, heartRateColumn[i], 0, heartRateColumnWidth[i]);
	}

	const int ridingFrequencySpeedColumnCount = 10;
	std::string ridingFrequencySpeedColumn[ridingFrequencySpeedColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "踏频", "速度", "单车号", "更新超时时间", "接收超时时间" };
	int ridingFrequencySpeedColumnWidth[ridingFrequencySpeedColumnCount] = { 500, 90, 100, 200, 60, 50, 50, 60, 100, 100 };
	for (int i = 0; i < ridingFrequencySpeedColumnCount; ++i)
	{
		mRidingFrequencySpeedDataList->InsertColumn(i, ridingFrequencySpeedColumn[i], 0, ridingFrequencySpeedColumnWidth[i]);
	}

	const int ridingFrequencyColumnCount = 9;
	std::string ridingFrequencyColumn[ridingFrequencyColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "踏频", "单车号", "更新超时时间", "接收超时时间" };
	int ridingFrequencyColumnWidth[ridingFrequencyColumnCount] = { 500, 90, 100, 200, 60, 50, 60, 100, 100 };
	for (int i = 0; i < ridingFrequencyColumnCount; ++i)
	{
		mRidingFrequencyDataList->InsertColumn(i, ridingFrequencyColumn[i], 0, ridingFrequencyColumnWidth[i]);
	}

	const int speedColumnCount = 9;
	std::string speedColumn[speedColumnCount] = { "字节内容", "Device Type", "Device ID", "数据区", "信号强度", "速度", "单车号", "更新超时时间", "接收超时时间" };
	int speedColumnWidth[speedColumnCount] = { 500, 90, 100, 200, 60, 50, 60, 100, 100 };
	for (int i = 0; i < speedColumnCount; ++i)
	{
		mSpeedDataList->InsertColumn(i, speedColumn[i], 0, speedColumnWidth[i]);
	}
	mListCheck0->SetValue(mHeartRateDataList->IsShown());
	mListCheck1->SetValue(mRidingFrequencySpeedDataList->IsShown());
	mListCheck2->SetValue(mRidingFrequencyDataList->IsShown());
	mListCheck3->SetValue(mSpeedDataList->IsShown());
}

void WirelessDevicePanel::update(float elapsedTime)
{
	// 如果数据有更新
	ANTPlusPacket* heartBeatPacket = mWirelessDataManager->getHeartBeat();
	if (heartBeatPacket != NULL)
	{
		mLastHeartBeat->SetLabel(StringUtility::floatToString(heartBeatPacket->mLastReceivePacketTime, 1));
		mHeartBeatBytes->SetLabel(StringUtility::charArrayToHexString(heartBeatPacket->mBuffer, heartBeatPacket->mLength));
		mBattery->SetLabel(StringUtility::intToString(static_cast<ANTPlusDataHeartBeat*>(heartBeatPacket->mData)->mBattery));
	}
	auto& packetList = mWirelessDataManager->getPacketList();
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
	EditorUtility::clearUpList(mHeartRateDataList, heartRatePacketList.size());
	EditorUtility::clearUpList(mRidingFrequencySpeedDataList, ridingFrequencySpeedPacketList.size());
	EditorUtility::clearUpList(mRidingFrequencyDataList, ridingFrequencyPacketList.size());
	EditorUtility::clearUpList(mSpeedDataList, speedPacketList.size());

	// 心率数据
	auto iterHeartRate = heartRatePacketList.begin();
	auto iterHeartRateEnd = heartRatePacketList.end();
	FOR_STL(heartRatePacketList, int i = 0; iterHeartRate != iterHeartRateEnd; (++iterHeartRate, ++i))
	{
		ANTPlusPacket* packet = iterHeartRate->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 数据更新时才重新显示
		// 设置显示内容
		std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
		std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
		std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
		std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
		std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
		std::string heartRateStr = StringUtility::intToString(data->getHeartRate());
		std::string machineIndexStr = StringUtility::intToString(data->mMachineIndex);
		EditorUtility::updateItem(mHeartRateDataList, i, 0, byteStr);			// 字节十六进制内容
		EditorUtility::updateItem(mHeartRateDataList, i, 1, deviceTypeStr);		// DeviceID
		EditorUtility::updateItem(mHeartRateDataList, i, 2, deviceIDStr);		// DeviceType
		EditorUtility::updateItem(mHeartRateDataList, i, 3, dataByteStr);		// 数据区
		EditorUtility::updateItem(mHeartRateDataList, i, 4, RSSIStr);			// RSSI
		EditorUtility::updateItem(mHeartRateDataList, i, 5, heartRateStr);		// 心率
		EditorUtility::updateItem(mHeartRateDataList, i, 6, machineIndexStr);	// 机器号
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastUpdatePacketTime, 1);
		std::string lastReceivePacketTimeStr = StringUtility::floatToString(packet->mLastReceivePacketTime, 1);
		EditorUtility::updateItem(mHeartRateDataList, i, 7, lastPacketTimeStr);			// 更新超时时间
		EditorUtility::updateItem(mHeartRateDataList, i, 8, lastReceivePacketTimeStr);	// 接收超时时间
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
		// 设置显示内容
		std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
		std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
		std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
		std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
		std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
		std::string ridingFrequencyStr = StringUtility::intToString(data->getCadence());
		std::string speedStr = StringUtility::intToString(data->getSpeed(mWirelessDataManager->getWheelPerimeter()));
		std::string machineIndexStr = StringUtility::intToString(data->mMachineIndex);
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 0, byteStr);			// 字节十六进制内容
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 1, deviceTypeStr);		// DeviceID
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 2, deviceIDStr);		// DeviceType
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 3, dataByteStr);		// 数据区
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 4, RSSIStr);			// RSSI
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 5, ridingFrequencyStr);	// 踏频
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 6, speedStr);			// 速度
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 7, machineIndexStr);	// 机器号
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastUpdatePacketTime, 1);
		std::string lastReceivePacketTimeStr = StringUtility::floatToString(packet->mLastReceivePacketTime, 1);
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 8, lastPacketTimeStr);			// 更新超时时间
		EditorUtility::updateItem(mRidingFrequencySpeedDataList, i, 9, lastReceivePacketTimeStr);	// 接收超时时间
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
		// 设置显示内容
		std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
		std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
		std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
		std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
		std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
		std::string ridingFrequencyStr = StringUtility::intToString(data->getCadence());
		std::string machineIndexStr = StringUtility::intToString(data->mMachineIndex);
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 0, byteStr);				// 字节十六进制内容
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 1, deviceTypeStr);		// DeviceID
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 2, deviceIDStr);			// DeviceType
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 3, dataByteStr);			// 数据区
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 4, RSSIStr);				// RSSI
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 5, ridingFrequencyStr);	// 踏频
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 6, machineIndexStr);		// 机器号
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastUpdatePacketTime, 1);
		std::string lastReceivePacketTimeStr = StringUtility::floatToString(packet->mLastReceivePacketTime, 1);
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 7, lastPacketTimeStr);			// 更新超时时间
		EditorUtility::updateItem(mRidingFrequencyDataList, i, 8, lastReceivePacketTimeStr);	// 接收超时时间
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
		// 设置显示内容
		std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
		std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
		std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
		std::string dataByteStr = StringUtility::charArrayToHexString(data->mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
		std::string RSSIStr = StringUtility::intToString(data->mRSSIValue);
		std::string speedStr = StringUtility::intToString(data->getSpeed(mWirelessDataManager->getWheelPerimeter()));
		std::string machineIndexStr = StringUtility::intToString(data->mMachineIndex);
		EditorUtility::updateItem(mSpeedDataList, i, 0, byteStr);			// 字节十六进制内容
		EditorUtility::updateItem(mSpeedDataList, i, 1, deviceTypeStr);		// DeviceID
		EditorUtility::updateItem(mSpeedDataList, i, 2, deviceIDStr);		// DeviceType
		EditorUtility::updateItem(mSpeedDataList, i, 3, dataByteStr);		// 数据区
		EditorUtility::updateItem(mSpeedDataList, i, 4, RSSIStr);			// RSSI
		EditorUtility::updateItem(mSpeedDataList, i, 5, speedStr);			// 速度
		EditorUtility::updateItem(mSpeedDataList, i, 6, machineIndexStr);	// 机器号
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastUpdatePacketTime, 1);
		std::string lastReceivePacketTimeStr = StringUtility::floatToString(packet->mLastReceivePacketTime, 1);
		EditorUtility::updateItem(mSpeedDataList, i, 7, lastPacketTimeStr);			// 更新超时时间
		EditorUtility::updateItem(mSpeedDataList, i, 8, lastReceivePacketTimeStr);	// 接收超时时间
	}
	END_FOR_STL(speedPacketList);
}

void WirelessDevicePanel::OnListCkeck0(wxCommandEvent& event)
{
	mHeartRateLabel->Show(event.IsChecked());
	mHeartRateDataList->Show(event.IsChecked());
	Layout();
}

void WirelessDevicePanel::OnListCkeck1(wxCommandEvent& event)
{
	mRidingFrequencySpeedLabel->Show(event.IsChecked());
	mRidingFrequencySpeedDataList->Show(event.IsChecked());
	Layout();
}

void WirelessDevicePanel::OnListCkeck2(wxCommandEvent& event)
{
	mRidingFrequencyLabel->Show(event.IsChecked());
	mRidingFrequencyDataList->Show(event.IsChecked());
	Layout();
}

void WirelessDevicePanel::OnListCkeck3(wxCommandEvent& event)
{
	mSpeedLabel->Show(event.IsChecked());
	mSpeedDataList->Show(event.IsChecked());
	Layout();
}