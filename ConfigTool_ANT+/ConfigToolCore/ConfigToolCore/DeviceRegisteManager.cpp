#include "DeviceRegisteManager.h"
#include "Utility.h"
#include "DataBase.h"
#include "DataRegisteDevice.h"
#include "ConfigToolCore.h"
#include "SetupDevice.h"
#include "LibcurlNetManager.h"

void DeviceRegisteManager::init()
{
	readFromDataBase(false);
}

void DeviceRegisteManager::update(float elapsedTime)
{
	;
}

void DeviceRegisteManager::destroy()
{
	;
}

int DeviceRegisteManager::registeDevice(char* deviceID, int machineIndex, char deviceType)
{
	if (machineIndex < 0)
	{
		return 3;
	}
	// 如果该单车号已经绑定了无线设备,则返回失败
	int id = ToolCoreUtility::toDeviceID(deviceID);
	if (mMachineIndexDeviceList.find(machineIndex) != mMachineIndexDeviceList.end())
	{
		txMap<char, DeviceInfo*>& deviceList = mMachineIndexDeviceList[machineIndex];
		auto iter = deviceList.find(deviceType);
		// 有相同类型的设备
		if (iter != deviceList.end())
		{
			// 设备ID号相同
			if (iter->second->mID == id)
			{
				return 1;
			}
			// ID号不同
			else
			{
				return 2;
			}
		}
	}
	// 如果无线设备已经绑定了某一辆单车,则先进行解绑
	if (mDeviceMachineList.find(id) != mDeviceMachineList.end())
	{
		removeFromList(mDeviceMachineList[id]);
	}
	DeviceInfo* info = TRACE_NEW(DeviceInfo, info);
	info->mID = id;
	info->mMachineIndex = machineIndex;
	info->mDeviceType = deviceType;
	info->mDeviceTypeName = ToolCoreUtility::deviceTypeToName(info->mDeviceType);
	memcpy(info->mIDBytes, deviceID, sizeof(int));
	addToList(info);
	mConfigToolCore->sendEvent(CE_REGISTE_DEVICE_LIST_MODIFIED, StringUtility::boolToString(true));
	return 0;
}

bool DeviceRegisteManager::unregisteDevice(char* deviceID)
{
	int id = ToolCoreUtility::toDeviceID(deviceID);
	auto iterDeviceID = mDeviceMachineList.find(id);
	if (iterDeviceID == mDeviceMachineList.end())
	{
		return false;
	}
	DeviceInfo* info = iterDeviceID->second;
	removeFromList(info);
	TRACE_DELETE(info);
	mConfigToolCore->sendEvent(CE_REGISTE_DEVICE_LIST_MODIFIED, StringUtility::boolToString(true));
	return true;
}

bool DeviceRegisteManager::unregisteDeviceFromMachineIndex(int machineIndex)
{
	auto iterMachineIndex = mMachineIndexDeviceList.find(machineIndex);
	if (iterMachineIndex == mMachineIndexDeviceList.end())
	{
		return false;
	}
	auto deviceList = iterMachineIndex->second;
	auto iterDevice = deviceList.begin();
	auto iterDeviceEnd = deviceList.end();
	for (; iterDevice != iterDeviceEnd; ++iterDevice)
	{
		DeviceInfo* info = iterDevice->second;
		removeFromList(info);
		TRACE_DELETE(info);
	}
	mConfigToolCore->sendEvent(CE_REGISTE_DEVICE_LIST_MODIFIED, StringUtility::boolToString(true));
	return true;
}

bool DeviceRegisteManager::isDeviceRegisted(char* deviceID, int& machineIndex)
{
	int id = ToolCoreUtility::toDeviceID(deviceID);
	auto iterDeviceID = mDeviceMachineList.find(id);
	if (iterDeviceID == mDeviceMachineList.end())
	{
		return false;
	}
	machineIndex = iterDeviceID->second->mMachineIndex;
	return true;
}

void DeviceRegisteManager::readFromDataBase(bool checkExist)
{
	std::vector<Data*> dataList;
	// 如果没有数据表格,并且不清空时,则返回,不对数据做修改
	if (!mDataBase->getDataList(DT_REGISTE_DEVICE, dataList))
	{
		if (checkExist)
		{
			TOOL_CORE_INFO("目录中找不到表格!");
		}
		return;
	}
	// 先全部注销
	unregisteAllDevice();
	// 然后从表格中读取所有注册的设备信息
	int dataCount = mDataBase->getDataCount(DT_REGISTE_DEVICE);
	for (int i = 0; i < dataCount; ++i)
	{
		DataRegisteDevice* data = (DataRegisteDevice*)mDataBase->queryData(DT_REGISTE_DEVICE, i);
		registeDevice(data->mDeviceID, data->mMachineIndex, data->mDeviceType);
	}
	mConfigToolCore->sendEvent(CE_REGISTE_DEVICE_LIST_MODIFIED, StringUtility::boolToString(false));
}

void DeviceRegisteManager::saveToDataBase()
{
	// 清空表格中的数据
	mDataBase->clearData(DT_REGISTE_DEVICE);
	// 然后将列表中的注册数据保存到表格中
	auto iter = mMachineIndexDeviceList.begin();
	auto iterEnd = mMachineIndexDeviceList.end();
	FOR_STL(mMachineIndexDeviceList, ; iter != iterEnd; ++iter)
	{
		auto& deviceList = iter->second;
		auto iterDevice = deviceList.begin();
		auto iterDeviceEnd = deviceList.end();
		for (; iterDevice != iterDeviceEnd; ++iterDevice)
		{
			DataRegisteDevice* data = (DataRegisteDevice*)mDataBase->createData(DT_REGISTE_DEVICE);
			memcpy(data->mDeviceID, iterDevice->second->mIDBytes, 4);
			data->mMachineIndex = iterDevice->second->mMachineIndex;
			data->mDeviceType = iterDevice->second->mDeviceType;
			mDataBase->addData(DT_REGISTE_DEVICE, data);
		}
	}
	END_FOR_STL(mMachineIndexDeviceList);
	// 写入文件
	mDataBase->writeBinaryFile(DT_REGISTE_DEVICE);
	mConfigToolCore->sendEvent(CE_REGISTE_DEVICE_LIST_MODIFIED, StringUtility::boolToString(false));
}

void DeviceRegisteManager::uploadRegisteDeviceInfoList()
{
	mConfigToolCore->sendEvent(CE_UPLOAD_STATE, StringUtility::intToString(US_UPLOADING));
	SetupDevice* setupDevicePacket = static_cast<SetupDevice*>(mLibcurlNetManager->createPacket(LP_SETUP_DEVICE));
	setupDevicePacket->setDeviceInfoList(mMachineIndexDeviceList);
	mLibcurlNetManager->sendMessage(setupDevicePacket);
}

void DeviceRegisteManager::unregisteAllDevice()
{
	auto temp = mMachineIndexDeviceList;
	auto iter = temp.begin();
	auto iterEnd = temp.end();
	FOR_STL(temp, ; iter != iterEnd; ++iter)
	{
		auto& deviceList = iter->second;
		auto iterDevice = deviceList.begin();
		auto iterDeviceEnd = deviceList.end();
		for (; iterDevice != iterDeviceEnd; ++iterDevice)
		{
			unregisteDeviceFromMachineIndex(iterDevice->second->mMachineIndex);
		}
	}
	END_FOR_STL(temp);
}

void DeviceRegisteManager::removeFromList(DeviceInfo* info)
{
	mDeviceMachineList.erase(info->mID);
	mMachineIndexDeviceList.erase(info->mMachineIndex);
}

void DeviceRegisteManager::addToList(DeviceInfo* info)
{
	mDeviceMachineList.insert(info->mID, info);
	if (mMachineIndexDeviceList.find(info->mMachineIndex) == mMachineIndexDeviceList.end())
	{
		txMap<char, DeviceInfo*> deviceList;
		deviceList.insert(info->mDeviceType, info);
		mMachineIndexDeviceList.insert(info->mMachineIndex, deviceList);
	}
	else
	{
		mMachineIndexDeviceList[info->mMachineIndex].insert(info->mDeviceType, info);
	}
}