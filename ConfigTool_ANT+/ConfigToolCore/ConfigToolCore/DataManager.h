#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"
#include "txThreadLock.h"

class ANTPlusPacket;
class DataManager : public txCommandReceiver, public ToolCoreBase
{
public:
	DataManager();
	virtual ~DataManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	void destroy();
	PARSE_RESULT setData(unsigned char* data, const int& dataCount, int& parsedCount);
	void clearData();
	txMap<int, ANTPlusPacket*>& getPacketList() { return mPacketList; }
	ANTPlusPacket* getHeartBeat() { return mHeartBeatPacket; }
	void setSpeedRatio(const float& speedRatio) { mSpeedRatio = speedRatio; }
	const float& getSpeedRatio() { return mSpeedRatio; }
	void setWheelPerimeter(const float& perimeter){ mWheelPerimeter = perimeter; }
	const float& getWheelPerimeter() { return mWheelPerimeter; }
protected:
	void parseHeartRateData();
	void parseHeartBeatData();
	void syncList();
protected:
	txMap<int, ANTPlusPacket*> mTempPacketList;
	ANTPlusPacket* mTempHeartBeatPacket;
	txMap<int, ANTPlusPacket*> mPacketList;	// 以DeviceID为索引
	ANTPlusPacket* mHeartBeatPacket;	// 只有一个心跳数据
	float mSpeedRatio;
	float mWheelPerimeter;
	txThreadLock mReceivedLock;
};

#endif