#include "DataManager.h"
#include "CTEventSystem.h"
#include "ANTPlusPacket.h"
#include "ANTPlusData.h"
#include "txMemoryTrace.h"
#include "ToolCoreUtility.h"
#include "ToolCoreLog.h"

DataManager::DataManager()
:
txCommandReceiver(TOSTRING(DataManager)),
mSpeedRatio(4.5f),
mHeartBeatPacket(NULL),
mWheelPerimeter(2.0f)
{
	mTempHeartBeatPacket = TRACE_NEW(ANTPlusPacket, mTempHeartBeatPacket);
}

void DataManager::update(float elapsedTime)
{
	syncList();
	auto iter = mPacketList.begin();
	auto iterEnd = mPacketList.end();
	FOR_STL(mPacketList, ; iter != iterEnd; ++iter)
	{
		iter->second->mLastPacketTime += elapsedTime;
	}
	END_FOR_STL(mPacketList);
	if (mHeartBeatPacket != NULL)
	{
		mHeartBeatPacket->mLastPacketTime += elapsedTime;
	}
}

void DataManager::destroy()
{
	auto iter = mTempPacketList.begin();
	auto iterEnd = mTempPacketList.end();
	FOR_STL(mTempPacketList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mTempPacketList);
	TRACE_DELETE(mTempHeartBeatPacket);
	clearData(); 
}

PARSE_RESULT DataManager::setData(unsigned char* data, const int& dataCount, int& parsedCount)
{
	LOCK(mReceivedLock);
	// 根据CmdID判断数据类型
	// 心率数据
	PARSE_RESULT result;
	if (data[0] == 0x1A && data[1] == 0x01)
	{
		// 使用临时对象解析数据
		static ANTPlusPacket tempPacket;
		result = tempPacket.parseData(data, dataCount);
		if (result == PR_SUCCESS)
		{
			parsedCount = tempPacket.mLength;
		}
		ANTPlusDataRiding* heartRateData = static_cast<ANTPlusDataRiding*>(tempPacket.mData);
		// 根据deviceID查找是否已经存在该心率带
		int deviceID = 0;
		memcpy(&deviceID, heartRateData->mDeviceID, 4);
		if (mTempPacketList.find(deviceID) == mTempPacketList.end())
		{
			ANTPlusPacket* packet = TRACE_NEW(ANTPlusPacket, packet);
			mTempPacketList.insert(deviceID, packet);
		}
		mTempPacketList[deviceID]->setNewData(&tempPacket);
	}
	// 心跳数据
	else if (data[0] == 0x0D && data[1] == 0x02)
	{
		result = mTempHeartBeatPacket->parseData(data, dataCount);
		if (result == PR_SUCCESS)
		{
			parsedCount = mTempHeartBeatPacket->mLength;
		}
	}
	else
	{
		result = PR_ERROR;
	}
	UNLOCK(mReceivedLock);
	return result;
}

void DataManager::clearData()
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

void DataManager::syncList()
{
	LOCK(mReceivedLock);
	auto iter = mTempPacketList.begin();
	auto iterEnd = mTempPacketList.end();
	FOR_STL(mTempPacketList, ; iter != iterEnd; ++iter)
	{
		if (!iter->second->mHasRead)
		{
			if (mPacketList.find(iter->first) == mPacketList.end())
			{
				ANTPlusPacket* packet = TRACE_NEW(ANTPlusPacket, packet);
				mPacketList.insert(iter->first, packet);
			}
			mPacketList[iter->first]->setNewData(iter->second);
			iter->second->mHasRead = true;
		}
	}
	END_FOR_STL(mTempPacketList);
	if (mTempHeartBeatPacket != NULL && !mTempHeartBeatPacket->mHasRead)
	{
		if (mHeartBeatPacket == NULL)
		{
			mHeartBeatPacket = TRACE_NEW(ANTPlusPacket, mHeartBeatPacket);
		}
		mHeartBeatPacket->setNewData(mTempHeartBeatPacket);
		mTempHeartBeatPacket->mHasRead = true;
	}
	UNLOCK(mReceivedLock);
}