#ifndef _SPEED_DATA_MANAGER_H_
#define _SPEED_DATA_MANAGER_H_

#include <map>

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"

class SpeedData;
class HeartRateData;
class CardData;
class SpeedDataManager : public txCommandReceiver, public ToolCoreBase
{
public:
	SpeedDataManager();
	virtual ~SpeedDataManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void destroy(){ clearData(); }
	void setSpeedData(SpeedData* data);
	void setHeartRateData(HeartRateData* data);
	void setFriction(int friction, int machineIndex);
	void setCardData(CardData* cardData);
	CardData* getCardData(){ return mCardData; }
	void clearData();
	std::map<int, SpeedData*>& getSpeedDataList() { return mSpeedDataList; }
	std::map<std::string, HeartRateData*>& getHeartRateDataList() { return mHeartRateDataList; }
	bool isSpeedDataDirty() { return mSpeedDataDirty; }
	void setSpeedDataDirty(bool dirty) { mSpeedDataDirty = dirty; }
	bool isHeartRateDataDirty() { return mHeartRateDataDirty; }
	void setHeartRateDataDirty(bool dirty) { mHeartRateDataDirty = dirty; }
	bool isCardInfoDirty(){ return mCardDirty; }
	void setCardInfoDirty(bool dirty) { mCardDirty = dirty; }
	void setSpeedRatio(float speedRatio) { mSpeedRatio = speedRatio; }
	float getSpeedRatio() { return mSpeedRatio; }
	float getHeartRateTimeOut() { return mHeartRateTimeOut; }
	void setHeartRateTimeOut(float time);
protected:
	std::map<int, SpeedData*> mSpeedDataList;	// 以机器号作索引
	bool mSpeedDataDirty;	// 速度数据是否已经改变过了
	std::map<std::string, HeartRateData*> mHeartRateDataList;	// 以心率带编号作索引
	bool mHeartRateDataDirty;
	CardData* mCardData;
	bool mCardDirty;	// 刷卡数据是否已经改变过了
	float mHeartRateTimeOut;
	float mSpeedRatio;
};

#endif