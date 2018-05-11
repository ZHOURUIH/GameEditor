#ifndef _DEVICE_DATA_
#define _DEVICE_DATA_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "DataPacket.h"

class DeviceData : public EditorCoreBase
{
public:
	DeviceData(){}
	virtual ~DeviceData();
	void update(float elapsedTime);
	void merge(DeviceData* other);
	PARSE_RESULT parseData(unsigned char* data, const int& dataCount, int& parsedCount);
public:
	txMap<int, DataPacket*> mDataPacketList;	// 以payload的cmdID为索引
};

#endif