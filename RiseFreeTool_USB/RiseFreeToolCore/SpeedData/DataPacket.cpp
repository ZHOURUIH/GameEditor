#include "DataPacket.h"

DataPacket::~DataPacket()
{
	TRACE_DELETE(mHeader);
	TRACE_DELETE(mPayload);
	TRACE_DELETE_ARRAY(mPacketData);
	mDataCount = 0;
}

PARSE_RESULT DataPacket::setData(unsigned char* data, const int& count, int& parsedCount)
{
	// 先解析包头
	PARSE_RESULT ret = mHeader->setData(data, count);
	if (ret != PR_SUCCESS)
	{
		return ret;
	}
	if (mPayload != NULL)
	{
		EDITOR_CORE_ERROR("can not parse again!");
		return ret;
	}
	int headerLength = mHeader->getHeaderLength();
	int cmdID = data[headerLength];
	if (cmdID == CMD_FIT)
	{
		mPayload = TRACE_NEW(FitPayload, mPayload);
	}
	else if (cmdID == CMD_FRICTION)
	{
		mPayload = TRACE_NEW(FrictionPayload, mPayload);
	}
	mPayload->setData(data + headerLength, count - headerLength);
	mDataCount = headerLength + mHeader->mPayloadLength;
	TRACE_DELETE_ARRAY(mPacketData);
	TRACE_NEW_ARRAY(unsigned char, mDataCount, mPacketData);
	memcpy(mPacketData, data, mDataCount);
	parsedCount = mDataCount;
	return ret;
}