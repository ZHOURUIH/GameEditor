#include "SpeedDataManager.h"
#include "txMemeryTrace.h"
#include "ComManager.h"

SpeedDataManager::SpeedDataManager()
:
CommandReceiver("SpeedDataManager")
{}

void SpeedDataManager::update(float elapsedTime)
{
	// 更新之前先同步列表
	syncList();
	txMap<std::string, DeviceData*> ::iterator iterSpeed = mSpeedDataList.begin();
	for (; iterSpeed != mSpeedDataList.end(); ++iterSpeed)
	{
		iterSpeed->second->update(elapsedTime);
	}
}

PARSE_RESULT SpeedDataManager::setData(const std::string& deviceName, unsigned char* data, const int& dataCount, int& parsedCount)
{
	LOCK(mReceivedListLock);
	DeviceData* deviceData = NULL;
	txMap<std::string, DeviceData*> ::iterator iterData = mReceivedPacketList.find(deviceName);
	if (iterData != mReceivedPacketList.end())
	{
		deviceData = iterData->second;
	}
	else
	{
		deviceData = TRACE_NEW(DeviceData, deviceData, deviceName);
		mReceivedPacketList.insert(deviceName, deviceData);
	}
	PARSE_RESULT ret = deviceData->parseData(data, dataCount, parsedCount);
	UNLOCK(mReceivedListLock);
	return ret;
}

void SpeedDataManager::clearData()
{
	txMap<std::string, DeviceData*>::iterator iterData = mSpeedDataList.begin();
	txMap<std::string, DeviceData*>::iterator iterDataEnd = mSpeedDataList.end();
	for (; iterData != iterDataEnd; ++iterData)
	{
		TRACE_DELETE(iterData->second);
	}
	mSpeedDataList.clear();
}

DeviceData* SpeedDataManager::getCurDeviceData()
{
	if (mSpeedDataList.find(mComManager->getCurDevice()) != mSpeedDataList.end())
	{
		return mSpeedDataList[mComManager->getCurDevice()];
	}
	return NULL;
}

void SpeedDataManager::generateFrictionData(const int& friction, char*& data, int& dataCount)
{
	// 阻力数据,cmd key keylen friction
	txSerializer frictionData;
	frictionData.write((unsigned char)CMD_FRICTION);
	frictionData.write((unsigned char)0x01);
	frictionData.write((unsigned char)0x01);
	frictionData.write((unsigned char)friction);
	// 包头
	txSerializer serializer;
	serializer.write((unsigned char)0xA5);
	serializer.write((unsigned char)0x00);
	unsigned short count = frictionData.getDataSize();
	serializer.write(count, true);
	unsigned short crc16 = BinaryUtility::crc16(0xFF, (unsigned char*)frictionData.getBuffer(), frictionData.getDataSize());
	serializer.write(crc16, true);
	unsigned short reserve = 0;
	serializer.write(reserve, true);
	// 在包头后添加阻力数据
	serializer.writeBuffer((char*)frictionData.getBuffer(), frictionData.getDataSize());

	dataCount = serializer.getDataSize();
	data = TRACE_NEW_ARRAY(char, dataCount, data);
	memcpy(data, serializer.getBuffer(), dataCount);
}

void SpeedDataManager::syncList()
{
	LOCK(mReceivedListLock);
	txMap<std::string, DeviceData*>::iterator iterReceived = mReceivedPacketList.begin();
	txMap<std::string, DeviceData*>::iterator iterReceivedEnd = mReceivedPacketList.end();
	for (; iterReceived != iterReceivedEnd; ++iterReceived)
	{
		// 已经存在则合并,然后销毁接收列表中的数据
		txMap<std::string, DeviceData*>::iterator iterSpeed = mSpeedDataList.find(iterReceived->first);
		if (iterSpeed != mSpeedDataList.end())
		{
			iterSpeed->second->merge(iterReceived->second);
			TRACE_DELETE(iterReceived->second);
		}
		// 不存在则将接收列表中的数据插入
		else
		{
			mSpeedDataList.insert(iterReceived->first, iterReceived->second);
		}
	}
	mReceivedPacketList.clear();
	UNLOCK(mReceivedListLock);
}