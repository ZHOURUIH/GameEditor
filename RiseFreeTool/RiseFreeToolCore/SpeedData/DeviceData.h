#ifndef _DEVICE_DATA_
#define _DEVICE_DATA_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "DataPacket.h"

class DeviceData : public EditorCoreBase
{
public:
	DeviceData(const std::string& name)
	{
		mDeviceName = name;
	}
	virtual ~DeviceData();
	void update(float elapsedTime);
	void merge(DeviceData* other);
	PARSE_RESULT parseData(unsigned char* data, const int& dataCount, int& parsedCount);
public:
	std::string mDeviceName;
	txMap<int, DataPacket*> mDataPacketList;	// 以payload的cmdID为索引
};

#endif