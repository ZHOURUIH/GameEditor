#include "SpeedDataManager.h"
#include "txMemeryTrace.h"
#include "txSerializer.h"

SpeedDataManager::SpeedDataManager()
:
CommandReceiver(TOSTRING(SpeedDataManager))
{}

void SpeedDataManager::update(float elapsedTime)
{
	// 更新之前先同步列表
	syncList();
	if (mSpeedData != NULL)
	{
		mSpeedData->update(elapsedTime);
	}
}

PARSE_RESULT SpeedDataManager::setData(unsigned char* data, const int& dataCount, int& parsedCount)
{
	LOCK(mReceivedListLock);
	if (mReceivedPacket == NULL)
	{
		mReceivedPacket = TRACE_NEW(DeviceData, mReceivedPacket);
	}
	PARSE_RESULT ret = mReceivedPacket->parseData(data, dataCount, parsedCount);
	UNLOCK(mReceivedListLock);
	return ret;
}

void SpeedDataManager::clearData()
{
	TRACE_DELETE(mSpeedData);
}

void SpeedDataManager::generateFrictionData(const int& friction, unsigned char*& data, int& dataCount)
{
	// 阻力数据,cmd key keylen friction
	txSerializer frictionData;
	frictionData.write((unsigned char)REPORT_OUT);
	frictionData.write((unsigned char)CMD_FRICTION);
	frictionData.write((unsigned char)0x01);
	frictionData.write((unsigned char)0x01);
	frictionData.write((unsigned char)friction);
	// 包头
	txSerializer serializer;
	serializer.write((unsigned char)0xA5);
	serializer.write((unsigned char)0x00);
	serializer.write((unsigned short)frictionData.getDataSize(), true);
	unsigned short crc16 = BinaryUtility::crc16(0xFF, (unsigned char*)frictionData.getBuffer(), frictionData.getDataSize());
	serializer.write(crc16, true);
	serializer.write((unsigned short)0, true);
	// 在包头后添加阻力数据
	serializer.writeBuffer((char*)frictionData.getBuffer(), frictionData.getDataSize());

	dataCount = serializer.getDataSize();
	data = TRACE_NEW_ARRAY(unsigned char, dataCount, data);
	memcpy(data, serializer.getBuffer(), dataCount);
}

void SpeedDataManager::syncList()
{
	LOCK(mReceivedListLock);
	if (mReceivedPacket != NULL)
	{
		if (mSpeedData != NULL)
		{
			mSpeedData->merge(mReceivedPacket);
		}
		else
		{
			mSpeedData = mReceivedPacket;
		}
		mReceivedPacket = NULL;
	}
	UNLOCK(mReceivedListLock);
}