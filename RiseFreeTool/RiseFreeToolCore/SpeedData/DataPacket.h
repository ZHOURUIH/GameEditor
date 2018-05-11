#ifndef _DATA_PACKET_H_
#define _DATA_PACKET_H_

#include "HeaderData.h"
#include "Payload.h"
#include "EditorCoreLog.h"

class DataPacket
{
public:
	DataPacket()
	{
		mTimeOut = -1.0f;
		mHeader = TRACE_NEW(HeaderData, mHeader);
		mPayload = NULL;
		mPacketData = NULL;
		mDataCount = 0;
	}
	virtual ~DataPacket();
	PARSE_RESULT setData(unsigned char* data, const int& count, int& parsedCount);
protected:
	void updateKey(DataKey* key);
public:
	float mTimeOut;		// 数据未更新超时,小于0则表示数据超时未更新,已失效
	HeaderData* mHeader;
	Payload* mPayload;
	unsigned char* mPacketData;
	int mDataCount;
};

#endif