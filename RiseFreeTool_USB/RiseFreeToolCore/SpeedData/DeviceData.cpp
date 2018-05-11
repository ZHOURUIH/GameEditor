#include "DeviceData.h"
#include "SpeedDataManager.h"

DeviceData::~DeviceData()
{
	txMap<int, DataPacket*>::iterator iter = mDataPacketList.begin();
	txMap<int, DataPacket*>::iterator iterEnd = mDataPacketList.end();
	for (; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	mDataPacketList.clear();
}

void DeviceData::update(float elapsedTime)
{
	txMap<int, DataPacket*>::iterator iter = mDataPacketList.begin();
	txMap<int, DataPacket*>::iterator iterEnd = mDataPacketList.end();
	for (; iter != iterEnd; )
	{
		iter->second->mTimeOut -= elapsedTime;
		if (iter->second->mTimeOut <= 0.0f)
		{
			TRACE_DELETE(iter->second);
			mDataPacketList.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}

void DeviceData::merge(DeviceData* other)
{
	txMap<int, DataPacket*>::iterator iterOther = other->mDataPacketList.begin();
	txMap<int, DataPacket*>::iterator iterOtherEnd = other->mDataPacketList.end();
	for (; iterOther != iterOtherEnd; ++iterOther)
	{
		if (mDataPacketList.find(iterOther->first) != mDataPacketList.end())
		{
			TRACE_DELETE(mDataPacketList[iterOther->first]);
			mDataPacketList[iterOther->first] = iterOther->second;
			mDataPacketList[iterOther->first]->mTimeOut = TIME_OUT;
		}
		else
		{
			mDataPacketList.insert(iterOther->first, iterOther->second);
			iterOther->second->mTimeOut = TIME_OUT;
		}
	}
	other->mDataPacketList.clear();
}

PARSE_RESULT DeviceData::parseData(unsigned char* data, const int& dataCount, int& parsedCount)
{
	DataPacket* dataPacket = TRACE_NEW(DataPacket, dataPacket);
	PARSE_RESULT ret = dataPacket->setData(data, dataCount, parsedCount);
	if (ret == PR_SUCCESS)
	{
		if (mDataPacketList.find(dataPacket->mPayload->mID) != mDataPacketList.end())
		{
			TRACE_DELETE(mDataPacketList[dataPacket->mPayload->mID]);
			mDataPacketList[dataPacket->mPayload->mID] = dataPacket;
		}
		else
		{
			mDataPacketList.insert(dataPacket->mPayload->mID, dataPacket);
		}
	}
	else
	{
		TRACE_DELETE(dataPacket);
	}
	return ret;
}