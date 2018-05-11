#ifndef _SPEED_DATA_MANAGER_H_
#define _SPEED_DATA_MANAGER_H_

#include "CommandReceiver.h"
#include "CommonDefine.h"
#include "DataPacket.h"
#include "txThreadLock.h"
#include "EditorCoreBase.h"
#include "DeviceData.h"

class SpeedDataManager : public CommandReceiver, public EditorCoreBase
{
public:
	SpeedDataManager();
	virtual ~SpeedDataManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void destroy(){ clearData(); }
	// data为数据缓冲区,dataCount表示缓冲区中有效数据的长度,parsedCount表示如果解析成功,解析的数据长度
	PARSE_RESULT setData(const std::string& deviceName, unsigned char* data, const int& dataCount, int& parsedCount);
	void clearData();
	DeviceData* getCurDeviceData();
	txMap<std::string, DeviceData*>& getDataList() { return mSpeedDataList; }
	void generateFrictionData(const int& friction, char*& data, int& dataCount);
protected:
	void syncList();
protected:
	txMap<std::string, DeviceData*> mReceivedPacketList;
	txMap<std::string, DeviceData*> mSpeedDataList;	// 以串口名为索引
	txThreadLock mReceivedListLock;
	txMap<int, DataPacket*> mEmptyDataList;
};

#endif