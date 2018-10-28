#include "WirelessDataManager.h"
#include "CTEventSystem.h"
#include "ANTPlusPacket.h"
#include "ANTPlusData.h"
#include "Utility.h"
#include "LogSystem.h"
#include "GameConfig.h"
#include "DeviceRegisteManager.h"
#include "ANTPlusDataRiding.h"

WirelessDataManager::WirelessDataManager()
:
txCommandReceiver(TOSTRING(DataManager)),
mSpeedRatio(4.5f),
mHeartBeatPacket(NULL),
mWheelPerimeter(2.0f)
{
	;
}

void WirelessDataManager::init()
{
	;
}

void WirelessDataManager::update(float elapsedTime)
{
	auto iter = mPacketList.begin();
	auto iterEnd = mPacketList.end();
	FOR_STL(mPacketList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mPacketList);
	if (mHeartBeatPacket != NULL)
	{
		mHeartBeatPacket->update(elapsedTime);
	}
}

void WirelessDataManager::destroy()
{
	clearData(); 
}

void WirelessDataManager::receivePacket(ANTPlusPacket* packet)
{
	if (packet->mType == APPT_HEART_BEAT)
	{
		TRACE_DELETE(mHeartBeatPacket);
		mHeartBeatPacket = packet;
	}
	else
	{
		ANTPlusDataRiding* newDeviceData = static_cast<ANTPlusDataRiding*>(packet->mData);
		// 查找设备对应的单车号,保存到设备中
		newDeviceData->mMachineIndex = findMachineIndex((char*)newDeviceData->mDeviceID);
		int deviceID = ToolCoreUtility::toDeviceID((char*)newDeviceData->mDeviceID);
		if (mPacketList.find(deviceID) == mPacketList.end())
		{
			mPacketList.insert(deviceID, packet);
		}
		else
		{
			ANTPlusPacket* thisPacket = mPacketList[deviceID];
			thisPacket->notifyReceived();
			// 判断两次数据是否相同,如果在一定时间内是相同数据,则丢弃该数据
			if (thisPacket->mLastUpdatePacketTime < PACKET_TIME_OUT)
			{
				ANTPlusDataRiding* thisRidingData = static_cast<ANTPlusDataRiding*>(thisPacket->mData);
				if (thisRidingData->isSameDataValue(newDeviceData))
				{
					TRACE_DELETE(packet);
					return;
				}
			}
			// 有新数据,或者超时需要更新数据
			packet->updateData(thisPacket);
			mPacketList[deviceID] = packet;
		}
	}
}

void WirelessDataManager::clearData()
{
	auto iter = mPacketList.begin();
	auto iterEnd = mPacketList.end();
	FOR_STL(mPacketList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mPacketList);
	TRACE_DELETE(mHeartBeatPacket);
}

int WirelessDataManager::findMachineIndex(char* deviceID)
{
	int machineIndex = -2;
	mDeviceRegisteManager->isDeviceRegisted(deviceID, machineIndex);
	return machineIndex;
}