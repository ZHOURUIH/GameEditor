#include "SpeedDataManager.h"
#include "Utility.h"
#include "SpeedData.h"
#include "HeartRateData.h"
#include "CTEventSystem.h"

SpeedDataManager::SpeedDataManager()
:
txCommandReceiver(TOSTRING(SpeedDataManager)),
mSpeedDataDirty(false),
mHeartRateDataDirty(false),
mHeartRateTimeOut(5.0f),
mSpeedRatio(4.5f)
{}

void SpeedDataManager::update(float elapsedTime)
{
	auto iterSpeed = mSpeedDataList.begin();
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
	auto iterHeartRate = mHeartRateDataList.begin();
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
	auto iterData = mSpeedDataList.find(data->mMachineIndex);
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
		mSpeedDataList.insert(speedData->mMachineIndex, speedData);
	}
	mSpeedDataDirty = true;
}

void SpeedDataManager::setHeartRateData(HeartRateData* data)
{
	// 刷新超时计时
	data->mTimeOut = mHeartRateTimeOut;
	auto iterData = mHeartRateDataList.find(data->mNumber);
	if (iterData != mHeartRateDataList.end())
	{
		*(iterData->second) = *data;
	}
	else
	{
		HeartRateData* heartRateData = TRACE_NEW(HeartRateData, heartRateData);
		*heartRateData = *data;
		mHeartRateDataList.insert(heartRateData->mNumber, heartRateData);
	}
	mHeartRateDataDirty = true;
}

void SpeedDataManager::setFriction(int friction, int machineIndex)
{
	auto iterData = mSpeedDataList.find(machineIndex);
	if (iterData != mSpeedDataList.end())
	{
		iterData->second->mFriction = friction;
		mSpeedDataDirty = true;
	}
}

void SpeedDataManager::clearData()
{
	auto iterSpeed = mSpeedDataList.begin();
	auto iterSpeedEnd = mSpeedDataList.end();
	for (; iterSpeed != iterSpeedEnd; ++iterSpeed)
	{
		TRACE_DELETE(iterSpeed->second);
	}
	mSpeedDataList.clear();
	mSpeedDataDirty = true;
	auto iterHeartRate = mHeartRateDataList.begin();
	auto iterHeartRateEnd = mHeartRateDataList.end();
	for (; iterHeartRate != iterHeartRateEnd; ++iterHeartRate)
	{
		TRACE_DELETE(iterHeartRate->second);
	}
	mHeartRateDataList.clear();
	mHeartRateDataDirty = true;
}

void SpeedDataManager::setHeartRateTimeOut(float time)
{
	mHeartRateTimeOut = time;
	mEventSystem->pushEvent(CE_HEART_RATE_TIME);
}