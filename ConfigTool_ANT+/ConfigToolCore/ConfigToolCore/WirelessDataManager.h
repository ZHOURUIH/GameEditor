#ifndef _WIRELESS_DATA_MANAGER_H_
#define _WIRELESS_DATA_MANAGER_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"
#include "ThreadLock.h"

class ANTPlusPacket;
class WirelessDataManager : public txCommandReceiver, public ToolCoreBase
{
public:
	WirelessDataManager();
	virtual ~WirelessDataManager(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	PARSE_RESULT setData(unsigned char* data, int dataCount, int& parsedCount);
	void clearData();
	void receivePacket(ANTPlusPacket* packet);
	txMap<int, ANTPlusPacket*>& getPacketList() { return mPacketList; }
	ANTPlusPacket* getHeartBeat() { return mHeartBeatPacket; }
	void setSpeedRatio(float speedRatio) { mSpeedRatio = speedRatio; }
	float getSpeedRatio() { return mSpeedRatio; }
	void setWheelPerimeter(float perimeter){ mWheelPerimeter = perimeter; }
	float getWheelPerimeter() { return mWheelPerimeter; }
protected:
	void parseHeartRateData();
	void parseHeartBeatData();
	void syncList();
	int findMachineIndex(char* deviceID);
protected:
	txMap<int, ANTPlusPacket*> mPacketList;	// 以DeviceID为索引
	ANTPlusPacket* mHeartBeatPacket;	// 只有一个心跳数据
	float mSpeedRatio;
	float mWheelPerimeter;
};

#endif