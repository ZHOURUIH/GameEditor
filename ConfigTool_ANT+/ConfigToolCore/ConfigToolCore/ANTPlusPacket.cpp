#include "ANTPlusPacket.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"
#include "txSerializer.h"
#include "ANTPlusData.h"

ANTPlusPacket::ANTPlusPacket()
{
	mBuffer = NULL;
	mBufferLength = 0;
	mLength = 0;
	mCmdID = 0;
	memset(mVersionInfo, 0, 2);
	mHubID = 0;
	mData = NULL;
	mCRC16 = 0;
	mHasRead = true;
	mLastPacketTime = 0.0f;
	mLastPacket = NULL;
}
ANTPlusPacket::~ANTPlusPacket()
{
	TRACE_DELETE(mData);
	TRACE_DELETE_ARRAY(mBuffer);
	TRACE_DELETE(mLastPacket);
}
PARSE_RESULT ANTPlusPacket::parseData(unsigned char* data, const int& length)
{
	saveLastPacket();
	txSerializer serializer((char*)data, length);
	serializer.read(mLength);
	// 数据不足
	if (mLength > length)
	{
		return PR_NOT_ENOUGH;
	}
	serializer.read(mCmdID);
	// 数据错误
	if (mCmdID != 0x01 && mCmdID != 0x02)
	{
		return PR_ERROR;
	}
	serializer.readBuffer((char*)mVersionInfo, 2, 2);
	serializer.read(mHubID);
	checkANTPlusData(mCmdID);
	char* tempBuffer = TRACE_NEW_ARRAY(char, mData->mLength, tempBuffer);
	serializer.readBuffer(tempBuffer, mData->mLength, mData->mLength);
	mData->setData(tempBuffer);
	TRACE_DELETE_ARRAY(tempBuffer);
	serializer.read(mCRC16, true);
	unsigned short crc = BinaryUtility::CRC_Calc16(data, mLength - sizeof(mCRC16));
	if (crc != mCRC16)
	{
		return PR_ERROR;
	}
	resizeBuffer(mLength);
	memcpy(mBuffer, data, mLength);
	mHasRead = false;
	mLastPacketTime = 0.0f;
	return PR_SUCCESS;
}
void ANTPlusPacket::setNewData(ANTPlusPacket* that)
{
	if (that == NULL)
	{
		return;
	}
	saveLastPacket();
	mLength = that->mLength;
	mCmdID = that->mCmdID;
	memcpy(mVersionInfo, that->mVersionInfo, 2);
	mHubID = that->mHubID;
	checkANTPlusData(mCmdID);
	mData->setData(that->mData->mBuffer);
	mCRC16 = that->mCRC16;
	resizeBuffer(mLength);
	memcpy(mBuffer, that->mBuffer, mLength);
	mHasRead = false;
	mLastPacketTime = 0.0f;
}
void ANTPlusPacket::resizeBuffer(const int& size)
{
	if (size == mBufferLength)
	{
		return;
	}
	TRACE_DELETE_ARRAY(mBuffer);
	mBufferLength = size;
	mBuffer = TRACE_NEW_ARRAY(unsigned char, mBufferLength, mBuffer);
}
void ANTPlusPacket::checkANTPlusData(const unsigned char& cmdID)
{
	// 数据类型不一致才重新创建
	if (mData != NULL)
	{
		if (cmdID != mData->mCmdID)
		{
			TRACE_DELETE(mData);
		}
	}
	if (mData == NULL)
	{
		if (cmdID == 0x01)
		{
			mData = TRACE_NEW(ANTPlusDataRiding, mData, this);
		}
		else if (cmdID == 0x02)
		{
			mData = TRACE_NEW(ANTPlusDataHeartBeat, mData, this);
		}
	}
}
void ANTPlusPacket::saveLastPacket()
{
	// 上一次的数据为空,并且已经有数据时,需要备份上一次的数据
	if (mLastPacket == NULL && mBuffer != NULL)
	{
		mLastPacket = TRACE_NEW(ANTPlusPacket, mLastPacket);
	}
	if (mLastPacket != NULL)
	{
		mLastPacket->mLength = mLength;
		mLastPacket->mCmdID = mCmdID;
		memcpy(mLastPacket->mVersionInfo, mVersionInfo, 2);
		mLastPacket->mHubID = mHubID;
		mLastPacket->checkANTPlusData(mCmdID);
		mLastPacket->mData->setData(mData->mBuffer);
		mLastPacket->mCRC16 = mCRC16;
		mLastPacket->resizeBuffer(mLength);
		memcpy(mLastPacket->mBuffer, mBuffer, mLength);
	}
}