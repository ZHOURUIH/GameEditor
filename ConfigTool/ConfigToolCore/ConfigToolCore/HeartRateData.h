#ifndef _HEART_RATE_DATA_H_
#define _HEART_RATE_DATA_H_

#include <map>

#include "txCommandReceiver.h"
#include "CommonDefine.h"

class HeartRateData
{
public:
	static const int mDataSize = 9;	// 字节内容的长度
	unsigned char mData[mDataSize];	// 字节内容
	std::string mNumber;			// 心率带编号
	int mHeartRate;					// 心率
	float mTimeOut;					// 数据超时计时

	HeartRateData()
	{
		memset(mData, 0, mDataSize);
		mNumber = "";
		mHeartRate = 0;
		mTimeOut = 0.0f;
	}
	HeartRateData(unsigned char* data, const std::string& number, int heartRate)
	{
		memcpy(mData, data, mDataSize);
		mNumber = number;
		mHeartRate = heartRate;
	}
	HeartRateData& operator = (HeartRateData& that)
	{
		memcpy(mData, that.mData, mDataSize);
		mNumber = that.mNumber;
		mHeartRate = that.mHeartRate;
		mTimeOut = that.mTimeOut;
		return *this;
	}
};

#endif