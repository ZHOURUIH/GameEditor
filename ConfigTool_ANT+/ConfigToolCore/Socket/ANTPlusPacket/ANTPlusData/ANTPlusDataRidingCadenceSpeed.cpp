#include "ANTPlusDataRidingCadenceSpeed.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataRidingCadenceSpeed::ANTPlusDataRidingCadenceSpeed(ANTPlusPacket* packet)
:ANTPlusDataRiding(packet)
{
	;
}

int ANTPlusDataRidingCadenceSpeed::getCadence()
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	int index = 2;
	unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
	unsigned short lastCount = BinaryUtility::read(lastCount, lastData->mPageData, index);
	index = 2;
	unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
	unsigned short curCount = BinaryUtility::read(curCount, mPageData, index);
	unsigned short timeDelta = curTime - lastTime;
	unsigned short countDelta = curCount - lastCount;
	if (timeDelta != 0)
	{
		return (int)(countDelta / (timeDelta / 1024.0f) * 60.0f);
	}
	else
	{
		return 0;
	}
}
float ANTPlusDataRidingCadenceSpeed::getSpeed(float perimeter)
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0.0f;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	int index = 4;
	unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
	unsigned short lastCircle = BinaryUtility::read(lastCircle, lastData->mPageData, index);
	index = 4;
	unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
	unsigned short curCircle = BinaryUtility::read(curCircle, mPageData, index);
	unsigned short timeDelta = curTime - lastTime;
	unsigned short circleDelta = curCircle - lastCircle;
	if (timeDelta != 0)
	{
		return circleDelta * perimeter / (timeDelta / 1024.0f);
	}
	else
	{
		return 0.0f;
	}
}

bool ANTPlusDataRidingCadenceSpeed::isSameDataValue(ANTPlusDataRiding* newData)
{
	return BinaryUtility::isMemoryEqual((char*)newData->mPageData, (char*)mPageData, ANTPlusDataRiding::PAGE_DATA_LENGTH);
}