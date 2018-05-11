#ifndef _ANT_PLUS_DATA_H_
#define _ANT_PLUS_DATA_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"

enum RIDING_DEVICE
{
	RD_NONE,
	RD_HEART_RATE = 0x78,
	RD_CADENCE_SPEED = 0x79,
	RD_CADENCE = 0x7A,
	RD_SPEED = 0x7B,
};

class ANTPlusPacket;
class ANTPlusData
{
public:
	ANTPlusPacket* mParentPacket;
	unsigned char mCmdID;
	char* mBuffer;
	int mLength;
public:
	ANTPlusData(ANTPlusPacket* packet);
	~ANTPlusData();
	void setData(char* data);
protected:
	void setLength(const int& length);
	virtual void parseData() = 0;
};

class ANTPlusDataRiding : public ANTPlusData
{
public:
	static const int DEVICE_ID_LENGTH = 4;
	static const int PAGE_DATA_LENGTH = 8;
	RIDING_DEVICE mRidingDevice;
public:
	unsigned char mDeviceType;	// 取值范围为RIDING_DEVICE枚举
	unsigned char mDeviceID[DEVICE_ID_LENGTH];
	unsigned char mPageData[PAGE_DATA_LENGTH];
	unsigned char mRSSIValue;
	short mReserve;
public:
	ANTPlusDataRiding(ANTPlusPacket* packet);
	int getHeartRate();
	int getCadence();
	float getSpeed(const float& perimeter);
protected:
	virtual void parseData();
};

class ANTPlusDataHeartBeat : public ANTPlusData
{
public:
	unsigned char mBattery;	// 电量
	short mReserve;
public:
	ANTPlusDataHeartBeat(ANTPlusPacket* packet);
protected:
	virtual void parseData();
};

#endif