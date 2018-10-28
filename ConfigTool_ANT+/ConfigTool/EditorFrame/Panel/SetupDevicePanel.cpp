#include "Utility.h"
#include "ConfigToolCore.h"
#include "WirelessDataManager.h"
#include "ANTPlusPacket.h"
#include "ANTPlusDataRiding.h"
#include "DataBase.h"
#include "DeviceRegisteManager.h"
#include "LibcurlNetManager.h"
#include "SetupDevice.h"

#include "SetupDevicePanel.h"
#include "EditorUtility.h"

enum
{
	ID_DATA_PATH_TEXT,
	ID_SET_DATA_PATH_BUTTON,
	ID_REGISTE_BUTTON,
	ID_UNREGISTE_BUTTON,
	ID_RECEIVED_DEVICE_LIST,
	ID_REGISTED_DEVICE_LIST,
	ID_SAVE_DEVICE_LIST_BUTTON,
	ID_UPLOAD_DEVICE_LIST_BUTTON,
};

BEGIN_EVENT_TABLE(SetupDevicePanel, EditorPanel)
EVT_BUTTON(ID_SET_DATA_PATH_BUTTON, OnSetDataPathButton)
EVT_BUTTON(ID_REGISTE_BUTTON, OnRegisteButton)
EVT_BUTTON(ID_UNREGISTE_BUTTON, OnUnregisteButton)
EVT_LIST_ITEM_SELECTED(ID_RECEIVED_DEVICE_LIST, OnReceiveItemSelect)
EVT_LIST_ITEM_ACTIVATED(ID_RECEIVED_DEVICE_LIST, OnReceiveItemActive)
EVT_LIST_ITEM_SELECTED(ID_REGISTED_DEVICE_LIST, OnRegistedItemSelect)
EVT_LIST_ITEM_ACTIVATED(ID_REGISTED_DEVICE_LIST, OnRegistedItemActive)
EVT_BUTTON(ID_SAVE_DEVICE_LIST_BUTTON, OnSavedDeviceListButton)
EVT_BUTTON(ID_UPLOAD_DEVICE_LIST_BUTTON, OnUploadDeviceListButton)
END_EVENT_TABLE()

SetupDevicePanel::SetupDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name),
mReceiveDeviceSelectItem(-1),
mRegisteDeviceSelectItem(-1)
{
	setup();
}

void SetupDevicePanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);
	
	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("当前选中设备ID:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	bSizer2->Add(mDeviceIDLabel = new wxStaticText(this, wxID_ANY, wxT("FFFFFFFF"), wxDefaultPosition, wxSize(150, -1), 0), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("    "), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("该设备是否已绑定:"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer2->Add(mDeviceRegistedLabel = new wxStaticText(this, wxID_ANY, wxT("否"), wxDefaultPosition, wxSize(50, -1), 0), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("        "), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer2->Add(mRegisteButton = new wxButton(this, ID_REGISTE_BUTTON, wxT("绑定"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer2->Add(mMachineIndexText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0), 0, wxALL, 5);
	bSizer2->Add(new wxStaticText(this, wxID_ANY, wxT("号单车"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer2->Add(mUnregisteButton = new wxButton(this, ID_UNREGISTE_BUTTON, wxT("解除绑定"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxVERTICAL);
	bSizer5->Add(new wxStaticText(this, wxID_ANY, wxT("查找到的设备列表"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer5->Add(mReceivedDeviceList = new wxListCtrl(this, ID_RECEIVED_DEVICE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT | wxLC_VRULES | wxLC_SINGLE_SEL), 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bSizer3->Add(mRegisteDeviceTitle = new wxStaticText(this, wxID_ANY, wxT("已绑定的设备列表"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(new wxStaticText(this, wxID_ANY, wxT("   "), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mSaveRegisteDeviceButton = new wxButton(this, ID_SAVE_DEVICE_LIST_BUTTON, wxT("保存绑定信息"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mUploadButton = new wxButton(this, ID_UPLOAD_DEVICE_LIST_BUTTON, wxT("上传绑定信息"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer3->Add(mUploadStateLabel = new wxStaticText(this, wxID_ANY, wxT("上传失败!"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer1->Add(bSizer3, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer22 = new wxBoxSizer(wxHORIZONTAL);
	bSizer22->Add(new wxStaticText(this, wxID_ANY, wxT("表格路径："), wxDefaultPosition, wxDefaultSize, 0), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	bSizer22->Add(mDataPathText = new wxTextCtrl(this, ID_DATA_PATH_TEXT, wxEmptyString, wxDefaultPosition, wxSize(1000, -1)), 0, wxALL, 5);
	bSizer22->Add(mSetDataPathButton = new wxButton(this, ID_SET_DATA_PATH_BUTTON, wxT("设置表格路径"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALL, 5);
	bSizer1->Add(bSizer22, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	bSizer4->Add(mRegistedDeviceList = new wxListCtrl(this, ID_REGISTED_DEVICE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT | wxLC_VRULES | wxLC_SINGLE_SEL), 1, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	SetSizer(bSizer1);
	Layout();
}

void SetupDevicePanel::init()
{
	// 为列表插入列
	const int receiveDeviceColumnCount = 6;
	std::string reveiceDeviceColumn[receiveDeviceColumnCount] = { "字节内容", "Device Type", "类型名称", "Device ID", "单车号", "超时时间" };
	int receiveDeviceColumnWidth[receiveDeviceColumnCount] = { 500, 90, 90, 100, 60, 200 };
	for (int i = 0; i < receiveDeviceColumnCount; ++i)
	{
		mReceivedDeviceList->InsertColumn(i, reveiceDeviceColumn[i], 0, receiveDeviceColumnWidth[i]);
	}

	const int registeDeviceColumnCount = 3;
	std::string registeDeviceColumn[registeDeviceColumnCount] = { "Device ID", "Device Type", "单车号" };
	int registeDeviceColumnWidth[registeDeviceColumnCount] = { 500, 100, 90 };
	for (int i = 0; i < registeDeviceColumnCount; ++i)
	{
		mRegistedDeviceList->InsertColumn(i, registeDeviceColumn[i], 0, registeDeviceColumnWidth[i]);
	}
	setSelection(-1, -1, true);
	refreshRegisteList();
	notifyUploadState(US_NONE);
	mDataPathText->SetValue("./");
}

void SetupDevicePanel::update(float elapsedTime)
{
	refreshReceiveList();
}

void SetupDevicePanel::setDataPath()
{
	mConfigToolCore->setDataPath(mDataPathText->GetValue().ToStdString());
	int ret = MessageBoxA(NULL, "是否读取该文件夹中的数据?读取后会覆盖已设置的数据", "提示", MB_YESNO);
	if (ret == IDYES)
	{
		mDeviceRegisteManager->readFromDataBase(true);
	}
}

void SetupDevicePanel::OnSetDataPathButton(wxCommandEvent& event)
{
	setDataPath();
}

void SetupDevicePanel::OnRegisteButton(wxCommandEvent& event)
{
	// 将表格中的字符串转换为字节数组
	std::string deviceIDString = mReceivedDeviceList->GetItemText(mReceiveDeviceSelectItem, 3);
	char deviceType = StringUtility::stringToInt(mReceivedDeviceList->GetItemText(mReceiveDeviceSelectItem, 1).ToStdString());
	unsigned char* data = NULL;
	int dataCount = 0;
	StringUtility::hexStringToCharArray(deviceIDString, data, dataCount);
	int machineIndex = -1;
	if (mMachineIndexText->GetValue().ToStdString() != "")
	{
		machineIndex = StringUtility::stringToInt(mMachineIndexText->GetValue().ToStdString());
	}
	int ret = mDeviceRegisteManager->registeDevice((char*)data, machineIndex, deviceType);
	if (ret == 1)
	{
		TOOL_CORE_INFO("无线设备与单车已经绑定");
	}
	else if (ret == 2)
	{
		TOOL_CORE_INFO("单车已经绑定了其他无线设备");
	}
	else if (ret == 3)
	{
		TOOL_CORE_INFO("单车号无效");
	}
	TRACE_DELETE_ARRAY(data);
}

void SetupDevicePanel::OnUnregisteButton(wxCommandEvent& event)
{
	std::string deviceIDString;
	if (mRegisteDeviceSelectItem != -1)
	{
		deviceIDString = mRegistedDeviceList->GetItemText(mRegisteDeviceSelectItem, 0);
	}
	else if (mReceiveDeviceSelectItem != -1)
	{
		deviceIDString = mReceivedDeviceList->GetItemText(mReceiveDeviceSelectItem, 3);
	}
	unsigned char* data = NULL;
	int dataCount = 0;
	StringUtility::hexStringToCharArray(deviceIDString, data, dataCount);
	if (!mDeviceRegisteManager->unregisteDevice((char*)data))
	{
		TOOL_CORE_INFO("绑定解除失败!找不到该设备的绑定信息!");
	}
	TRACE_DELETE_ARRAY(data);
}

void SetupDevicePanel::OnReceiveItemSelect(wxListEvent& event)
{
	setSelection(event.GetIndex(), -1);
}

void SetupDevicePanel::OnReceiveItemActive(wxListEvent& event)
{
	setSelection(event.GetIndex(), -1);
}

void SetupDevicePanel::OnRegistedItemSelect(wxListEvent& event)
{
	setSelection(-1, event.GetIndex());
}

void SetupDevicePanel::OnRegistedItemActive(wxListEvent& event)
{
	setSelection(-1, event.GetIndex());
}

void SetupDevicePanel::OnSavedDeviceListButton(wxCommandEvent& event)
{
	mDeviceRegisteManager->saveToDataBase();
}

void SetupDevicePanel::OnUploadDeviceListButton(wxCommandEvent& event)
{
	mDeviceRegisteManager->uploadRegisteDeviceInfoList();
}

void SetupDevicePanel::refreshSelection()
{
	// 刷新当前选中项的信息显示
	std::string deviceIDString;
	bool deviceRegisted = false;
	// -2表示设备ID无效,-1表示该设备未注册
	int machineIndex = -2;
	if (mReceiveDeviceSelectItem != -1)
	{
		deviceIDString = mReceivedDeviceList->GetItemText(mReceiveDeviceSelectItem, 3);
	}
	else if (mRegisteDeviceSelectItem != -1)
	{
		deviceIDString = mRegistedDeviceList->GetItemText(mRegisteDeviceSelectItem, 0);
	}
	if (deviceIDString != EMPTY_STRING)
	{
		unsigned char* data = NULL;
		int dataCount = 0;
		StringUtility::hexStringToCharArray(deviceIDString, data, dataCount);
		machineIndex = -1;
		deviceRegisted = mDeviceRegisteManager->isDeviceRegisted((char*)data, machineIndex);
		TRACE_DELETE_ARRAY(data);
	}
	mDeviceIDLabel->SetLabel(deviceIDString);
	if (machineIndex != -2)
	{
		mDeviceRegistedLabel->SetLabel(deviceRegisted ? "是" : "否");
		mRegisteButton->Enable(!deviceRegisted);
		mUnregisteButton->Enable(deviceRegisted);
	}
	else
	{
		mDeviceRegistedLabel->SetLabel("");
		mRegisteButton->Enable(false);
		mUnregisteButton->Enable(false);
	}
	if (machineIndex >= 0)
	{
		mMachineIndexText->SetValue(StringUtility::intToString(machineIndex));
	}
	else
	{
		mMachineIndexText->SetValue("");
	}
}

void SetupDevicePanel::refreshReceiveList()
{
	// 如果数据有更新
	auto& packetList = mWirelessDataManager->getPacketList();
	// 将没有数据的显示内容清空
	EditorUtility::clearUpList(mReceivedDeviceList, packetList.size());
	auto iterDevice = packetList.begin();
	auto iterDeviceEnd = packetList.end();
	FOR_STL(packetList, int i = 0; iterDevice != iterDeviceEnd; (++iterDevice, ++i))
	{
		ANTPlusPacket* packet = iterDevice->second;
		ANTPlusDataRiding* data = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 设置显示内容
		std::string byteStr = StringUtility::charArrayToHexString(packet->mBuffer, packet->mBufferLength);
		std::string deviceTypeStr = StringUtility::intToString(data->mDeviceType);
		std::string deviceTypeName = ToolCoreUtility::deviceTypeToName(data->mDeviceType);
		std::string deviceIDStr = StringUtility::charArrayToHexString(data->mDeviceID, ANTPlusDataRiding::DEVICE_ID_LENGTH);
		std::string machineIndexStr = StringUtility::intToString(data->mMachineIndex);
		std::string lastPacketTimeStr = StringUtility::floatToString(packet->mLastReceivePacketTime, 1);
		EditorUtility::updateItem(mReceivedDeviceList, i, 0, byteStr);			// 字节十六进制内容
		EditorUtility::updateItem(mReceivedDeviceList, i, 1, deviceTypeStr);	// DeviceType
		EditorUtility::updateItem(mReceivedDeviceList, i, 2, deviceTypeName);	// 类型名称
		EditorUtility::updateItem(mReceivedDeviceList, i, 3, deviceIDStr);		// DeviceID
		EditorUtility::updateItem(mReceivedDeviceList, i, 4, machineIndexStr);	// 机器号
		EditorUtility::updateItem(mReceivedDeviceList, i, 5, lastPacketTimeStr);// 超时时间
	}
	END_FOR_STL(packetList);
}

void SetupDevicePanel::refreshRegisteList()
{
	auto& deviceRegisteList = mDeviceRegisteManager->getRegisteDeviceList();
	int registeCount = 0;
	auto iterRegiste = deviceRegisteList.begin();
	auto iterRegisteEnd = deviceRegisteList.end();
	FOR_STL(deviceRegisteList, ; iterRegiste != iterRegisteEnd; ++iterRegiste)
	{
		registeCount += iterRegiste->second.size();
	}
	END_FOR_STL(deviceRegisteList);
	EditorUtility::clearUpList(mRegistedDeviceList, registeCount);
	iterRegiste = deviceRegisteList.begin();
	FOR_STL(deviceRegisteList, int i = 0; iterRegiste != iterRegisteEnd; ++iterRegiste)
	{
		auto& deviceList = iterRegiste->second;
		auto iterDevice = deviceList.begin();
		auto iterDeviceEnd = deviceList.end();
		for (; iterDevice != iterDeviceEnd; (++iterDevice, ++i))
		{
			DeviceInfo* info = iterDevice->second;
			// 设置显示内容
			std::string deviceIDStr = StringUtility::charArrayToHexString((unsigned char*)info->mIDBytes, 4);
			std::string deviceTypeName = info->mDeviceTypeName;
			std::string machineIndexStr = StringUtility::intToString(info->mMachineIndex);
			EditorUtility::updateItem(mRegistedDeviceList, i, 0, deviceIDStr);		// DeviceID
			EditorUtility::updateItem(mRegistedDeviceList, i, 1, deviceTypeName);	// DeviceType
			EditorUtility::updateItem(mRegistedDeviceList, i, 2, machineIndexStr);	// 单车号
		}
	}
	END_FOR_STL(deviceRegisteList);
}

void SetupDevicePanel::notifyRegisteListModified(const bool& modified, const bool& forceRefresh)
{
	std::string label = "已绑定的设备列表";
	if (modified)
	{
		label += " *";
	}
	mRegisteDeviceTitle->SetLabel(label);
	notifyUploadState(US_UNUPLOAD);
	// 列表修改后或者需要强制刷新时才刷新列表显示
	if (modified || forceRefresh)
	{
		refreshRegisteList();
		setSelection(mReceiveDeviceSelectItem, -1, true);
	}
}

void SetupDevicePanel::notifyUploadState(const UPLOAD_STATE& state)
{
	if (state == US_NONE)
	{
		mUploadStateLabel->SetLabel("");
	}
	else if (state == US_UNUPLOAD)
	{
		mUploadStateLabel->SetLabel("未上传");
	}
	else if (state == US_UPLOADING)
	{
		mUploadStateLabel->SetLabel("正在上传...");
	}
	else if (state == US_UPLOADED)
	{
		mUploadStateLabel->SetLabel("上传成功");
	}
	else if (state == US_UPLOAD_FAILED)
	{
		mUploadStateLabel->SetLabel("上传失败");
	}
}

void SetupDevicePanel::setSelection(const int& receiveListSelect, const int& registeListSelect, const bool& forceRefresh)
{
	// 只要有选中项改变或者强制刷新时就需要刷新显示
	if (forceRefresh || registeListSelect != mRegisteDeviceSelectItem || receiveListSelect != mReceiveDeviceSelectItem)
	{
		mRegisteDeviceSelectItem = registeListSelect;
		mReceiveDeviceSelectItem = receiveListSelect;
		if (mRegisteDeviceSelectItem != -1 && mReceiveDeviceSelectItem != -1)
		{
			TOOL_CORE_INFO("不能同时选中两个列表的设备!");
		}
		refreshSelection();
	}
}