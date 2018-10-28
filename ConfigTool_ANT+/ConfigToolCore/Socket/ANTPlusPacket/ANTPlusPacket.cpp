#include "ANTPlusPacket.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"
#include "txSerializer.h"
#include "ANTPlusData.h"
#include "ToolCoreBase.h"
#include "LogSystem.h"
#include "SocketWireless.h"
#include "WirelessDataManager.h"

ANTPlusPacket::ANTPlusPacket(ANT_PLUS_PACKET_TYPE type)
{
	mType = type;
	mBuffer = NULL;
	mBufferLength = 0;
	mLength = 0;
	mCmdID = 0;
	memset(mVersionInfo, 0, 2);
	mHubID = 0;
	mData = NULL;
	mCRC16 = 0;
	mLastUpdatePacketTime = 0.0f;
	mLastReceivePacketTime = 0.0f;
	mLastPacket = NULL;
}
ANTPlusPacket::~ANTPlusPacket()
{
	TRACE_DELETE(mData);
	TRACE_DELETE_ARRAY(mBuffer);
	TRACE_DELETE(mLastPacket);
}

void ANTPlusPacket::update(float elapsedTime)
{
	mLastUpdatePacketTime += elapsedTime;
	mLastReceivePacketTime += elapsedTime;
	// 如果超过一定时间没有接收到数据,则清空上一次记录的数据包
	if (mLastReceivePacketTime >= PACKET_TIME_OUT && mLastPacket != NULL)
	{
		TRACE_DELETE(mLastPacket);
	}
}

PARSE_RESULT ANTPlusPacket::read(unsigned char* data, int length, int& readCount)
{
	PARSE_RESULT result = parse(this, data, length);
	if (result == PR_SUCCESS)
	{
		readCount = mLength;
	}
	return result;
}

void ANTPlusPacket::execute()
{
	mWirelessDataManager->receivePacket(this);
}

void ANTPlusPacket::updateData(ANTPlusPacket* lastPacket)
{
	TRACE_DELETE(mLastPacket);
	mLastPacket = lastPacket;
	resetLastUpdateTime();
	writeUpdateLog();
}

void ANTPlusPacket::notifyReceived()
{
	resetLastReceiveTime();
	writeReceiveLog();
}

void ANTPlusPacket::resizeBuffer(int size)
{
	if (size == mBufferLength)
	{
		return;
	}
	TRACE_DELETE_ARRAY(mBuffer);
	mBufferLength = size;
	mBuffer = TRACE_NEW_ARRAY(unsigned char, mBufferLength, mBuffer);
}

PARSE_RESULT ANTPlusPacket::parse(ANTPlusPacket* packet, unsigned char* data, int length)
{
	if (packet == NULL)
	{
		return PR_ERROR;
	}
	txSerializer serializer((char*)data, length);
	serializer.read(packet->mLength);
	// 数据不足
	if (packet->mLength > length)
	{
		return PR_NOT_ENOUGH;
	}
	serializer.read(packet->mCmdID);
	// 数据错误
	if (packet->mCmdID != PC_RIDING && packet->mCmdID != PC_HEART_BEAT)
	{
		return PR_ERROR;
	}
	serializer.readBuffer((char*)packet->mVersionInfo, 2, 2);
	serializer.read(packet->mHubID);
	packet->checkANTPlusData();
	char* tempBuffer = TRACE_NEW_ARRAY(char, packet->mData->mLength, tempBuffer);
	serializer.readBuffer(tempBuffer, packet->mData->mLength, packet->mData->mLength);
	packet->mData->setData(tempBuffer);
	TRACE_DELETE_ARRAY(tempBuffer);
	serializer.read(packet->mCRC16, true);
	unsigned short crc = BinaryUtility::CRC_Calc16(data, packet->mLength - sizeof(packet->mCRC16));
	if (crc != packet->mCRC16)
	{
		return PR_ERROR;
	}
	packet->resizeBuffer(packet->mLength);
	memcpy(packet->mBuffer, data, packet->mLength);
	return PR_SUCCESS;
}

void ANTPlusPacket::copyPacket(ANTPlusPacket* source, ANTPlusPacket* dest)
{
	dest->mLength = source->mLength;
	dest->mCmdID = source->mCmdID;
	memcpy(dest->mVersionInfo, source->mVersionInfo, 2);
	dest->mHubID = source->mHubID;
	dest->checkANTPlusData();
	dest->mData->copyData(source->mData);
	dest->mCRC16 = source->mCRC16;
	dest->resizeBuffer(dest->mLength);
	memcpy(dest->mBuffer, source->mBuffer, dest->mLength);
}

ANT_PLUS_PACKET_TYPE ANTPlusPacket::checkType(unsigned char* data, int length)
{
	if (data == NULL || length == 0)
	{
		return APPT_NONE;
	}
	if (data[0] > length)
	{
		return APPT_NONE;
	}
	if (data[1] == PC_RIDING)
	{
		static int deviceTypeIndex = sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned char)* 2 + sizeof(int);
		if (data[deviceTypeIndex] == RD_CADENCE)
		{
			return APPT_CADENCE;
		}
		else if (data[deviceTypeIndex] == RD_SPEED)
		{
			return APPT_SPEED;
		}
		else if (data[deviceTypeIndex] == RD_CADENCE_SPEED)
		{
			return APPT_CADENCE_SPEED;
		}
		else if (data[deviceTypeIndex] == RD_HEART_RATE)
		{
			return APPT_HEART_RATE;
		}
	}
	else if (data[1] == PC_HEART_BEAT)
	{
		return APPT_HEART_BEAT;
	}
	return APPT_NONE;
}