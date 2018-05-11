#include "ANTPlusData.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusData::ANTPlusData(ANTPlusPacket* packet)
{
	mParentPacket = packet;
}
ANTPlusData::~ANTPlusData()
{
	TRACE_DELETE_ARRAY(mBuffer);
}
void ANTPlusData::setData(char* data)
{
	memcpy(mBuffer, data, mLength);
	parseData();
}
void ANTPlusData::setLength(const int& length)
{
	mLength = length;
	mBuffer = TRACE_NEW_ARRAY(char, mLength, mBuffer);
}

//---------------------------------------------------------------------------------------------------------------
ANTPlusDataRiding::ANTPlusDataRiding(ANTPlusPacket* packet)
	:ANTPlusData(packet)
{
	mCmdID = 0x01;
	setLength(16);
}
int ANTPlusDataRiding::getHeartRate()
{
	if (mRidingDevice != RD_HEART_RATE)
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0;
	}
	return mPageData[PAGE_DATA_LENGTH - 1];
}
int ANTPlusDataRiding::getCadence()
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	if (mRidingDevice == RD_HEART_RATE)
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0;
	}
	else if (mRidingDevice == RD_CADENCE_SPEED)
	{
		int index = 2;
		unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
		unsigned short lastCount = BinaryUtility::read(lastCount, lastData->mPageData, index);
		index = 2;
		unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
		unsigned short curCount = BinaryUtility::read(curCount, mPageData, index);
		unsigned short timeDelta = curTime - lastTime;
		unsigned short countDelta = curCount - lastCount;
		if (timeDelta != 0)
		{
			return (int)(countDelta / (timeDelta / 1024.0f) * 60.0f);
		}
		else
		{
			return 0;
		}
	}
	else if (mRidingDevice == RD_CADENCE)
	{
		int index = 4;
		unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
		unsigned short lastCount = BinaryUtility::read(lastCount, lastData->mPageData, index);
		index = 4;
		unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
		unsigned short curCount = BinaryUtility::read(curCount, mPageData, index);
		unsigned short timeDelta = curTime - lastTime;
		unsigned short countDelta = curCount - lastCount;
		if (timeDelta != 0)
		{
			int cadence = (int)(countDelta / (timeDelta / 1024.0f) * 60.0f);
			std::string curDataStr = "cur data : " + StringUtility::charArrayToHexString(mPageData, PAGE_DATA_LENGTH);
			std::string lastDataStr = "last data : " + StringUtility::charArrayToHexString(lastData->mPageData, PAGE_DATA_LENGTH);
			OutputDebugStringA((curDataStr + "|||" + lastDataStr + "---: " + StringUtility::intToString(cadence) + "\n").c_str());
			return cadence;
		}
		else
		{
			return 0;
		}
	}
	else if (mRidingDevice == RD_SPEED)
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0;
	}
	else
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0;
	}
}
float ANTPlusDataRiding::getSpeed(const float& perimeter)
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0.0f;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	if (mRidingDevice == RD_HEART_RATE)
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0.0f;
	}
	else if (mRidingDevice == RD_CADENCE_SPEED)
	{
		int index = 4;
		unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
		unsigned short lastCircle = BinaryUtility::read(lastCircle, lastData->mPageData, index);
		index = 4;
		unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
		unsigned short curCircle = BinaryUtility::read(curCircle, mPageData, index);
		unsigned short timeDelta = curTime - lastTime;
		unsigned short circleDelta = curCircle - lastCircle;
		if (timeDelta != 0)
		{
			return circleDelta * perimeter / (timeDelta / 1024.0f);
		}
		else
		{
			return 0.0f;
		}
	}
	else if (mRidingDevice == RD_CADENCE)
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0.0f;
	}
	else if (mRidingDevice == RD_SPEED)
	{
		int index = 4;
		unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
		unsigned short lastCircle = BinaryUtility::read(lastCircle, lastData->mPageData, index);
		index = 4;
		unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
		unsigned short curCircle = BinaryUtility::read(curCircle, mPageData, index);
		unsigned short timeDelta = curTime - lastTime;
		unsigned short circleDelta = curCircle - lastCircle;
		if (timeDelta != 0)
		{
			return circleDelta * perimeter / (timeDelta / 1024.0f);
		}
		else
		{
			return 0.0f;
		}
	}
	else
	{
		TOOL_CORE_ERROR("packet type error!");
		return 0.0f;
	}
}
void ANTPlusDataRiding::parseData()
{
	txSerializer serializer(mBuffer, mLength);
	serializer.read(mDeviceType);
	serializer.readBuffer((char*)mDeviceID, DEVICE_ID_LENGTH, DEVICE_ID_LENGTH);
	serializer.readBuffer((char*)mPageData, PAGE_DATA_LENGTH, PAGE_DATA_LENGTH);
	serializer.read(mRSSIValue);
	serializer.read(mReserve);
	if (mDeviceType >= RD_HEART_RATE && mDeviceType <= RD_SPEED)
	{
		mRidingDevice = (RIDING_DEVICE)mDeviceType;
	}
	else
	{
		mRidingDevice = RD_NONE;
	}
}
//----------------------------------------------------------------------------------------------------------------------

ANTPlusDataHeartBeat::ANTPlusDataHeartBeat(ANTPlusPacket* packet)
:ANTPlusData(packet)
{
	mCmdID = 0x02;
	setLength(3);
}
void ANTPlusDataHeartBeat::parseData()
{
	txSerializer serializer(mBuffer, mLength);
	serializer.read(mBattery);
	serializer.read(mReserve);
}