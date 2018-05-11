#include "SpeedDataManager.h"
#include "txMemeryTrace.h"
#include "SpeedData.h"
#include "HeartRateData.h"
#include "CardData.h"
#include "CTEventSystem.h"

SpeedDataManager::SpeedDataManager()
:
txCommandReceiver("SpeedDataManager"),
mCardDirty(false),
mSpeedDataDirty(false),
mHeartRateDataDirty(false),
mCardData(NULL),
mHeartRateTimeOut(5.0f),
mSpeedRatio(4.5f)
{}

void SpeedDataManager::update(float elapsedTime)
{
	std::map<int, SpeedData*>::iterator iterSpeed = mSpeedDataList.begin();
	for (; iterSpeed != mSpeedDataList.end(); )
	{
		iterSpeed->second->mTimeCount -= elapsedTime;
		if (iterSpeed->second->mTimeCount <= 0.0f)
		{
			TRACE_DELETE(iterSpeed->second);
			mSpeedDataList.erase(iterSpeed++);
			mSpeedDataDirty = true;
		}
		else
		{
			++iterSpeed;
		}
	}
	std::map<std::string, HeartRateData*>::iterator iterHeartRate = mHeartRateDataList.begin();
	for (; iterHeartRate != mHeartRateDataList.end();)
	{
		iterHeartRate->second->mTimeOut -= elapsedTime;
		if (iterHeartRate->second->mTimeOut <= 0.0f)
		{
			TRACE_DELETE(iterHeartRate->second);
			mHeartRateDataList.erase(iterHeartRate++);
			mHeartRateDataDirty = true;
		}
		else
		{
			++iterHeartRate;
		}
	}
}

void SpeedDataManager::setSpeedData(SpeedData* data)
{
	// 默认阻力
	data->mFriction = 1;
	// 刷新超时计时
	data->mTimeCount = SPEED_DATA_TIME_OUT;
	std::map<int, SpeedData*>::iterator iterData = mSpeedDataList.find(data->mMachineIndex);
	if (iterData != mSpeedDataList.end())
	{
		// 保存上一次的阻力
		data->mFriction = iterData->second->mFriction;
		*(iterData->second) = *data;
	}
	else
	{
		SpeedData* speedData = TRACE_NEW(SpeedData, speedData);
		*speedData = *data;
		mSpeedDataList.insert(std::make_pair(speedData->mMachineIndex, speedData));
	}
	mSpeedDataDirty = true;
}

void SpeedDataManager::setHeartRateData(HeartRateData* data)
{
	// 刷新超时计时
	data->mTimeOut = mHeartRateTimeOut;
	std::map<std::string, HeartRateData*>::iterator iterData = mHeartRateDataList.find(data->mNumber);
	if (iterData != mHeartRateDataList.end())
	{
		*(iterData->second) = *data;
	}
	else
	{
		HeartRateData* heartRateData = TRACE_NEW(HeartRateData, heartRateData);
		*heartRateData = *data;
		mHeartRateDataList.insert(std::make_pair(heartRateData->mNumber, heartRateData));
	}
	mHeartRateDataDirty = true;
}

void SpeedDataManager::setFriction(int friction, int machineIndex)
{
	std::map<int, SpeedData*>::iterator iterData = mSpeedDataList.find(machineIndex);
	if (iterData != mSpeedDataList.end())
	{
		iterData->second->mFriction = friction;
		mSpeedDataDirty = true;
	}
}

void SpeedDataManager::setCardData(CardData* cardData)
{
	if (mCardData == NULL)
	{
		mCardData = TRACE_NEW(CardData, mCardData);
	}
	*mCardData = *cardData;
	mCardDirty = true;
}

void SpeedDataManager::clearData()
{
	std::map<int, SpeedData*>::iterator iterSpeed = mSpeedDataList.begin();
	std::map<int, SpeedData*>::iterator iterSpeedEnd = mSpeedDataList.end();
	for (; iterSpeed != iterSpeedEnd; ++iterSpeed)
	{
		TRACE_DELETE(iterSpeed->second);
	}
	mSpeedDataList.clear();
	mSpeedDataDirty = true;
	std::map<std::string, HeartRateData*>::iterator iterHeartRate = mHeartRateDataList.begin();
	std::map<std::string, HeartRateData*>::iterator iterHeartRateEnd = mHeartRateDataList.end();
	for (; iterHeartRate != iterHeartRateEnd; ++iterHeartRate)
	{
		TRACE_DELETE(iterHeartRate->second);
	}
	mHeartRateDataList.clear();
	mHeartRateDataDirty = true;
	TRACE_DELETE(mCardData);
	mCardDirty = true;
}

void SpeedDataManager::setHeartRateTimeOut(float time)
{
	mHeartRateTimeOut = time;
	mEventSystem->pushEvent(CET_HEART_RATE_TIME, std::vector<std::string>());
}